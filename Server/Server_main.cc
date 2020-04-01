#include <semaphore.h>

#include "server.h"

sem_t sem; //信号量

int main(int argc, char** argv)
{
	sem_init(&sem, 0,1);
    int port=8000;
    if(argc==2) port=atoi(argv[1]);
    Server* ser=new Server(port);
    ser->start();
    ser->Handle();
    ser->stop();
return 0;
}