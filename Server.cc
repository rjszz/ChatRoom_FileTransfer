#include "server.h"

int main(int argc, char** argv)
{
    Server* ser=new Server();
    ser->start();
    ser->Handle();
    ser->stop();
return 0;
}