#pragma once

#include <thread>
#include <string>
#include <me/SPSCQueue.h>

namespace me { namespace thread{
    class WorkerThread;

    class ServerThread
    {
    public:
        ServerThread(const ServerThread& ) = delete;
        ServerThread& operator=(const ServerThread& ) = delete;
        ServerThread() = default;
        void start(const std::string& service);
        std::thread startNewThread(const std::string& service);
        void linkOutput(WorkerThread& workerThread);


    private:
        me::SPSCQueue<1024> m_outputMsgQueue;
        me::thread::WorkerThread* m_outputThread = nullptr;
    };
}}