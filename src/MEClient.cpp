#include <me/socket/ConnectSocket.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <me/Utils.h>
#include <test.pb.h>



void connectToInput(const char* server, const char* service)
{
    me::socket::ConnectSocket connectSocket(server, service);

    if (connectSocket.connectToTCP() == false)
    {
        DieWithUserMessage("SetupTCPClientSocket() failed", "unable to connect");
    }

    for(;;)
    {
        std::string myMessage;
        std::getline(std::cin, myMessage);

        if (connectSocket.sendMsg(myMessage) == false)
        {
            DieWithSystemMessage("send() failed");
        }
    }
}

void connectToOutput(const char* server, const char* service)
{
    me::socket::ConnectSocket connectSocket(server, service);

    if (connectSocket.connectToTCP() == false)
    {
        DieWithUserMessage("SetupTCPClientSocket() failed", "unable to connect");
    }

    for(;;)
    {
        Person* person = connectSocket.getNextMessage();
        std::cout << "Received Back : " << person->name() << std::endl;
        delete person;
    }
}

int main(int argc, char *argv[]) 
{
    if (argc != 4)
    { // Test for correct number of arguments
        DieWithUserMessage("Parameter(s)", "<In Server Address/Name> <In Server Port/Service> <Out Server Port/Service>");
    }

    std::thread t1(connectToOutput, argv[1], argv[3]);
    std::thread t2(connectToInput, argv[1], argv[2]);
    t1.join();
    t2.join();
    
}
