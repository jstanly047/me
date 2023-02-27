#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <me/Utils.h>

#define BUFSIZE 2024


void DieWithUserMessage(const char *msg, const char *detail) {
    fputs(msg, stderr);
    fputs(": ", stderr);
    fputs(detail, stderr);
    fputc('\n', stderr);
    exit(1);
}

void DieWithSystemMessage(const char *msg) {
    perror(msg);
    exit(1);
}


void PrintSocketAddress(const struct sockaddr *address, FILE *stream) {
    if (address == NULL || stream == NULL)
        return;
    void *numericAddress;
    char addrBuffer[INET6_ADDRSTRLEN];
    
    in_port_t port;
    switch (address->sa_family)
    {
        case AF_INET:
        numericAddress = &((struct sockaddr_in *) address)->sin_addr;
        port = ntohs(((struct sockaddr_in *) address)->sin_port);
        break;
        case AF_INET6:
        numericAddress = &((struct sockaddr_in6 *) address)->sin6_addr;
        port = ntohs(((struct sockaddr_in6 *) address)->sin6_port);
        break;
        default:
        fputs("[unknown type]", stream);
        return;
    }

    if (inet_ntop(address->sa_family, numericAddress, addrBuffer, sizeof(addrBuffer)) == NULL)
    {
        fputs("[invalid address]", stream);
    }
    else
    {
        fprintf(stream, "%s", addrBuffer);
        if (port != 0)
        {
            fprintf(stream, "-%u", port);
        }
    }
}

void log(const char* msg)
{
    
}