#include "unified.h"
#include<iostream>

#define ACCEPT yes
#define UNACCEPT no

static bool TransFile=false;
static bool acFinish=false;
char TransMess[4];

std::unordered_map<std::string,int>client_map;


void Error(const char* message)
{
    printf(message,strerror(errno),errno);
     exit(0);
}

static std::string getName(int connfd)
{
        
        for(auto it:client_map)
                if(it.second==connfd) return it.first;
}

void send_message(const char* buff,int n)
{
	//sem_wait(&sem);
		for(auto it:client_map)
			 if( send(it.second, buff, n, 0) < 0)       //发送回客户端
				Error("send msg error: %s(errno: %d)\n");
	 //sem_post(&sem);
}

static bool del_message(int connfd,char* mess)
{
        // printf("mess1:%c%c%c\n",mess[0],mess[1],mess[2]);
        if(mess[0]!='s' || mess[1]!='c' || mess[2]!='p') return true;
       // printf("mess:%s\n",mess);
        char username[FILE_NAME_MAX_SIZE+1];
        char buff[MAXLINE];

        int i,len=0;
        for(i=3;mess[i]!='\0';i++)
            username[len++]=mess[i];
        username[len]='\0';
        //std::cout<<"username:"<<username<<'\n';
        std::string str(username,strlen(username));
        //std::cout<<"str:"<<str<<'\n';
        if(!client_map.count(str))
        {
                send(connfd, "error", 6, 0);        //用户已下线
                return false;
        }
                    
        TransFile=true;

        int otherfd=client_map[str];  //获得该用户套接字

        char message[FILE_NAME_MAX_SIZE+1]={'s','c','p','\0'};
        // bzero(message,FILE_NAME_MAX_SIZE+1);

        std::string name=getName(connfd);
        strcpy(message+3,name.c_str());
        printf("message:%s\n",message);
        send(otherfd,message, strlen(message), 0);  //向用户发送传输请求

        while (!acFinish);    //等待回复
     
        
        //bzero(buff,MAXLINE);
         //int n = recv(otherfd, buff, MAXLINE, 0);   //等待用户回复
        //printf("buff:%s\n",TransMess);
         send(connfd,TransMess,strlen(TransMess),0);

         if(strcmp(TransMess,"yes")==0)
         {
             int length=0;
             bzero(buff, MAXLINE);
             while((length = recv(connfd, buff, MAXLINE, 0)) > 0)
            {
               // printf("buff:%s\n",buff);
                send(otherfd,buff,length,0);
                if(strcmp(buff+length-4,"@$@")==0) break;       //结束标志
                 bzero(buff, MAXLINE);
            }
         }

         bzero(username,FILE_NAME_MAX_SIZE+1);
         bzero(TransMess,4);
         TransFile=false;
         acFinish=false;
         return false;
}


void Connect(int connfd,sockaddr_in cliaddr)
{
    char* buff=new char[MAXLINE];
    //获得用户名并判断是否在已经存在
    int name_len=0;
    std::string name;
    do{
            bzero(buff,MAXLINE);
            name_len = recv(connfd, buff, MAXLINE, 0);
            if(name_len==0) return;
            name=std::string (buff,name_len);
            if(client_map.count(name))
            {        
                    send(connfd,"no", 3, 0);
                    continue;
            }
            client_map[name]=connfd;
            break;          
    }while (1);
    
    //提示信息
    printf("Connect client:<%s,%d>\nusername:%s\n",
            inet_ntoa(cliaddr.sin_addr),
            ntohs(cliaddr.sin_port),
            name.c_str());
    
    std::string sys_message="System message:<"+name+"> is coming in";

    send_message(sys_message.c_str(),strlen(sys_message.c_str()));

    // struct User user;
    // user.username=name; 
    
     buff[name_len]=':';
    while(1)
    {
        int n = recv(connfd, buff+name_len+1, MAXLINE, 0);
        
        if(n==0)
        {
            printf("Disconnect client:<%s,%d>\n<%s> has left the room\n",
						inet_ntoa(cliaddr.sin_addr),
						ntohs(cliaddr.sin_port),
						name.c_str());
            client_map.erase(name);
             std::string exit_message ="System message:<"+name+"> is leaving";
             send_message(exit_message.c_str(),strlen(exit_message.c_str()));            
            break;
        }
        if(TransFile)
        {
            int len=0;
            for(int i=name_len+1;buff[i]!='\0';i++)
                    TransMess[len++]=buff[i];
            if(strcmp(TransMess,"yes")==0 || strcmp(TransMess,"no")==0)
            {
                    // printf("TransMess:%s\n",TransMess);
                    // printf("YESYEYSYESYEYS\n");
                    acFinish=true;
                    while (acFinish);
                    //  printf("NONONONONO\n");
                    memset(buff+name_len+1,0,strlen(buff)-name_len);
                    continue;
            }
        }
        if(del_message(connfd,buff+name_len+1))
        {
                printf("%s\n", buff);
	        	send_message(buff,n+name_len+1);
        }
         memset(buff+name_len+1,0,strlen(buff)-name_len);
    }
    delete [] buff;
}