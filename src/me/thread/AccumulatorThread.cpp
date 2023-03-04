#include <me/thread/AccumulatorThread.h>
#include <me/socket/ServerSocket.h>
#include <me/socket/DataSocket.h>
#include <me/Utils.h>
#include <array>
#include <sys/epoll.h>
#include <iostream>
#include <me/book/OrderMatch.h>
#include <functional>
#include <csignal>

using namespace me::thread;

constexpr int MAX_EVENT = 2;


std::vector<me::SPSCQueue<1024> *> AccumulatorThread::m_inputMsgQueues;
int AccumulatorThread::m_epollFd = 0;
me::socket::ServerSocket* AccumulatorThread::m_serverSocket = nullptr;

AccumulatorThread::AccumulatorThread(const std::string& service): m_service(service)
{

}

void AccumulatorThread::start()
{
    if (m_isStarted)
    {
        return;
    }

    m_isStarted = true;
    m_thread = std::thread(&AccumulatorThread::run, this);
}

void AccumulatorThread::run()
{
    signal(SIGPIPE, AccumulatorThread::handleConnectionBreak);

    std::cout << std::this_thread::get_id() << " AccumulatorThread Started " << std::endl;
    m_epollFd = epoll_create1(0);

    if (m_epollFd < 0) 
    {
        DieWithUserMessage(__func__, "Error in open an epoll file descriptor");
    }

    m_serverSocket = new me::socket::ServerSocket(m_service);
    m_serverSocket->setNonBlocking();

    if (m_serverSocket->setUpTCP() == false)
    {
        DieWithUserMessage(__func__, "SetupTCPServerSocket() failed");
    }

    if (m_serverSocket->registerEpoll(m_epollFd) == false) 
    {
        DieWithUserMessage(__func__, "Failed to add listen socket to epoll");
    }

    handleEpollEvent();
}

void AccumulatorThread::handleEpollEvent()
{
    std::array<struct epoll_event, MAX_EVENT> events;

    while (true) 
    {
        int n = epoll_wait(m_epollFd, events.data(), MAX_EVENT, -1);

        for (int i = 0; i < n; ++i) 
        {
            if (events[i].data.ptr == m_serverSocket) 
            {
                if ((events[0].events & EPOLLERR) ||
                    !(events[0].events & EPOLLIN))
                {
                    //?? Need to check further
                    continue;
                }

                auto dataSocket = m_serverSocket->acceptDataSocket();
                sendToClient(*dataSocket);
            }
            else
            {
                auto dataSocket = reinterpret_cast<me::socket::DataSocket*>(events[i].data.ptr);
                if ((events[i].events & EPOLLHUP ||
                    events[i].events & EPOLLERR) ||
                    !(events[i].events & EPOLLIN))
                {
                    delete dataSocket;
                    continue;
                }
            }
        }
    }
}

void AccumulatorThread::handleConnectionBreak(int signal)
{
    handleEpollEvent();
}

void AccumulatorThread::sendToClient(me::socket::DataSocket& dataSocket)
{
    // TODO: now only one upstream thread available , need to change for multiple upstream


    for (auto inputQueue : m_inputMsgQueues)
    {
        for (;;)
        {
            auto orderMatch = reinterpret_cast<me::book::OrderMatch*>(inputQueue->pop());
            auto encodeBuffer = orderMatch->encode();
            dataSocket.sendMsg(encodeBuffer.first, encodeBuffer.second);
            delete orderMatch;
        }
    }
}


