#pragma once

struct timeval;

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
        void setNonBlocking();
        bool registerEpoll(int epollFd);
        bool setRecvBufferSize(int size);
        bool setSendBufferSize(int size);
        bool setReuseAddAndPort();
        bool setCoalescing();
        bool setNoDelay();
        bool setKeepAlive();
        bool setMinimumDataToReturnRecv(int numberOfBytes);
        bool setMinimumDataToReturnSend(int numberOfBytes);
        bool setReceiveTimeOut(const struct timeval &timeout);
        bool setSendTimeOut(const struct timeval &timeout);
        bool setDataFlushTimeoutOnClose(int waitTimeInSec);
        bool setTCPKeepAlive(int maxIdleTime, int interval, int numberOfTry);

    protected:
        BaseSocket(int socketId);
        bool setSocketDescriptorFlags();

        int m_socketId;
        int m_socketFlags = 0;
        int m_socketDescriptorFlags = 0;
    };
}}