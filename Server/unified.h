#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<thread>

enum {MAXLINE=4096};

void Error(const char*);
void Connect(int connfd,sockaddr_in cliaddr);
