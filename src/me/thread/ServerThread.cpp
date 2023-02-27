#include <me/thread/ServerThread.h>
#include <me/socket/ServerSocket.h>
#include <me/socket/DataSocket.h>
#include <me/thread/WorkerThread.h>
#include <me/Utils.h>
#include <thread>
#include <array>
#include <sys/epoll.h>
#include <iostream>
#include <test.pb.h>

using namespace me::thread;

constexpr int MAX_EVENT = 10;

void ServerThread::start(const std::string& service)
{
    int epollFd = epoll_create1(0);

    if (epollFd < 0) 
    {
        DieWithUserMessage(__func__, "Error in open an epoll file descriptor");
    }

    me::socket::ServerSocket serverSocket(service);

    if (serverSocket.setUpTCP() == false)
    {
        DieWithUserMessage(__func__, "SetupTCPServerSocket() failed");
    }

    serverSocket.makeNonBlocking();

    if (serverSocket.registerEpoll(epollFd) == false) 
    {
        DieWithUserMessage(__func__, "Failed to add listen socket to epoll");
    }

    std::array<struct epoll_event, MAX_EVENT> events;

    while (true) 
    {
        int n = epoll_wait(epollFd, events.data(), MAX_EVENT, -1);

        for (int i = 0; i < n; ++i) 
        {
            if (events[i].data.ptr == &serverSocket) 
            {
                if ((events[i].events & EPOLLERR) ||
                    !(events[i].events & EPOLLIN))
                {
                    //?? Need to check further
                    continue;
                }

                auto dataSocket = serverSocket.acceptDataSocket();
                dataSocket->registerEpoll(epollFd);
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

                auto person = dataSocket->getNextMessage();
                std::cout << std::this_thread::get_id() << " send msg person " << person->name() << std::endl;
                m_outputMsgQueue.push_back(person);
            }
        }
    }
}

std::thread ServerThread::startNewThread(const std::string& service)
{
    return std::thread(&ServerThread::start, this, service);
}

void ServerThread::link(WorkerThread& workerThread)
{
    workerThread.setInputQueue(&m_outputMsgQueue);
}
