#include <me/thread/WorkerThread.h>
#include <me/thread/AccumulatorThread.h>
#include <iostream>
#include <test.pb.h>

using namespace me::thread;

void WorkerThreadCallback::sendMsgToOutput(void* msg)
{
    assert(m_workerThread);
    m_workerThread->sendMsgToOutput(msg);
}

WorkerThread::WorkerThread(WorkerThreadCallback& workerThreadCallback):m_workerThreadCallback(workerThreadCallback)
{
    m_workerThreadCallback.m_workerThread = this;
}

void WorkerThread::linkOutput(WorkerThread& workerThread)
{
    workerThread.setInputQueue(&m_outputMsgQueue);
    m_outputThread = &workerThread;
}

void WorkerThread::linkAccumulator(AccumulatorThread& accumulatorThread)
{
    accumulatorThread.addInputQueue(&m_outputMsgQueue);
    m_accumulatorThread = &accumulatorThread;
}

void WorkerThread::start()
{
    assert(m_inputMsgQueue);
    assert( (m_outputThread == nullptr && m_accumulatorThread != nullptr) ||
            (m_outputThread != nullptr && m_accumulatorThread == nullptr) ||
            (m_outputThread == nullptr && m_accumulatorThread == nullptr));

    if (m_outputThread)
    {
        m_outputThread->start();
    }
    else if (m_accumulatorThread)
    {
        m_accumulatorThread->start();
    }

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

void WorkerThread::sendMsgToOutput(void* msg)
{
    Person* person = reinterpret_cast<Person*>(msg);
     std::cout << std::this_thread::get_id() << " Worker sending to output " << person->name() << std::endl;
    m_outputMsgQueue.push_back(msg);
}