#include "network.h"

#if defined(_WIN32)

#include <system_error>
#include <map>
#include <iphlpapi.h>
#include "osal.h"

namespace Network
{

static void InitializeWinSock();
static void UninitializeWinSock();

class WinSockInitializer
{
public:
    WinSockInitializer()
    {
        InitializeWinSock();
    }
    ~WinSockInitializer()
    {
        UninitializeWinSock();
    }
};

static std::map<SocketHandle, bool> socketBlocking;
static WinSockInitializer initializer;

std::ostream & Network::operator << (std::ostream & stream, SocketFamily value)
{
    switch (value)
    {
    case SocketFamily::Unix:
        stream << "UNIX";
        break;
    case SocketFamily::InternetV4:
        stream << "IPV4";
        break;
    case SocketFamily::InternetV6:
        stream << "IPV6";
        break;
    case SocketFamily::IPX:
        stream << "IPX";
        break;
    default:
        stream << "Unknown";
        break;
    }
    return stream;
}

const SocketHandle InvalidHandleValue = INVALID_SOCKET;
const uint32_t TimeWait = 10;
const SocketTimeout InfiniteTimeout = -1; // Infinite timeout

void InitializeWinSock()
{
    WSADATA data;
    int errorCode = WSAStartup(MAKEWORD(2, 2), &data);
    if (errorCode != 0)
    {
        throw std::system_error(errorCode, std::system_category());
    }
    socketBlocking.clear();
}
void UninitializeWinSock()
{
    if (WSACleanup() == SOCKET_ERROR)
    {
        int errorCode = WSAGetLastError();

        throw std::system_error(errorCode, std::system_category());
    }
    socketBlocking.clear();
}

bool LookupAddress(std::string const & hostname, in_addr & address)
{
    addrinfo * addressInfo;
    addrinfo hints = { 0, AF_INET, 0, 0, 0, nullptr, nullptr, nullptr };
    int errorCode = getaddrinfo(hostname.c_str(), 0, &hints, &addressInfo);
    if (errorCode != 0)
        return false;
    address = ((sockaddr_in *)(addressInfo[0].ai_addr))->sin_addr;
    freeaddrinfo(addressInfo);
    return true;
}

std::string GetBindToDevice(SocketHandle UNUSED(handle))
{
    return "";
}

void SetBindToDevice(SocketHandle UNUSED(handle), const std::string & UNUSED(deviceName))
{
}

SocketHandle OpenSocket(SocketFamily socketFamily, SocketType socketType)
{
    SocketHandle result = socket((int)socketFamily, (int)socketType, 0);
    if (result == -1)
    {
        int errorCode = WSAGetLastError();

        throw std::system_error(errorCode, std::system_category());
    }
    socketBlocking.insert_or_assign(result, true);
    return result;
}

void CloseSocket(SocketHandle handle)
{
    if (handle == INVALID_SOCKET)
        return;
    int result = closesocket(handle);
    socketBlocking.erase(handle);

    if (result == SOCKET_ERROR)
    {
        int errorCode = WSAGetLastError();

        throw std::system_error(errorCode, std::system_category());
    }
}

void SetSocketOption(SocketHandle handle, SocketOptionLevel level, SocketOption socketOption, void * optionValue, socklen_t optionLength)
{
    int result = setsockopt(handle, (int)level, (int)socketOption, (char *)optionValue, optionLength);
    if (result == -1)
    {
        int errorCode = WSAGetLastError();

        throw std::system_error(errorCode, std::system_category());
    }
}

void GetSocketOption(SocketHandle handle, SocketOptionLevel level, SocketOption socketOption, void * optionValue, socklen_t & optionLength)
{
    int result = getsockopt(handle, (int)level, (int)socketOption, (char *)optionValue, &optionLength);
    if (result == -1)
    {
        int errorCode = WSAGetLastError();

        throw std::system_error(errorCode, std::system_category());
    }
}

bool GetBlockingMode(SocketHandle handle)
{
    if (socketBlocking.find(handle) != socketBlocking.end())
        return socketBlocking[handle];
    return false;
}

void SetBlockingMode(SocketHandle handle, bool value)
{
    unsigned long mode = value ? 1 : 0;
    int flags = ioctlsocket(handle, FIONBIO, &mode);
    if (flags == -1)
    {
        int errorCode = WSAGetLastError();

        throw std::system_error(errorCode, std::system_category());
    }
    socketBlocking.insert_or_assign(handle, value);
}

void Bind(SocketHandle handle, sockaddr_in const & address)
{
    Bind(handle, (sockaddr *)&address, sizeof(sockaddr_in));
}

void Bind(SocketHandle handle, sockaddr const * address, socklen_t addressLength)
{
    int errorCode = ::bind(handle, address, addressLength);
    if (errorCode == -1)
    {
        int errorCode = WSAGetLastError();

        throw std::system_error(errorCode, std::system_category());
    }
}

bool Connect(SocketHandle handle, sockaddr_in const & serverAddress, SocketTimeout timeout)
{
    return Connect(handle, (const sockaddr *)&serverAddress, sizeof(sockaddr_in), timeout);
}

bool Connect(SocketHandle handle, sockaddr const * serverAddress, socklen_t addressLength, SocketTimeout timeout)
{
    WSAEVENT eventHandle = WSACreateEvent();
    int result = WSAEventSelect(handle, eventHandle, FD_CONNECT);
    int errorCode = 0;
    if (result == -1)
    {
        errorCode = WSAGetLastError();

        throw std::system_error(errorCode, std::system_category());
    }

    result = ::connect(handle, serverAddress, addressLength);
    if (result == -1)
    {
        errorCode = WSAGetLastError();

        if (errorCode == WSAEWOULDBLOCK)
        {
            result = WSAWaitForMultipleEvents(1, &eventHandle, true, timeout == InfiniteTimeout ? WSA_INFINITE : DWORD(timeout), false);
            if (result == WSA_WAIT_EVENT_0)
            {
                errorCode = 0;
            }
        }
    }

    if (WSAEventSelect(handle, eventHandle, 0) != 0)
    {
        errorCode = WSAGetLastError();

        throw std::system_error(errorCode, std::system_category());
    }

    if (!WSACloseEvent(eventHandle))
    {
        errorCode = WSAGetLastError();

        throw std::system_error(errorCode, std::system_category());
    }

    SetBlockingMode(handle, true);

    return (errorCode == 0);
}

void Listen(SocketHandle handle, int numListeners)
{
    int result = ::listen(handle, numListeners);
    if (result == -1)
    {
        int errorCode = WSAGetLastError();

        throw std::system_error(errorCode, std::system_category());
    }
}

bool Accept(SocketHandle serverHandle, SocketHandle & connectionHandle, sockaddr_in & clientAddress, SocketTimeout timeout)
{
    socklen_t addressLength = sizeof(sockaddr_in);
    return Accept(serverHandle, connectionHandle, (sockaddr *)&clientAddress, addressLength, timeout);
}

bool Accept(SocketHandle serverHandle, SocketHandle & connectionHandle, sockaddr * clientAddress, socklen_t & addressLength, SocketTimeout timeout)
{
    WSAEVENT eventHandle = WSACreateEvent();
    int result = WSAEventSelect(serverHandle, eventHandle, FD_ACCEPT);
    int errorCode = 0;
    if (result == -1)
    {
        errorCode = WSAGetLastError();

        throw std::system_error(errorCode, std::system_category());
    }

    SOCKET socketResult = ::accept(serverHandle, clientAddress, &addressLength);
    if (socketResult == INVALID_SOCKET)
    {
        errorCode = WSAGetLastError();

        if (errorCode == WSAEWOULDBLOCK)
        {
            result = WSAWaitForMultipleEvents(1, &eventHandle, true, timeout == InfiniteTimeout ? WSA_INFINITE : DWORD(timeout), false);
            if (result == WSA_WAIT_EVENT_0)
            {
                socketResult = ::accept(serverHandle,clientAddress, &addressLength);
                errorCode = 0;
            }
        }
    }

    if (WSAEventSelect(serverHandle, eventHandle, 0) != 0)
    {
        errorCode = WSAGetLastError();

        throw std::system_error(errorCode, std::system_category());
    }

    if (!WSACloseEvent(eventHandle))
    {
        errorCode = WSAGetLastError();

        throw std::system_error(errorCode, std::system_category());
    }

    SetBlockingMode(serverHandle, true);
    connectionHandle = socketResult;
    return (errorCode == 0);
}

void GetLocalAddress(SocketHandle handle, sockaddr_in & address)
{
    socklen_t addressLength = sizeof(sockaddr_in);
    GetLocalAddress(handle, (sockaddr *)&address, addressLength);
}

void GetLocalAddress(SocketHandle handle, sockaddr * address, socklen_t & addressLength)
{
    int result = ::getsockname(handle, address, &addressLength);
    if (result == -1)
    {
        int errorCode = WSAGetLastError();

        throw std::system_error(errorCode, std::system_category());
    }
}

void GetRemoteAddress(SocketHandle handle, sockaddr_in & address)
{
    socklen_t addressLength = sizeof(sockaddr_in);
    GetRemoteAddress(handle, (sockaddr *)&address, addressLength);
}

void GetRemoteAddress(SocketHandle handle, sockaddr * address, socklen_t & addressLength)
{
    int result = ::getpeername(handle, address, &addressLength);
    if (result == -1)
    {
        int errorCode = WSAGetLastError();

        throw std::system_error(errorCode, std::system_category());
    }
}

size_t Receive(SocketHandle handle, uint8_t * data, size_t bufferSize, int flags)
{
    size_t numBytes = 0;
    try
    {
        ssize_t result = 0;

        result = ::recv(handle, (char *)data, int(bufferSize), flags);
        if (result == -1)
        {
            int errorCode = WSAGetLastError();

            if ((errorCode != WSAEINTR) && (errorCode != WSAEWOULDBLOCK) && (errorCode != EAGAIN))
                throw std::system_error(errorCode, std::system_category());
        }
        else if (result == 0)
        {
            numBytes = -1;
        }
        else
        {
            numBytes = result;
        }
    }
    catch (const std::system_error & e)
    {
        if (e.code().value() == WSAEBADF)
        {
            numBytes = 0;
        }
    }
    return numBytes;
}

bool Send(SocketHandle handle, uint8_t const * data, size_t bytesToSend, int flags)
{
    ssize_t numBytesLeftToSend = bytesToSend;
    size_t offset = 0;

    while (numBytesLeftToSend > 0)
    {
        ssize_t numBytes = ::send(handle, (char *)data + offset, int(numBytesLeftToSend), flags);
        if (numBytes == -1)
        {
            int errorCode = WSAGetLastError();

            if ((errorCode == EPIPE) || (errorCode == ECONNRESET))
            {
                return false;
            }
            throw std::system_error(errorCode, std::system_category());
        }
        else
        {
            offset += numBytes;
            numBytesLeftToSend -= numBytes;
        }
    }
    return true;
}

size_t ReceiveFrom(SocketHandle handle, sockaddr_in & address, uint8_t * data, size_t bufferSize)
{
    socklen_t addressLength = sizeof(sockaddr_in);
    return ReceiveFrom(handle, (sockaddr *)&address, addressLength, data, bufferSize);
}

size_t ReceiveFrom(SocketHandle handle, sockaddr * address, socklen_t & addressLength, uint8_t * data, size_t bufferSize)
{
    ssize_t numBytes = ::recvfrom(handle, (char *)data, int(bufferSize), 0, address, &addressLength);
    if (numBytes == -1)
    {
        int errorCode = WSAGetLastError();

        if ((errorCode != EINTR) && (errorCode != EWOULDBLOCK) && (errorCode != EAGAIN))
            throw std::system_error(errorCode, std::system_category());
        numBytes = 0;
    }

    return numBytes;
}

void SendTo(SocketHandle handle, sockaddr_in const & address, uint8_t const * data, size_t bytesToSend)
{
    SendTo(handle, (sockaddr *)&address, sizeof(sockaddr_in), data, int(bytesToSend));
}

void SendTo(SocketHandle handle, sockaddr const * address, socklen_t addressLength, uint8_t const * data, size_t bytesToSend)
{
    int errorCode = ::sendto(handle, (char *)data, int(bytesToSend), 0, address, addressLength);
    if (errorCode == -1)
    {
        int errorCode = WSAGetLastError();

        throw std::system_error(errorCode, std::system_category());
    }
}

bool ParseAddress(std::string const & text, in_addr & address)
{
    int errorCode = inet_pton(AF_INET, text.c_str(), &address);
    return (errorCode == 1);
}

Adapter::List GetAdaptersForAddressFamily(SocketFamily requestedFamily)
{
    Adapter::List adapterList;
    const size_t BufferSize = 15000;
    PIP_ADAPTER_ADDRESSES interfaces = PIP_ADAPTER_ADDRESSES(new uint8_t[BufferSize]);

    unsigned long numBytes = BufferSize;
    DWORD result = GetAdaptersAddresses((unsigned long)requestedFamily, GAA_FLAG_INCLUDE_PREFIX, nullptr, interfaces, &numBytes);
    if (result != 0)
        throw std::system_error(result, std::system_category());
    PIP_ADAPTER_ADDRESSES networkInterface = interfaces;
    while (networkInterface != nullptr)
    {
        SocketFamily family = SocketFamily::Any;
        sockaddr_in ipAddress;
        sockaddr_in netMask;
        sockaddr_in broadcastAddress;
        bool isLoopback;
        IP_ADAPTER_UNICAST_ADDRESS * unicastAddress = networkInterface->FirstUnicastAddress;

        while (unicastAddress != nullptr)
        {
            family = SocketFamily(unicastAddress->Address.lpSockaddr->sa_family);
            if (family == SocketFamily::InternetV4)
            {
                ipAddress = *(struct sockaddr_in *)unicastAddress->Address.lpSockaddr;
                if (unicastAddress->OnLinkPrefixLength <= 32)
                {
                    unsigned long networkMask;
                    uint32_t address = uint32_t(ipAddress.sin_addr.S_un.S_addr);
                    ConvertLengthToIpv4Mask(unicastAddress->OnLinkPrefixLength, &networkMask);
                    netMask = ipAddress;
                    netMask.sin_addr.S_un.S_addr = networkMask;
                    broadcastAddress = ipAddress;
                    broadcastAddress.sin_addr.S_un.S_addr = uint32_t((address & networkMask) | ~networkMask);
                }
                break;
            }
            unicastAddress = unicastAddress->Next;
        }
        isLoopback = (networkInterface->IfType == IF_TYPE_SOFTWARE_LOOPBACK);
        std::wstring nameWide(networkInterface->FriendlyName);
        std::string name(nameWide.begin(), nameWide.end());

        Adapter::Ptr adapter(std::make_shared<Adapter>(name, family, ipAddress, netMask, broadcastAddress, isLoopback));
        adapterList.push_back(adapter);

        networkInterface = networkInterface->Next;
    }
    delete[] interfaces;

    return adapterList;
}

std::string FindFirstNonLoopbackInterfaceForAddressFamily(SocketFamily addressFamily)
{
    Adapter::List adapterList(GetAdaptersForAddressFamily(addressFamily));
    for (auto adapter : adapterList)
    {
        if (!adapter->IsLoopback())
            return adapter->Name();
    }
    return "";
}

sockaddr_in GetIPAddressForDevice(std::string const & deviceName)
{
    Adapter::List adapterList(GetAdaptersForAddressFamily(SocketFamily::Any));
    for (auto adapter : adapterList)
    {
        if (adapter->Name() == deviceName)
            return adapter->Address();
    }
    return sockaddr_in();
}

sockaddr_in GetNetMaskForDevice(std::string const & deviceName)
{
    Adapter::List adapterList(GetAdaptersForAddressFamily(SocketFamily::Any));
    for (auto adapter : adapterList)
    {
        if (adapter->Name() == deviceName)
            return adapter->NetMask();
    }
    return sockaddr_in();
}

sockaddr_in GetBroadcastAddressForDevice(std::string const & deviceName)
{
    Adapter::List adapterList(GetAdaptersForAddressFamily(SocketFamily::Any));
    for (auto adapter : adapterList)
    {
        if (adapter->Name() == deviceName)
            return adapter->BroadcastAddress();
    }
    return sockaddr_in();
}

} // namespace Network

#endif // define(_WIN32)
