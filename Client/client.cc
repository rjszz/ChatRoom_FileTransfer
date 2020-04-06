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
     if(sendline[0]=='\0') continue;

     //发送用户名并接收回应
     if( send(sockfd, sendline, strlen(sendline), 0) < 0) 
            Error("send msg error: %s(errno: %d)\n");
    int n = recv(sockfd, recvbuf,MAXLINE, 0);
    recvbuf[n]='\0';
    if(strcmp(recvbuf,"no")==0)
    {
         printf("该用户已存在，请更换昵称\n");
         continue;
    }
    break; 
    }while(1);

   
     
    
    printf("\nWelcome to 'The little house'!\n");
    printf("Warning:在'The little house'请谨慎使用删除键(Backspace),否则可能会引发灾难\n\n");
    printf("------------------------------------------------------------------------\n");
    printf("scp(文件传输)指令说明：\n");
    printf("格式：scp filename @username\n");
    printf("说明：1、请注意空格位置\n");
    printf("      2、filename 表示要传输的文件(绝对路径或相对路径)\n");
    printf("      3、username 表示要传输的对象\n");
    printf("      4、本指令可传输任意类型文件\n");
    printf("示例：scp helloworld.jpg @rjszz\n");
    printf("------------------------------------------------------------------------\n\n");
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

