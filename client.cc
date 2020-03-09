#include "client.h"

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
    printf("send msg to server: \n");
    fgets(sendline, MAXLINE, stdin);
    if( send(sockfd, sendline, strlen(sendline), 0) < 0)
        Error("send msg error: %s(errno: %d)\n");
    // {
    //     printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
    //     return 0;
    // }
}

void Client::start()
{
    Socket();
    Connet();
}

