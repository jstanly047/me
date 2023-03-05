#include <me/socket/ConnectSocket.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <me/Utils.h>
#include <me/book/Order.h>
#include <me/book/OrderMatch.h>
#include <sys/epoll.h>
#include <me/utility/TimerClock.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>

uint32_t writeToFile(std::unique_ptr<me::book::Order>& order, std::ofstream& fd)
{
    auto encodeBuffer = order->encode();
    uint32_t networkByteOrderMsgBytesSize = htonl(encodeBuffer.second);
    fd.write(reinterpret_cast<const char*>(&networkByteOrderMsgBytesSize), sizeof(uint32_t));
    fd.write(reinterpret_cast<const char*>(encodeBuffer.first), encodeBuffer.second);
    return (encodeBuffer.second + sizeof(uint32_t));
}

size_t generateOrderInFile(int size, int numberOfSymbol)
{
    

    std::ofstream outfile("order.bin", std::ios::out | std::ios::binary |std::ios::trunc);
    std::vector<std::string> symbols;
    symbols.reserve(numberOfSymbol);

    for (int i = 0; i < numberOfSymbol ; i++)
    {
        symbols.push_back("SYM" + std::to_string(i));
    }

    size_t totalBytes = 0;
    for (long long int i =0; i < size; i++)
    {
        bool isBuy = (i % 2) == 0;
        unsigned int delta = isBuy ? 1880 : 1884;
        double price = (std::rand() % 10) + delta;
        unsigned long long qty = ((std::rand() % 10) + 1) * 100;
        int symbolIdex = std::rand() % numberOfSymbol;
        //auto order = new me::book::Order(i, isBuy,"OID" + std::to_string(i), symbols[symbolIdex], price, qty);
        auto order = std::make_unique<me::book::Order>(i, isBuy,"OID" + std::to_string(i), symbols[symbolIdex], price, qty);
        totalBytes += writeToFile(order, outfile);
    }

    auto order = std::make_unique<me::book::Order>(size + 1, me::book::Order::BUY,"END1", "END", 1.0, 1);
    totalBytes += writeToFile(order, outfile);

    order = std::make_unique<me::book::Order>(size + 2, me::book::Order::SELL,"END2", "END", 1.0, 1);
    totalBytes += writeToFile(order, outfile);

    /*off_t offset = lseek(fd, 0, SEEK_SET);
    if (offset == -1) {
        DieWithSystemMessage("Seek reset failed");
    }*/
    outfile.flush();
    outfile.close();
    return totalBytes;
}

void connectToInput(const char* server, const char* service, int totalBytes)
{
    me::socket::ConnectSocket connectSocket(server, service);
    connectSocket.setBufferTCPSendData();

    if (connectSocket.connectToTCP() == false)
    {
        DieWithUserMessage("SetupTCPClientSocket() failed", "unable to connect");
    }

    std::cout << "Send Buff Size: " << connectSocket.getSendBufferSize() << std::endl;

    me::utility::AccumulateAndAverage sendTime("Time Took Send Order");

    sendTime.begin();

    int fd = open("order.bin", O_RDONLY,  S_IRUSR | S_IWUSR);

    if (fd == -1)
    {
         DieWithSystemMessage("Failed to open file");
    }

    if (connectSocket.sendFile(fd, totalBytes) == false)
    {
        DieWithSystemMessage("Can not send file");
    }
    sendTime.end(totalBytes);    
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
    std::cout << "Recv Buff Size: " << connectSocket.getRcvBufferSize() << std::endl;

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

    auto totalBytes = generateOrderInFile(atoll(argv[4]), atoi(argv[5]));
    std::thread t1(connectToOutput, argv[1], argv[3]);
    std::thread t2(connectToInput, argv[1], argv[2], totalBytes);
    t1.join();
    t2.join();
    
}
