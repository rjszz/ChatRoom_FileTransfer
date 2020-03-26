#include "client.h"
#include<iostream>
using namespace std;

void Client::Error(const char* message)
{
    printf(message,strerror(errno),errno);
    exit(0);
}

void Client::Socket()
{
     if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        Error("create socket error: %s(errno: %d)\n");
    //  {
    //     printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
    //     return 0;
    // }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
   
}

void Client::Connet()
{
    printf("ip:%s\n",ip);
     if( inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0)
    {
        printf("inet_pton error for %s\n",ip);
        exit(0);
    }
    printf("port:%d\n",port);
     if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
        Error("connect error: %s(errno: %d)\n");
    //  {
    //     printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
    //     return 0;
    // }
}

void Client::handle()
{
    printf("Please enter 'exit' to exit the program\n");
    char* recvbuf=new char[MAXLINE]; 
    while(1)
    {
        printf("$");

        fgets(sendline, MAXLINE, stdin);
        sendline[strlen(sendline)-1]='\0';
        if(sendline[0]=='\0') continue;
        else if(strcmp(sendline,"exit")==0) break;

        if( send(sockfd, sendline, strlen(sendline), 0) < 0)
            Error("send msg error: %s(errno: %d)\n");
        
        int n = recv(sockfd, recvbuf,MAXLINE, 0);
        recvbuf[n]='\0';
         printf("recv msg from server: %s\n", recvbuf);

         memset(sendline,0,strlen(sendline));
         memset(sendline,0,strlen(recvbuf));
    }
    delete [] recvbuf;
}

void Client::start()
{
    Socket();
    Connet();
}

