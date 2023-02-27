#include <thread>
#include <string>
#include <me/SPSCQueue.h>

namespace me { namespace thread{
    class WorkerThread;

    class ServerThread
    {
    public:
        void start(const std::string& service);
        std::thread startNewThread(const std::string& service);
        void link(WorkerThread& workerThread);

    private:
        me::SPSCQueue<1024> m_outputMsgQueue;
    };
}}