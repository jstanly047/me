#pragma once

namespace me { namespace socket
{
    constexpr static int NULL_SOCKET = -1;

    class BaseSocket
    {
    public:
        BaseSocket(BaseSocket&& move) noexcept;
        BaseSocket& operator=(BaseSocket&& move) noexcept;
        BaseSocket(const BaseSocket&)               = delete;
        BaseSocket& operator=(const BaseSocket&)    = delete;
        ~BaseSocket();
        bool makeNonBlocking();
        bool registerEpoll(int epollFd);
        bool setRecvBufferSize(int size);

    protected:
        BaseSocket(int socketId);

        int m_socketId;
    };
}}