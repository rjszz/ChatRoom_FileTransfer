#include "client.h"


void Client::Socket()
{
     if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        Error("create socket error: %s(errno: %d)\n");

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
   
}

void Client::Connet()
{
    // printf("ip:%s\n",ip);
     if( inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0)
    {
        printf("inet_pton error for %s\n",ip);
        exit(0);
    }

     if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
        Error("connect error: %s(errno: %d)\n");

    printf("Connect to <%s,%d> successfully\n\n",ip,port);
}

void Client::handle()
{
    
    char* recvbuf=new char[MAXLINE]; 
    //首次登录，输入用户名
    do{
     printf("Please enter a nickname:");
     fgets(sendline, MAXLINE, stdin);
     sendline[strlen(sendline)-1]='\0';
    }while(sendline[0]=='\0');
     if( send(sockfd, sendline, strlen(sendline), 0) < 0) //发送用户名
            Error("send msg error: %s(errno: %d)\n");
    
    printf("\nWelcome to 'The little house'!\n");
     printf("Warning:在'The little house'请谨慎使用删除键(Backspace),否则可能会引发灾难\n");
    printf("Please enter 'exit' to exit the program\n");


	std::thread get_thread(get_message,sockfd);
	get_thread.detach();
	
	std::thread send_thread(send_message,sockfd);
	send_thread.detach();
	 
    delete [] recvbuf;
	pthread_exit(NULL);
}

void Client::start()
{
    Socket();
    Connet();
}

