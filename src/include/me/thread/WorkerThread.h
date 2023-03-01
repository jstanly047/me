#pragma once

#include <me/SPSCQueue.h>
#include <thread>

namespace me { namespace thread
{
    class WorkerThread;
    class AccumulatorThread;

    class WorkerThreadCallback
    {
    public:
        virtual void onMsg(void* msg) = 0;

    protected:
        void sendMsgToOutput(void* msg);

    private:
        WorkerThread* m_workerThread;

        friend class WorkerThread;
    };

    class WorkerThread
    {
    public:
        WorkerThread(WorkerThreadCallback& workerThreadCallback);
        WorkerThread(const WorkerThread& ) = delete;
        WorkerThread& operator=(const WorkerThread& ) = delete;

        void setInputQueue(me::SPSCQueue<1024> *inputMsgQueue) { m_inputMsgQueue = inputMsgQueue; }
        void linkOutput(WorkerThread& workerThread);
        void linkAccumulator(AccumulatorThread& accumulatorThread);
        void start();
        void join() { m_thread.join(); }

    private:
        void sendMsgToOutput(void* msg);
        void run();

        me::SPSCQueue<1024> *m_inputMsgQueue = nullptr;
        me::thread::WorkerThread* m_outputThread = nullptr;
        me::thread::AccumulatorThread* m_accumulatorThread = nullptr;

        me::SPSCQueue<1024> m_outputMsgQueue;
        WorkerThreadCallback& m_workerThreadCallback;
        std::thread m_thread;

        friend class WorkerThreadCallback;
    };
}}