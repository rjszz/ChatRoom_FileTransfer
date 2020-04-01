#include <iostream>
#include <semaphore.h>
#include "unified.h"

std::unordered_map<std::string,int>client_map;

static sem_t get_sem;
static sem_t send_sem; 

void Error(const char* message)
{
    printf(message,strerror(errno),errno);
    exit(0);
}

void send_message(int sockfd)
{
	sem_init(&send_sem,0,1);
	
	char* send_mess=new char[MAXLINE];
	while(1)
    {
        //printf("$");
		//开始输入，不能进行输出
		//sem_wait(&send_sem);
		for(int i=0;i<3;i++) 
		{
			send_mess[i]=(char)scanKeyboard(); 
			if(i==0) 
			{ 
				if(send_mess[0]=='\n') {i--; continue;}	// 直接回车不会发送
				sem_wait(&get_sem);
			}
		}
		
		//从第4个字符开始
        fgets(send_mess+3, MAXLINE-3, stdin);
		
        send_mess[strlen(send_mess)-1]='\0';

		if(strcmp(send_mess,"exit")==0) 
		{
			shutdown(sockfd,SHUT_RDWR); //关闭 套接字 的输入输出端口
			break;} 

        if( send(sockfd, send_mess, strlen(send_mess), 0) < 0)
            Error("send msg error: %s(errno: %d)\n");

		sem_post(&get_sem);
         memset(send_mess,0,strlen(send_mess));
    }
	delete [] send_mess;
}

void get_message(int sockfd)
{
	char* recvbuf=new char[MAXLINE];
	sem_init(&get_sem,0,1);

	while(1)
	{
		int n = recv(sockfd, recvbuf,MAXLINE, 0);
		if(n==0) break;
        recvbuf[n]='\0';
		
		sem_wait(&get_sem);
         printf("\033[32m%s\033[0m\n", recvbuf);
		 sem_post(&get_sem);
	}

}



int scanKeyboard()
{
int in;
struct termios new_settings;
struct termios stored_settings;
tcgetattr(0,&stored_settings);
new_settings = stored_settings;
new_settings.c_lflag &= (~ICANON);
new_settings.c_cc[VTIME] = 0;
tcgetattr(0,&stored_settings);
new_settings.c_cc[VMIN] = 1;
tcsetattr(0,TCSANOW,&new_settings);
 
in = getchar();
 
tcsetattr(0,TCSANOW,&stored_settings);
return in;
}

