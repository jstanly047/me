#include <me/socket/ConnectSocket.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <me/Utils.h>
#include <me/book/Order.h>
#include <me/book/OrderMatch.h>
#include <sys/epoll.h>


void connectToInput(const char* server, const char* service, long long int size, int numberOfSymbol)
{
    me::socket::ConnectSocket connectSocket(server, service);

    if (connectSocket.connectToTCP() == false)
    {
        DieWithUserMessage("SetupTCPClientSocket() failed", "unable to connect");
    }

    std::vector<std::string> symbols;
    symbols.reserve(numberOfSymbol);

    for (int i = 0; i < numberOfSymbol ; i++)
    {
        symbols.push_back("SYM" + std::to_string(i));
    }

    for (long long int i =0; i < size; i++)
    {
        bool isBuy = (i % 2) == 0;
        unsigned int delta = isBuy ? 1880 : 1884;
        double price = (std::rand() % 10) + delta;
        unsigned long long qty = ((std::rand() % 10) + 1) * 100;
        int symbolIdex = std::rand() % numberOfSymbol;
        //auto order = new me::book::Order(i, isBuy,"OID" + std::to_string(i), symbols[symbolIdex], price, qty);
        auto order = std::make_unique<me::book::Order>(i, isBuy,"OID" + std::to_string(i), symbols[symbolIdex], price, qty);
        auto encodeBuffer = order->encode();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (connectSocket.sendMsg(encodeBuffer.first, encodeBuffer.second) == false)
        {
            DieWithSystemMessage("send() failed");
        }

        delete[] encodeBuffer.first;
    }

    std::string exitStr;
    std::cin >> exitStr;
}

void connectToOutput(const char* server, const char* service)
{
    int epollFd = epoll_create1(0);

    if (epollFd < 0) 
    {
        DieWithUserMessage(__func__, "Error in open an epoll file descriptor");
    }

    me::socket::ConnectSocket connectSocket(server, service);

    if (connectSocket.connectToTCP() == false)
    {
        DieWithUserMessage("SetupTCPClientSocket() failed", "unable to connect");
    }

    if (connectSocket.registerEpoll(epollFd) == false) 
    {
        DieWithUserMessage(__func__, "Failed to add listen socket to epoll");
    }

    std::array<struct epoll_event, 1> events;

    for (;;)
    {
        epoll_wait(epollFd, events.data(), 1, -1);

        auto dataSocket = reinterpret_cast<me::socket::DataSocket*>(events[0].data.ptr);
        if ((events[0].events & EPOLLHUP ||
            events[0].events & EPOLLERR) ||
            !(events[0].events & EPOLLIN))
        {
            delete dataSocket;
            continue;
        }

        for (;;)
        {
            auto encodeBuffer = dataSocket->getNextMessage();

            if (encodeBuffer.first == nullptr)
            {
                break;
            }

            auto orderMatch = me::book::OrderMatch::decode(encodeBuffer.first, encodeBuffer.second);
            delete[] encodeBuffer.first;
            std::cout << "Received Match : " << orderMatch->getSellOrderID() << ":" << orderMatch->getBuyOrderID() << std::endl;
            delete orderMatch;
        }
    }
}

int main(int argc, char *argv[]) 
{
    if (argc != 6)
    { // Test for correct number of arguments
        DieWithUserMessage("Parameter(s)", "<In Server Address/Name> <In Server Port/Service> <Out Server Port/Service> <Number of Orders> <Number Of Symbol>");
    }

    std::thread t1(connectToOutput, argv[1], argv[3]);
    std::thread t2(connectToInput, argv[1], argv[2], atoll(argv[4]), atoi(argv[5]));
    t1.join();
    t2.join();
    
}
