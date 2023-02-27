#include <me/SPSCQueue.h>
#include <thread>

namespace me { namespace thread
{
    class WorkerThreadCallback
    {
    public:
        virtual void onMsg(void* msg) = 0;
    };

    class WorkerThread
    {
    public:
        WorkerThread(WorkerThreadCallback& workerThreadCallback);
        void setInputQueue(me::SPSCQueue<1024> *inputMsgQueue) { m_inputMsgQueue = inputMsgQueue; }
        void link(WorkerThread& workerThread);
        void start();
        void join() { m_thread.join(); }

    private:
        void run();

        me::SPSCQueue<1024> *m_inputMsgQueue = nullptr;
        me::SPSCQueue<1024> m_outputMsgQueue;
        WorkerThreadCallback& m_workerThreadCallback;
        std::thread m_thread;
    };
}}