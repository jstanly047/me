#pragma once

#include "BaseSocket.h"
#include <string>
class Person;

namespace me{ namespace socket
{
    class DataSocket : public BaseSocket
    {
    public:
        DataSocket(int socketID);

        bool sendMsg(uint8_t* buffer, uint32_t size);
        std::pair<uint8_t*, uint32_t> getNextMessage();

    private:
        uint32_t m_networkByteOrderMsgBytesSize = 0;
        uint32_t m_receivedSizeDataCount = 0;
        bool m_isSizeDataRead = false;
        
        uint32_t m_receivedMsgDataCount = 0;
        uint32_t m_expectedMsgSize = 0;
        uint8_t m_msgBuffer[120];
    };
}}