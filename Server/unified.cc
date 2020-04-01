#include "unified.h"


std::unordered_map<std::string,int>client_map;


void Error(const char* message)
{
    printf(message,strerror(errno),errno);
    exit(0);
}

void send_message(const char* buff,int n)
{
	//sem_wait(&sem);
		for(auto it:client_map)
			 if( send(it.second, buff, n, 0) < 0)       //发送回客户端
				Error("send msg error: %s(errno: %d)\n");
	 //sem_post(&sem);
}
void Connect(int connfd,sockaddr_in cliaddr)
{
    char* buff=new char[MAXLINE];
    //获得用户名
    int name_len = recv(connfd, buff, MAXLINE, 0);
    std::string name(buff,name_len);
	
	client_map[name]=connfd;
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
        printf("%s\n", buff);
         // if( send(connfd, buff, n, 0) < 0)       //发送回客户端
             // Error("send msg error: %s(errno: %d)\n");
		send_message(buff,n+name_len+1);
         memset(buff+name_len+1,0,strlen(buff)-name_len);
    }
    delete [] buff;
}