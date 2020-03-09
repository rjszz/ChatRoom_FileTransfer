#ifndef   CLIENT_H
#define   CLIENT_H
#include "unified.h"

class Client
{
    private:
        int sockfd;
        char* recvline;
        char* sendline;
        char* ip;
        int port;
        struct sockaddr_in  servaddr;

         void Error(const char*);
    
    public:
        Client(char* IP,int Po=8000):ip(IP),port(Po)
        {
            
            sendline = new char [MAXLINE];
        }

        void Socket();
        void Connet();
        void handle();
        void start();
        void stop(){close(sockfd);}

        ~Client()
        {
            delete [] sendline;
        }
};
#endif