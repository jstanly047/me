#pragma once
#include <me/SPSCQueue.h>
#include <thread>
#include <vector>
#include <mutex>

namespace me { namespace socket
{
    class DataSocket;
}}

namespace me { namespace thread
{
    class AccumulatorThread
    {
    public:
        AccumulatorThread(const AccumulatorThread& ) = delete;
        AccumulatorThread& operator=(const AccumulatorThread& ) = delete;

        AccumulatorThread(const std::string& service);
        void addInputQueue(me::SPSCQueue<1024> *inputMsgQueue) { m_inputMsgQueues.push_back(inputMsgQueue); }
        void start();
        void join() { m_thread.join(); }

    private:
        void run();
        void sendToClient(me::socket::DataSocket& dataSocket);

        bool m_isStarted = false;
        std::string m_service;
        std::vector<me::SPSCQueue<1024> *> m_inputMsgQueues;
        std::thread m_thread;
    };
}}