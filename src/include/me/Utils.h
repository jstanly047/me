#pragma once

#include <stdio.h>
#include <vector>
#include <string>
#include <time.h>

struct sockaddr;



void DieWithUserMessage(const char *msg, const char *detail);
void DieWithSystemMessage(const char *msg);
void PrintSocketAddress(const struct sockaddr *address, FILE *stream);
void log(const char* msg);
std::vector<std::vector<std::string>> parseCSV(const std::string& filename);

namespace me
{
    class TimerClock
    {
    public:
        void begin();
        long long int end();

    private:
        struct timespec m_start;
        struct timespec m_end;
    };
}