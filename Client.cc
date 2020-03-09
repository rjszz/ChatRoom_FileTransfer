#include "client.h"

int main(int argc, char** argv)
{
    if( argc != 2){
        printf("usage: ./client <ipaddress>\n");
        return 0;
    }
    Client* cli= new Client(argv[1]);
    cli->start();
    cli->handle();
    cli->stop();

return 0;
}