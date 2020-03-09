#include "server.h"

void Server::Error(const char* message)
{
    printf(message,strerror(errno),errno);
     exit(0);
}

bool Server::Socket()
{
    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
        Error("create socket error: %s(errno: %d)\n");
        //printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        // return 0;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

return true;
}

bool Server::Bind()
{
     if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
            Error("bind socket error: %s(errno: %d)\n");
//      {
//         printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
//         return 0;
//     }
return true;
}

bool Server::Listen()
{

    if( listen(listenfd, maxsize) == -1)
         Error("listen socket error: %s(errno: %d)\n");
    // {
    //     printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
    //     return 0;
    // }
return true;
}

void Server::Handle()
{
     printf("======waiting for client's request======\n");
    while(1){
        if( (connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1){
            printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
            continue;
        }
        int n = recv(connfd, buff, MAXLINE, 0);
        buff[n] = '\0';
        printf("recv msg from client: %s\n", buff);
        close(connfd);
    }
}

void Server::start()
{
    Socket();
    Bind();
    Listen();
}