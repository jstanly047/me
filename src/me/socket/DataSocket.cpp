#include<me/socket/DataSocket.h>
#include<string>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<stdio.h>
#include <me/Utils.h>
#include <test.pb.h>
#include <iostream>

using namespace me::socket;


DataSocket::DataSocket(int socketID) : BaseSocket(socketID)
{
}

bool DataSocket::sendMsg(uint8_t* buffer, uint32_t size)
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
    
    uint32_t networkByteOrderMsgBytesSize = htonl(size);
    sendMessage(&networkByteOrderMsgBytesSize, sizeof(uint32_t));
    sendMessage(buffer, size);
    return true;
}

std::pair<uint8_t*, uint32_t>  DataSocket::getNextMessage()
{
    auto receiveMsg = [&](uint8_t* buf, size_t size, uint32_t& readCount)->bool
    {
        while (readCount < size)
        {
            uint8_t* receiveBuffer = buf + readCount;
            ssize_t numBytesRecv = recv(m_socketId, receiveBuffer, size - readCount, MSG_DONTWAIT);
        
            if (numBytesRecv < 0)
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    return false;
                }
                else
                {
                    std:: cout << "recv() failed" << std::endl;
                    return false;
                }
            }
            else if (numBytesRecv == 0)
            {
                std::cout << "recv() connection closed prematurely" << std::endl;
                return false;
            }

            readCount += numBytesRecv;
        }

        return true;
    };

    
    if (m_isSizeDataRead == false)
    {
        bool isReadFully = receiveMsg(reinterpret_cast<uint8_t*>(&m_networkByteOrderMsgBytesSize), sizeof(uint32_t), m_receivedSizeDataCount);

        if (isReadFully == false)
        {
            //std::cout << "size size read " <<  m_receivedSizeDataCount
            return {nullptr, 0};
        }
        
        m_isSizeDataRead = true;
        m_receivedSizeDataCount = 0;
        m_expectedMsgSize = ntohl(m_networkByteOrderMsgBytesSize);
        m_networkByteOrderMsgBytesSize = 0;
    }
    
    bool isReadFully = receiveMsg(m_msgBuffer, m_expectedMsgSize, m_receivedMsgDataCount);

    if (isReadFully == false)
    {
        return {nullptr, 0};
    }

    m_isSizeDataRead = false;
    m_receivedMsgDataCount = 0;
    return {m_msgBuffer,  m_expectedMsgSize};
}
