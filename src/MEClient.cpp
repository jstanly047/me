#include <me/socket/ConnectSocket.h>
#include <string.h>
#include <iostream>
#include <me/Utils.h>



int main(int argc, char *argv[]) 
{
    if (argc < 3 || argc > 4)
    { // Test for correct number of arguments
        DieWithUserMessage("Parameter(s)", "<Server Address/Name> <Echo Word> [<Server Port/Service>]");
    }

    char *server = argv[1];
    char *echoString = argv[2];
    const char *service = (argc == 4) ? argv[3] : "echo";
    me::socket::ConnectSocket connectSocket(server, service);

    if (connectSocket.connectToTCP() == false)
    {
        DieWithUserMessage("SetupTCPClientSocket() failed", "unable to connect");
    }

    if (connectSocket.sendMsg(echoString) == false)
    {
        DieWithSystemMessage("send() failed");
    }

    for(;;)
    {
        std::string myMessage;
        std::getline(std::cin, myMessage);
        connectSocket.sendMsg(myMessage);
    }
}
