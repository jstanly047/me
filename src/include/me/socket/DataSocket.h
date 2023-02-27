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

        bool sendMsg(const std::string& message);
        Person* getNextMessage();
    };
}}