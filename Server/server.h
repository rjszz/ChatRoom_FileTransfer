#ifndef   SERVER_H
#define   SERVER_H
#include "unified.h"

class Server
{
    private:
        int listenfd;
        int connfd;
        struct sockaddr_in  servaddr;
        char*  buff;
        int port;
        int maxsize;
    
        void Error(const char*);


    public:
        Server(int Po=8000,int Size=50):port(Po),maxsize(Size){buff = new char[MAXLINE];};

        bool Socket();
        bool Bind();
        bool Listen();
        void Handle();
        void start();
        void stop(){close(listenfd);};

        ~Server(){delete [] buff;}

};

#endif