#include "network.h"

#include <system_error>
#include <map>
#include <fcntl.h>
#include "osal.h"

namespace Network
{

std::ostream & operator << (std::ostream & stream, SocketFamily value)
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
    case SocketFamily::Packet:
        stream << "Packet";
        break;
    default:
        stream << "Unknown";
        break;
    }
    return stream;
}

const SocketHandle InvalidHandleValue = -1;
const uint32_t TimeWait = 10;
const SocketTimeout InfiniteTimeout = -1; // Infinite timeout

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

std::string GetBindToDevice(SocketHandle handle)
{
    ifreq ifr;
    bzero(&ifr, sizeof(ifr));
    socklen_t optionLength = sizeof(ifr);
    GetSocketOption(handle, SocketOptionLevel::Socket, SocketOption::BindToDevice, &ifr, optionLength);
    return ifr.ifr_ifrn.ifrn_name;
}

void SetBindToDevice(SocketHandle handle, const std::string & deviceName)
{
    ifreq ifr;
    bzero(&ifr, sizeof(ifr));
    strncpy(ifr.ifr_ifrn.ifrn_name, deviceName.c_str(), sizeof(ifr.ifr_ifrn.ifrn_name));
    SetSocketOption(handle, SocketOptionLevel::Socket, SocketOption::BindToDevice, &ifr, sizeof(ifr));
}

SocketHandle OpenSocket(SocketFamily socketFamily, SocketType socketType)
{
    SocketHandle result = socket((int)socketFamily, (int)socketType, 0);
    if (result == -1)
    {
        int errorCode = errno;

        throw std::system_error(errorCode, std::system_category());
    }
    return result;
}

void CloseSocket(SocketHandle handle)
{
    if (handle == InvalidHandleValue)
        return;
    int result = close(handle);
    if (result == -1)
    {
        int errorCode = errno;

		throw std::system_error(errorCode, std::system_category());
    }
}

void SetSocketOption(SocketHandle handle, SocketOptionLevel level, SocketOption socketOption, void * optionValue, socklen_t optionLength)
{
    int result = setsockopt(handle, (int)level, (int)socketOption, optionValue, optionLength);
    if (result == -1)
    {
        int errorCode = errno;

		throw std::system_error(errorCode, std::system_category());
    }
}

void GetSocketOption(SocketHandle handle, SocketOptionLevel level, SocketOption socketOption, void * optionValue, socklen_t & optionLength)
{
    int result = getsockopt(handle, (int)level, (int)socketOption, optionValue, &optionLength);
    if (result == -1)
    {
        int errorCode = errno;

		throw std::system_error(errorCode, std::system_category());
    }
}

bool GetBlockingMode(SocketHandle handle)
{
    int flags = fcntl(handle, F_GETFL);
    if (flags == -1)
    {
        int errorCode = errno;

		throw std::system_error(errorCode, std::system_category());
    }
    return (flags & O_NONBLOCK) == 0;
}

void SetBlockingMode(SocketHandle handle, bool value)
{
    int flags = fcntl(handle, F_GETFL);
    if (flags == -1)
    {
        int errorCode = errno;

 		throw std::system_error(errorCode, std::system_category());
    }
    int errorCode = fcntl(handle, F_SETFL, value ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK));
    if (errorCode == -1)
    {
        int errorCode = errno;

		throw std::system_error(errorCode, std::system_category());
    }
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
        int errorCode = errno;

        throw std::system_error(errorCode, std::system_category());
    }
}

bool Connect(SocketHandle handle, sockaddr_in const & serverAddress, SocketTimeout timeout)
{
    return Connect(handle, (const sockaddr *)&serverAddress, sizeof(sockaddr_in), timeout);
}

bool Connect(SocketHandle handle, sockaddr const * serverAddress, socklen_t addressLength, SocketTimeout timeout)
{
    if (timeout != InfiniteTimeout)
    {
        SetBlockingMode(handle, false);
    }
    else
    {
        SetBlockingMode(handle, true);
    }

    int result = ::connect(handle, serverAddress, addressLength);
    if (result == -1)
    {
        int errorCode = errno;

        if ((errorCode == EINPROGRESS) || (errorCode == EALREADY))
        {
            pollfd descriptor;
            descriptor.fd = handle;
            descriptor.events = POLLIN | POLLOUT;
            descriptor.revents = 0;
            int pollResult = ::poll(&descriptor, 1, timeout);
            if (pollResult == -1)
            {
                errorCode = errno;
            }
            else if (pollResult == 0)
            {
                return false;
            }
            else
            {
                if ((descriptor.revents & POLLHUP))
                    return false;
                result = 0;
            }
        }
        else if ((errorCode != EWOULDBLOCK) && (errorCode != EAGAIN))
            throw std::system_error(errorCode, std::system_category());
    }

    SetBlockingMode(handle, true);
    return (result != -1);
}

void Listen(SocketHandle handle, int numListeners)
{
    int result = ::listen(handle, numListeners);
    if (result == -1)
    {
        int errorCode = errno;

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
    if (timeout != InfiniteTimeout)
    {
        SetBlockingMode(serverHandle, false);
    }
    else
    {
        SetBlockingMode(serverHandle, true);
    }

    uint32_t waitTime = 0;
    if (timeout != InfiniteTimeout)
    {
        waitTime = timeout;
    }

    int result = 0;
    do
    {
        result = ::accept(serverHandle, clientAddress, &addressLength);
        if (result == -1)
        {
            int errorCode = errno;

            if (((errorCode == EWOULDBLOCK) || (errorCode == EAGAIN)) && (waitTime > 0))
            {
                usleep(TimeWait * 1000);
                waitTime -= std::min(waitTime, TimeWait);
            }
            else if (errorCode == EBADF)
            {
                return false;
            }
            else
                throw std::system_error(errorCode, std::system_category());
        }
    }
    while ((result == -1) && (waitTime > 0));

    SetBlockingMode(serverHandle, true);
    connectionHandle = result;
    return (result != -1);
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
        int errorCode = errno;

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
        int errorCode = errno;

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
            int errorCode = errno;

            if ((errorCode != EINTR) && (errorCode != EWOULDBLOCK) && (errorCode != EAGAIN))
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
        if (e.code().value() == EBADF)
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
            int errorCode = errno;

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
        int errorCode = errno;

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
    int errorCode = ::sendto(handle, data, int(bytesToSend), 0, address, addressLength);
    if (errorCode == -1)
    {
        int errorCode = errno;

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
    ifaddrs * interfaces = nullptr;
    try
    {
        if (getifaddrs(&interfaces) != 0)
            throw std::system_error(errno, std::system_category());
        ifaddrs * networkInterface = interfaces;
        while (networkInterface != nullptr)
        {
            SocketFamily family = SocketFamily::Any;
            sockaddr_in ipAddress {0, 0, {0}, 0};
            sockaddr_in netMask {0, 0, {0}, 0};
            sockaddr_in broadcastAddress {0, 0, {0}, 0};
            bool isLoopback {};
            if (networkInterface->ifa_addr != nullptr)
            {
                family = SocketFamily(networkInterface->ifa_addr->sa_family);
            }
            if ((requestedFamily == SocketFamily::Any) || (family == requestedFamily))
            {
                if (family == SocketFamily::InternetV4)
                {
                    ipAddress = *(struct sockaddr_in *)networkInterface->ifa_addr;
                    if (networkInterface->ifa_netmask != nullptr)
                    {
                        netMask = *(struct sockaddr_in *)networkInterface->ifa_netmask;
                    }
                    if (networkInterface->ifa_broadaddr != nullptr)
                    {
                        broadcastAddress = *(struct sockaddr_in *)networkInterface->ifa_broadaddr;
                    }
                }
				isLoopback = (std::string(networkInterface->ifa_name) == "lo");
				Adapter::Ptr adapter(std::make_shared<Adapter>(networkInterface->ifa_name, family, ipAddress, netMask, broadcastAddress, isLoopback));
                adapterList.push_back(adapter);
            }
            networkInterface = networkInterface->ifa_next;
        }
    }
    catch (...)
    {
        freeifaddrs(interfaces);
        throw;
    }
    freeifaddrs(interfaces);
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
    Adapter::List adapterList(GetAdaptersForAddressFamily(SocketFamily::InternetV4));
    for (auto adapter : adapterList)
    {
        if (adapter->Name() == deviceName)
            return adapter->Address();
    }
    return sockaddr_in();
}

sockaddr_in GetNetMaskForDevice(std::string const & deviceName)
{
    Adapter::List adapterList(GetAdaptersForAddressFamily(SocketFamily::InternetV4));
    for (auto adapter : adapterList)
    {
        if (adapter->Name() == deviceName)
            return adapter->NetMask();
    }
    return sockaddr_in();
}

sockaddr_in GetBroadcastAddressForDevice(std::string const & deviceName)
{
    Adapter::List adapterList(GetAdaptersForAddressFamily(SocketFamily::InternetV4));
    for (auto adapter : adapterList)
    {
        if (adapter->Name() == deviceName)
            return adapter->BroadcastAddress();
    }
    return sockaddr_in();
}

} // namespace Network
