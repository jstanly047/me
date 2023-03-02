#include <string.h>
#include <me/Utils.h>
#include <me/thread/ServerThread.h>
#include <me/thread/WorkerThread.h>
#include <me/thread/AccumulatorThread.h>
#include <me/handle/OrderProcess.h>
#include <test.pb.h>


int main(int argc, char *argv[]) 
{
    if (argc != 3)
    {
        DieWithUserMessage("Parameter(s)", "<In Server Port/Service> <Out Server Port/Service>");
    }

    me::handle::OrderProcess orderProcess;
    me::thread::ServerThread serverThread;
    me::thread::WorkerThread workerThread(orderProcess);
    me::thread::AccumulatorThread accumulatorThread(argv[2]);

    serverThread.linkOutput(workerThread);
    workerThread.linkAccumulator(accumulatorThread);

    serverThread.start(argv[1]);
    workerThread.join();
    accumulatorThread.join();
}