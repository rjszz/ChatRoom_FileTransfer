#include "unified.h"

void Error(const char* message)
{
    printf(message,strerror(errno),errno);
    exit(0);
}

void Connect(int connfd,sockaddr_in cliaddr)
{
    char* buff=new char[MAXLINE];
     printf("Connect client:<%s,%d>\n",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));
    while(1)
    {
        int n = recv(connfd, buff, MAXLINE, 0);
        
        if(n==0)
        {
            printf("Disconnect client:<%s,%d>\n",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));
            break;
        }
        printf("recv msg from client: %s\n", buff);

         if( send(connfd, buff, n, 0) < 0)       //发送回客户端
             Error("send msg error: %s(errno: %d)\n");
         memset(buff,0,strlen(buff));
    }
    delete [] buff;
}