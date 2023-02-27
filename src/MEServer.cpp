#include <string.h>
#include <me/Utils.h>
#include <me/thread/ServerThread.h>
#include <me/thread/WorkerThread.h>
#include <test.pb.h>

class PersonMsgReader : public me::thread::WorkerThreadCallback
{
    void onMsg(void* msg) override
    {
        Person* person = reinterpret_cast<Person*>(msg);
        std::cout << std::this_thread::get_id() << " Worker Got " << person->name() << std::endl;
    }
};


int main(int argc, char *argv[]) 
{
    if (argc != 2)
    {
        DieWithUserMessage("Parameter(s)", "<Server Port/Service>");
    }

    char *service = argv[1];
    PersonMsgReader msgReader;
    me::thread::ServerThread serverThread;
    me::thread::WorkerThread workerThread(msgReader);
    serverThread.link(workerThread);
    workerThread.start();

    serverThread.start(service);
    workerThread.join();
}