#include "client.h"

int main(int argc, char** argv)
{
    if( argc != 3){
        printf("usage: ./client <ipaddress> <port>\n");
        return 0;
    }
    Client* cli= new Client(argv[1],atoi(argv[2]));
    cli->start();
    cli->handle();
    cli->stop();
pthread_exit(NULL);
return 0;
}