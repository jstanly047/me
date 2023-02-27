#include <me/socket/BaseSocket.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <iostream>
#include <me/Utils.h>

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

bool BaseSocket::makeNonBlocking()
{
    int flags = fcntl(m_socketId, F_GETFL, 0);
    if (flags == -1) {
        std::cerr << m_socketId << "  Error in getting status flags\n";
        return false;
    }

    flags |= O_NONBLOCK;
    int s = fcntl(m_socketId, F_SETFL, flags);
    if (s == -1) {
        std::cerr << "Error in setting status flags\n";
        return false;
    }

    return true;
}

bool BaseSocket::registerEpoll(int epollFd)
{
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.ptr = this;
    return epoll_ctl(epollFd, EPOLL_CTL_ADD, m_socketId, &event) == 0;
}