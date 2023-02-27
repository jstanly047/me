#include <me/thread/WorkerThread.h>
#include <iostream>

using namespace me::thread;

WorkerThread::WorkerThread(WorkerThreadCallback& workerThreadCallback):m_workerThreadCallback(workerThreadCallback)
{

}

void WorkerThread::link(WorkerThread& workerThread)
{
    workerThread.setInputQueue(&m_outputMsgQueue);
}

void WorkerThread::start()
{
    assert(m_inputMsgQueue);

    m_thread = std::thread(&WorkerThread::run, this);
}

void WorkerThread::run()
{
    std::cout << std::this_thread::get_id() << " Worker Started " << std::endl;
    for (;;)
    {
        m_workerThreadCallback.onMsg(m_inputMsgQueue->pop());
    }
}