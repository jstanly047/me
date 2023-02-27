#include<me/socket/DataSocket.h>
#include<string>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<stdio.h>
#include <me/Utils.h>
#include <test.pb.h>

using namespace me::socket;


DataSocket::DataSocket(int socketID) : BaseSocket(socketID)
{
}

bool DataSocket::sendMsg(const std::string& message)
{
    if (m_socketId == NULL_SOCKET)
    {
        return false;
    }

    auto sendMessage = [&](const void* buf, size_t size)
    {
        ssize_t numBytesSend = send(m_socketId, buf, size, 0);
    
        if (numBytesSend < 0)
        {
            DieWithSystemMessage("send() failed");
        }
        else if (numBytesSend != (ssize_t) size)
        {
            DieWithUserMessage("send()", "sent unexpected number of bytes");
        }

        return true;
    };

    Person personMsg;
    personMsg.set_name(message.c_str());
    
    uint32_t msgBytesSize =  (uint32_t) personMsg.ByteSizeLong();
    uint32_t networkByteOrderMsgBytesSize = htonl(msgBytesSize);
    sendMessage(&networkByteOrderMsgBytesSize, sizeof(uint32_t));
    std::vector<char> messageBytes;
    messageBytes.reserve(msgBytesSize);
    personMsg.SerializeToArray(messageBytes.data(), msgBytesSize);
    sendMessage(messageBytes.data(), msgBytesSize);
    return true;
}

Person* DataSocket::getNextMessage()
{
    auto receiveMsg = [&](uint8_t* buf, size_t size)->bool
    {
        size_t totalBytesRcvd = 0;

        while (totalBytesRcvd < size)
        {
            uint8_t* receiveBuffer = buf + totalBytesRcvd;
            ssize_t numBytesRecv = recv(m_socketId, receiveBuffer, size - totalBytesRcvd, 0);
        
            if (numBytesRecv < 0)
            {
                DieWithSystemMessage("recv() failed");
            }
            else if (numBytesRecv == 0)
            {
                DieWithUserMessage("recv()", "connection closed prematurely");
            }

            totalBytesRcvd += numBytesRecv;
        }

        return true;
    };

    uint32_t networkByteOrderMsgBytesSize;
    receiveMsg(reinterpret_cast<uint8_t*>(&networkByteOrderMsgBytesSize), sizeof(uint32_t));
    uint32_t msgByteSize = ntohl(networkByteOrderMsgBytesSize);
    uint8_t* megBuffer = new uint8_t[msgByteSize];
    receiveMsg(megBuffer, msgByteSize);
    Person* personMsg = new Person();
    personMsg->ParseFromArray(megBuffer, msgByteSize);
    delete[] megBuffer;
    return personMsg;
}
