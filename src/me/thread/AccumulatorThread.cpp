#include <me/thread/AccumulatorThread.h>
#include <me/socket/ServerSocket.h>
#include <me/socket/DataSocket.h>
#include <me/Utils.h>
#include <array>
#include <sys/epoll.h>
#include <iostream>
#include <test.pb.h>

using namespace me::thread;

constexpr int MAX_EVENT = 1;

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
    std::cout << std::this_thread::get_id() << " AccumulatorThread Started " << std::endl;
    int epollFd = epoll_create1(0);

    if (epollFd < 0) 
    {
        DieWithUserMessage(__func__, "Error in open an epoll file descriptor");
    }

    me::socket::ServerSocket serverSocket(m_service);

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
        epoll_wait(epollFd, events.data(), MAX_EVENT, -1);

        if (events[0].data.ptr == &serverSocket) 
        {
            if ((events[0].events & EPOLLERR) ||
                !(events[0].events & EPOLLIN))
            {
                //?? Need to check further
                continue;
            }

            auto dataSocket = serverSocket.acceptDataSocket();
            sendToClient(*dataSocket);
        }
    }
}

void AccumulatorThread::sendToClient(me::socket::DataSocket& dataSocket)
{
    // TODO: now only one upstream thread available , need to change for multiple upstream
    for (auto inputQueue : m_inputMsgQueues)
    {
        for (;;)
        {
            Person* person = reinterpret_cast<Person*>(inputQueue->pop());
            std::cout << std::this_thread::get_id() << " AccumulatorThread Got " << person->name() << std::endl;
            dataSocket.sendMsg(person->name());
            delete person;
        }
    }
}


