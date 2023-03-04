#include <me/socket/BaseSocket.h>
#include <iostream>
#include <me/Utils.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <assert.h>

using namespace me::socket;

BaseSocket::BaseSocket(int socketId) : m_socketId(socketId)
{
}

BaseSocket::~BaseSocket()
{
    if (m_socketId != NULL_SOCKET)
    {
        close(m_socketId);
    }   
}

BaseSocket::BaseSocket(BaseSocket&& move) noexcept
{
    m_socketId = move.m_socketId;
    move.m_socketId = -1;
}

BaseSocket& BaseSocket::operator=(BaseSocket&& move) noexcept
{
    m_socketId = move.m_socketId;
    move.m_socketId = -1;
    return *this;
}

void BaseSocket::setNonBlocking()
{
    m_socketDescriptorFlags |= O_NONBLOCK;
}

bool BaseSocket::registerEpoll(int epollFd)
{
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.ptr = this;
    return epoll_ctl(epollFd, EPOLL_CTL_ADD, m_socketId, &event) == 0;
}

bool BaseSocket::setReuseAddAndPort()
{
    int flag = 1;
    if (setsockopt(m_socketId, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) == -1) 
    {
        return false;
    }
    
    return true;
}

bool BaseSocket::setCoalescing()
{
    int flag = 1;
    if (setsockopt(m_socketId, SOL_SOCKET, SO_MAX_PACING_RATE, &flag, sizeof(flag)) == -1) 
    {
        return false;
    }
    
    return true;
}

bool BaseSocket::setNoDelay()
{
    int flag = 1;
    if (setsockopt(m_socketId, SOL_SOCKET, SO_MAX_PACING_RATE, &flag, sizeof(flag)) == -1) 
    {
        return false;
    }
    
    return true  ;
}

bool BaseSocket::setRecvBufferSize(int size)
{
    if (setsockopt(m_socketId, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size)) == -1) 
    {
        return false;
    }

    return true;
}

bool BaseSocket::setSendBufferSize(int size)
{
    if (setsockopt(m_socketId, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size)) == -1) 
    {
        return false;
    }

    return true;
}

bool BaseSocket::setKeepAlive()
{
    int keepAlive = 1;
    if (setsockopt(m_socketId, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, sizeof(keepAlive)) == -1) 
    {
        return false;
    }

    return true;
}

// No impact when MSG_DONTWAIT or socket is non-blocking
bool BaseSocket::setReceiveTimeOut(const struct timeval &timeout)
{
    if (setsockopt(m_socketId, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1)
    {
        return false;
    }

    return true;
}

// No impact when MSG_DONTWAIT or socket is non-blocking
bool BaseSocket::setSendTimeOut(const struct timeval &timeout)
{
    if (setsockopt(m_socketId, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) == -1) 
    {
        return false;
    }

    return true;
}

// No impact when MSG_DONTWAIT pass to recv
bool BaseSocket::setMinimumDataToReturnRecv(int numberOfBytes)
{
    if (setsockopt(m_socketId, SOL_SOCKET, SO_RCVLOWAT, &numberOfBytes, sizeof(numberOfBytes)) == -1) 
    {
        return false;
    }

    return true;
}

// No impact when MSG_DONTWAIT pass to recv
bool BaseSocket::setMinimumDataToReturnSend(int numberOfBytes)
{
    if (setsockopt(m_socketId, SOL_SOCKET, SO_SNDLOWAT, &numberOfBytes, sizeof(numberOfBytes)) == -1) 
    {
        return false;
    }

    return true;
}

// Wait for unsent data to be sent on close
bool BaseSocket::setDataFlushTimeoutOnClose(int waitTimeInSec)
{
    struct linger soLinger;
    soLinger.l_onoff = 1;
    soLinger.l_linger = waitTimeInSec;

    if (setsockopt(m_socketId, SOL_SOCKET, SO_LINGER, &soLinger, sizeof(soLinger)) == -1) 
    {
        return false;
    }

    return true;
}

bool BaseSocket::setBufferTCPSendData()
{
    int bufferSendData = 1;
    if (setsockopt(m_socketId, SOL_SOCKET, TCP_CORK, &bufferSendData, sizeof(bufferSendData)) == -1) 
    {
        return false;
    }

    return true;
}

bool BaseSocket::setMaxSendDataWaitThreshold(int numberOfBytes)
{
    if (setsockopt(m_socketId, SOL_SOCKET, TCP_NOTSENT_LOWAT, &numberOfBytes, sizeof(numberOfBytes)) == -1) 
    {
        return false;
    }

    return true;
}

bool  BaseSocket::setTCPKeepAlive(int maxIdleTime, int interval, int numberOfTry)
{
    assert(maxIdleTime>0 && interval > 0 && numberOfTry > 0);
    int keepAlive = 1;
    if (setsockopt(m_socketId, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, sizeof(keepAlive)) == -1) 
    {
        return false;
    }
    
    // Allowed time in seconds to be idle, after this idle time system sends first keep-alive probe
    if (setsockopt(m_socketId, IPPROTO_TCP, TCP_KEEPIDLE, &maxIdleTime, sizeof(maxIdleTime)) == -1) 
    {
        return false;
    }

    // Subsequent keep-alive probe interval in seconds
    if (setsockopt(m_socketId, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(interval)) == -1) 
    {
        return false;
    }

    // Number keep-alive probe attempt before closing the socket
    if (setsockopt(m_socketId, IPPROTO_TCP, TCP_KEEPCNT, &numberOfTry, sizeof(numberOfTry)) == -1) 
    {
        return false;
    }

    return true;
}

bool BaseSocket::setSocketDescriptorFlags()
{
    int flags = fcntl(m_socketId, F_GETFL, 0);

    if (flags == -1) 
    {
        std::cerr << m_socketId << "  Error in getting status flags\n";
        return false;
    }

    flags |= m_socketDescriptorFlags;
    int s = fcntl(m_socketId, F_SETFL, flags);

    if (s == -1) 
    {
        std::cerr << "Error in setting status flags\n";
        return false;
    }

    return true;
}