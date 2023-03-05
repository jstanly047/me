#include <me/socket/ConnectSocket.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <me/Utils.h>
#include <me/book/Order.h>
#include <me/book/OrderMatch.h>
#include <sys/epoll.h>

void sendOrdersFromFile(me::socket::ConnectSocket& connectSocket, const std::string& fileName)
{
    auto rows = parseCSV(fileName);

    unsigned long long int i=1;
    for (auto &row : rows)
    {
        auto order = std::make_unique<me::book::Order>(i++, row[0] == "BUY", row[1], row[2], std::stod(row[3]), std::stoull(row[4]));
        auto encodeBuffer = order->encode();

        if (connectSocket.sendMsg(encodeBuffer.first, encodeBuffer.second) == false)
        {
            DieWithSystemMessage("send() failed");
        }
    }
}


void connectToInput(const char* server, const char* service, const std::string& orderFileName)
{
    me::socket::ConnectSocket connectSocket(server, service);

    if (connectSocket.connectToTCP() == false)
    {
        DieWithUserMessage("SetupTCPClientSocket() failed", "unable to connect");
    }

    sendOrdersFromFile(connectSocket, orderFileName);
    std::string exitStr;
    std::cin >> exitStr;
}

void connectToOutput(const char* server, const char* service, const std::string& orderMatchFile)
{
    auto rows = parseCSV(orderMatchFile);
    auto rowItr = rows.begin();
    bool isPassed = true;

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

            if (orderMatch->getSymbol() != (*rowItr)[0] || 
                orderMatch->getSellOrderID() != (*rowItr)[1] ||
                orderMatch->getAskPrice() != std::stod((*rowItr)[2]) ||
                orderMatch->getMatchedQuantity() != std::stoull((*rowItr)[3]) ||
                orderMatch->getBuyOrderID() != (*rowItr)[4] ||
                orderMatch->getBidPrice() != std::stod((*rowItr)[5]))
            {
                std::cout << "(" << orderMatch->toString() << ") NOT MATCH WITH EXPECTED (" 
                        << (*rowItr)[0] << "," << (*rowItr)[1] << "," << (*rowItr)[2] << "," <<
                         (*rowItr)[3] << "," << (*rowItr)[4] << "," << (*rowItr)[5] << ")" << std::endl;
                isPassed = false;
            }
            ++rowItr;
            delete orderMatch;

            if (rowItr == rows.end())
            {
                if (isPassed)
                {
                    std::cout << "ALL Passed" << std::endl;
                }
                else
                {
                    std::cout << "Failed" << std::endl;
                }
                exit(0);
            }
        }
    }
}

int main(int argc, char *argv[]) 
{
    if (argc != 6)
    { // Test for correct number of arguments
        DieWithUserMessage("Parameter(s)", "<In Server Address/Name> <In Server Port/Service> <Out Server Port/Service> <Input Order File> <Match Validation File>");
    }

    std::thread t1(connectToOutput, argv[1], argv[3], argv[5]);
    std::thread t2(connectToInput, argv[1], argv[2], argv[4]);
    t1.join();
    t2.join();
    
}
