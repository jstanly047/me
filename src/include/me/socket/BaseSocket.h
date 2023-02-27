#pragma once

namespace me { namespace socket
{
    constexpr static int NULL_SOCKET = -1;

    class BaseSocket
    {
    public:
        BaseSocket(BaseSocket&& move) noexcept;
        BaseSocket& operator=(BaseSocket&& move) noexcept;
        BaseSocket(BaseSocket const&)               = delete;
        BaseSocket& operator=(BaseSocket const&)    = delete;
        ~BaseSocket();
        bool makeNonBlocking();
        bool registerEpoll(int epollFd);

    protected:
        BaseSocket(int socketId);

        int m_socketId;
    };
}}