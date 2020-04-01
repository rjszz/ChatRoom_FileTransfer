#ifndef UNIFIED_H
#define UNIFIED_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<thread>
#include<unordered_map>
#include <semaphore.h>


// struct User
// {
    // std::string username;
    // struct sockaddr_in cliaddr;
    // char* message;
// };
extern sem_t sem;

enum {MAXLINE=4096};

void Error(const char*);
void Connect(int connfd,sockaddr_in cliaddr);

#endif
