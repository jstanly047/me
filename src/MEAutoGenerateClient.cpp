#include <me/socket/ConnectSocket.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <me/Utils.h>
#include <me/book/Order.h>
#include <me/book/OrderMatch.h>
#include <sys/epoll.h>
#include <me/utility/TimerClock.h>

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

    me::utility::TimerClock totalTime("Encode and Send Orders");
    me::utility::AccumulateAndAverage sendTime("Time Took Send Order");
    totalTime.begin();
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
        sendTime.begin();

        if (connectSocket.sendMsg(encodeBuffer.first, encodeBuffer.second) == false)
        {
            DieWithSystemMessage("send() failed");
        }
        sendTime.end(encodeBuffer.second);
    }

    auto order = std::make_unique<me::book::Order>(size + 1, me::book::Order::BUY,"END1", "END", 1.0, 1);
    auto encodeBuffer = order->encode();
    sendTime.begin();
    if (connectSocket.sendMsg(encodeBuffer.first, encodeBuffer.second) == false)
    {
        DieWithSystemMessage("send() failed");
    }
    sendTime.end(encodeBuffer.second);

    order = std::make_unique<me::book::Order>(size + 2, me::book::Order::SELL,"END2", "END", 1.0, 1);
    encodeBuffer = order->encode();
    sendTime.begin();
    if (connectSocket.sendMsg(encodeBuffer.first, encodeBuffer.second) == false)
    {
        DieWithSystemMessage("send() failed");
    }
    sendTime.end(encodeBuffer.second);
    totalTime.end();

    std::cout << totalTime.getStatInMilliSec() << std::endl;
    std::cout << sendTime.getStatInMilliSec() << std::endl;

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

    me::utility::TimerClock receiveTime("Receive Order");
    receiveTime.begin();

    bool breakWait = false;

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

            breakWait = (orderMatch->getSymbol()  == "END");
           // std::cout << "Received Match : " << orderMatch->getSellOrderID() << ":" << orderMatch->getBuyOrderID() << std::endl;
            delete orderMatch;
        }

        if (breakWait)
        {
            break;
        }
    }

    receiveTime.end();
    std::cout << receiveTime.getStatInMilliSec() << std::endl;
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
