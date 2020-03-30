#include "server.h"

// void Server::Error(const char* message)
// {
//     printf(message,strerror(errno),errno);
//      exit(0);
// }

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

    // struct sockaddr_in cliaddr;     //客户端的ip地址与端口
    // socklen_t addrlen=sizeof(cliaddr);      //cliaddr的长度
    //  if( (connfd = accept(listenfd,(struct sockaddr*)&cliaddr,&addrlen)) == -1){
    //         printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
    //         return;
    //     }
    
    // printf("Connect client:<%s,%d>\n",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));

    while(1){
            struct sockaddr_in cliaddr;     //客户端的ip地址与端口
            socklen_t addrlen=sizeof(cliaddr);      //cliaddr的长度

           if( (connfd = accept(listenfd,(struct sockaddr*)&cliaddr,&addrlen)) == -1){
            printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
            return;
        }
        std::thread mythread(Connect,connfd,cliaddr);
        mythread.detach();
        // int n = recv(connfd, buff, MAXLINE, 0);
        // if(n==0)
        // {
        //     printf("Disconnect client:<%s,%d>",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));
        //     break;
        // }
        // printf("recv msg from client: %s\n", buff);

        //  if( send(connfd, buff, n, 0) < 0)       //发送回客户端
        //      Error("send msg error: %s(errno: %d)\n");
        // printf("recv msg from client: %s\n", buff);
        //  memset(buff,0,strlen(buff));
        
    }
    close(connfd);
}

// void Server::Thread(int connfd,sockaddr_in cliaddr)
// {
//     char* buff==new char[MAXLINE];
//     while(1)
//     {
//         int n = recv(connfd, buff, MAXLINE, 0);
//         if(n==0)
//         {
//             printf("Disconnect client:<%s,%d>",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));
//             break;
//         }
//         printf("recv msg from client: %s\n", buff);

//          if( send(connfd, buff, n, 0) < 0)       //发送回客户端
//              Error("send msg error: %s(errno: %d)\n");
//         printf("recv msg from client: %s\n", buff);
//          memset(buff,0,strlen(buff));
//     }
//     delete [] buff;
// }
void Server::start()
{
    Socket();
    Bind();
    Listen();
}