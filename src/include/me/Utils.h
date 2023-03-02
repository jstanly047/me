#pragma once

#include <stdio.h>
#include <vector>
struct sockaddr;



void DieWithUserMessage(const char *msg, const char *detail);
void DieWithSystemMessage(const char *msg);
void PrintSocketAddress(const struct sockaddr *address, FILE *stream);
void log(const char* msg);