#pragma once

#include <memory>
#include <string>
#include <vector>
#if defined(_MSC_VER)
#include "network/windows/socket-defs.h"
#elif defined(__GNUC__)
#include "network/posix/socket-defs.h"
#endif

namespace Network
{

bool ParseAddress(std::string const & text, in_addr & address);
bool LookupAddress(std::string const & hostname, in_addr & address);

std::string GetBindToDevice(SocketHandle handle);
void SetBindToDevice(SocketHandle handle, const std::string & deviceName);

SocketHandle OpenSocket(SocketFamily socketFamily, SocketType socketType);
void CloseSocket(SocketHandle handle);
void SetSocketOption(SocketHandle handle, SocketOptionLevel level, SocketOption socketOption, void * optionValue, socklen_t optionLength);
void GetSocketOption(SocketHandle handle, SocketOptionLevel level, SocketOption socketOption, void * optionValue, socklen_t & optionLength);

bool GetBlockingMode(SocketHandle handle);
void SetBlockingMode(SocketHandle handle, bool value);

void Bind(SocketHandle handle, sockaddr_in const & address);
void Bind(SocketHandle handle, sockaddr const * address, socklen_t addressLength);
bool Connect(SocketHandle handle, sockaddr_in const & serverAddress, SocketTimeout timeout);
bool Connect(SocketHandle handle, sockaddr const * serverAddress, socklen_t addressLength, SocketTimeout timeout);
bool Accept(SocketHandle serverHandle, SocketHandle & connectionHandle, sockaddr_in & clientAddress, SocketTimeout timeout);
bool Accept(SocketHandle serverHandle, SocketHandle & connectionHandle, sockaddr * clientAddress, socklen_t & addressLength, SocketTimeout timeout);
void Listen(SocketHandle handle, int numListeners);

void GetLocalAddress(SocketHandle handle, sockaddr_in & address);
void GetLocalAddress(SocketHandle handle, sockaddr * address, socklen_t & addressLength);
void GetRemoteAddress(SocketHandle handle, sockaddr_in & address);
void GetRemoteAddress(SocketHandle handle, sockaddr * address, socklen_t & addressLength);

size_t Receive(SocketHandle handle, uint8_t * data, size_t bufferSize, int flags);
bool Send(SocketHandle handle, uint8_t const * data, size_t bytesToSend, int flags);
size_t ReceiveFrom(SocketHandle handle, sockaddr_in & address, uint8_t * data, size_t bufferSize);
size_t ReceiveFrom(SocketHandle handle, sockaddr * address, socklen_t & addressLength, uint8_t * data, size_t bufferSize);
void SendTo(SocketHandle handle, sockaddr_in const & address, uint8_t const * data, size_t bytesToSend);
void SendTo(SocketHandle handle, sockaddr const * address, socklen_t addressLength, uint8_t const * data, size_t bytesToSend);

class Adapter
{
public:
    typedef std::shared_ptr<Adapter> Ptr;
    typedef std::vector<Ptr> List;

    Adapter() = delete;
    Adapter(const Adapter &) = delete;
    Adapter(std::string const & name, SocketFamily family,
        sockaddr_in const & ipAddress,
        sockaddr_in const & netMask,
        sockaddr_in const & broadcastAddress,
        bool isLoopBack)
        : name(name)
        , family(family)
        , ipAddress(ipAddress)
        , netMask(netMask)
        , broadcastAddress(broadcastAddress)
        , isLoopback(isLoopBack)
    {
    }

    Adapter & operator = (const Adapter &) = delete;

    std::string const & Name() const { return name; }
    SocketFamily AddressFamily() const { return family; }
    sockaddr_in const & Address() const { return ipAddress; }
    sockaddr_in const & NetMask() const { return netMask; }
    sockaddr_in const & BroadcastAddress() const { return broadcastAddress; }
    bool IsLoopback() const { return isLoopback; }

    bool operator ==(const Adapter & other) const
    {
        if (this == &other)
            return true;
        return (name == other.name) &&
            (family == other.family) &&
            (ipAddress == other.ipAddress) &&
            (netMask == other.netMask) &&
            (broadcastAddress == other.broadcastAddress) &&
            (isLoopback == other.isLoopback);
    }
    bool operator !=(const Adapter & other) const
    {
        return !operator ==(other);
    }

private:
    std::string name;
    SocketFamily family;
    sockaddr_in ipAddress;
    sockaddr_in netMask;
    sockaddr_in broadcastAddress;
    bool isLoopback;
};

Adapter::List GetAdaptersForAddressFamily(SocketFamily requestedFamily);
std::string FindFirstNonLoopbackInterfaceForAddressFamily(SocketFamily addressFamily);
sockaddr_in GetIPAddressForDevice(std::string const & deviceName);
sockaddr_in GetNetMaskForDevice(std::string const & deviceName);
sockaddr_in GetBroadcastAddressForDevice(std::string const & deviceName);

} // namespace Network
