#pragma once

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

namespace Network
{

typedef int SocketTimeout;
typedef SOCKET SocketHandle;

extern const SocketHandle InvalidHandleValue;
extern const uint32_t TimeWait;
extern const SocketTimeout InfiniteTimeout;

enum class SocketFamily
{
	Any = AF_UNSPEC,
	Unix = AF_UNIX,
	Internet = AF_INET,
	InternetV4 = AF_INET,
	InternetV6 = AF_INET6,
	IPX = AF_IPX,
};

std::ostream & operator << (std::ostream & stream, SocketFamily value);

enum class SocketType
{
	Stream = SOCK_STREAM,
	Datagram = SOCK_DGRAM,
	Raw = SOCK_RAW,
	SeqPacket = SOCK_SEQPACKET,
};

enum class SocketOptionLevel
{
	Socket = SOL_SOCKET,
};

enum class SocketOption
{
	Debug = SO_DEBUG,
	ReuseAddress = SO_REUSEADDR,
	Type = SO_TYPE,
	Error = SO_ERROR,
	DontRoute = SO_DONTROUTE,
	Broadcast = SO_BROADCAST,
	SendBuffer = SO_SNDBUF,
	ReceiveBuffer = SO_RCVBUF,
	KeepAlive = SO_KEEPALIVE,
	OutOfBandInline = SO_OOBINLINE,
	Linger = SO_LINGER,
	ReceiveTimeout = SO_RCVTIMEO,
	SendTimeout = SO_SNDTIMEO,
};

inline void SetIPAddress(sockaddr_in & address, uint32_t ipAddress)
{
    address.sin_addr.S_un.S_addr = ipAddress;
}
inline uint32_t GetIPAddress(sockaddr_in const & address)
{
    return address.sin_addr.S_un.S_addr;
}
inline bool operator == (sockaddr_in const & lhs, sockaddr_in const & rhs)
{
    return (lhs.sin_family == rhs.sin_family) &&
        (lhs.sin_port == rhs.sin_port) &&
        (lhs.sin_addr.S_un.S_addr == rhs.sin_addr.S_un.S_addr);
}

} // namespace Network
