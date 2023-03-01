#include <string.h>
#include <me/Utils.h>
#include <me/thread/ServerThread.h>
#include <me/thread/WorkerThread.h>
#include <me/thread/AccumulatorThread.h>
#include <test.pb.h>

class PersonMsgReader : public me::thread::WorkerThreadCallback
{
private:
    void onMsg(void* msg) override
    {
        Person* person = reinterpret_cast<Person*>(msg);
        std::cout << std::this_thread::get_id() << " Worker Got " << person->name() << std::endl;
        sendMsgToOutput(msg);
    }
};


int main(int argc, char *argv[]) 
{
    if (argc != 3)
    {
        DieWithUserMessage("Parameter(s)", "<In Server Port/Service> <Out Server Port/Service>");
    }

    PersonMsgReader msgReader;
    me::thread::ServerThread serverThread;
    me::thread::WorkerThread workerThread(msgReader);
    me::thread::AccumulatorThread accumulatorThread(argv[2]);

    serverThread.linkOutput(workerThread);
    workerThread.linkAccumulator(accumulatorThread);

    serverThread.start(argv[1]);
    workerThread.join();
    accumulatorThread.join();
}