#include <iostream>
#include <semaphore.h>
#include "unified.h"

std::unordered_map<std::string,int>client_map;

static sem_t get_sem;
static sem_t send_sem; 

static bool sendFile=false;
static bool getFile=false;
static bool ac=false;
static bool isDel=false;


static FILE* sendfp=nullptr;
static FILE* getfp=nullptr;
static char sendFileName[FILE_NAME_MAX_SIZE+1];
static char getFileName[FILE_NAME_MAX_SIZE+1];

void Error(const char* message)
{
    printf(message,strerror(errno),errno);
}
static void print_progress(int progress)	//打印进度
{
	switch (progress)
	{
	case 50:
		printf("正在传输:>         \r"); fflush(stdout);
		break;
	case 100:
		printf("正在传输:>>        \r"); fflush(stdout);
		break;
	case 150:
		printf("正在传输:>>>       \r"); fflush(stdout);
		break;
	case 200:
		printf("正在传输:>>>>      \r"); fflush(stdout);
		break;
	case 250:
		printf("正在传输:>>>>>     \r"); fflush(stdout);
		break;
	case 300:
		printf("正在传输:>>>>>>    \r"); fflush(stdout);
		break;
	case 350:
		printf("正在传输:>>>>>>>   \r"); fflush(stdout);
		break;
	case 400:
		printf("正在传输:>>>>>>>>  \r"); fflush(stdout);
		break;
	case 450:
		printf("正在传输:>>>>>>>>> \r"); fflush(stdout);
		break;
	case 500:
		printf("正在传输:>>>>>>>>>>\r"); fflush(stdout);
		break; 

	default:
		break;
	}
}

static int SCP(char* mess,char* username)	//确认发送文件
{
	if(mess[0]!='s' || mess[1]!='c' || mess[2]!='p') return 0;

	//char file_name[FILE_NAME_MAX_SIZE+1];
    bzero(sendFileName, FILE_NAME_MAX_SIZE+1);

	int length=0;
	int i;
	for(i=4;mess[i]!='@' && mess[i]!='\0';i++)
		sendFileName[length++]=mess[i];

	//指令格式错误
	if(mess[i]!='@')
	{
			Error("\033[31mSystem message:scp指令格式有误\033[0m\n");
			return 1;
	}

	//完整路径
	sendFileName[--length]='\0';

	//获得用户名
	bzero(username,MAXLINE);
	int len=0;
	for(i+=1;mess[i]!='\0';i++)
		username[len++]=mess[i];
	username[len]='\0';


	sendfp=fopen(sendFileName,"r");
	//文件打开错误
	 if(!sendfp)
   {
       Error("\033[31mSystem message:File Not Found\033[0m\n"); 
	   return 1;
   }
	//成功返回文件指针
	return 2;
}

static int acSCP(int sockfd,char* mess)		//确认接收文件
{
		if(mess[0]!='s' || mess[1]!='c' || mess[2]!='p') return 0;
		getFile=true;
		

		char name[FILE_NAME_MAX_SIZE+1];
		printf("mess:%s\n",mess);
		int i,len=0;
		for(i=3;mess[i]!='\0';i++)
				name[len++]=mess[i];
		name[i]='\0';
		printf("name:%s\n",name);
		printf("是否接受<%s>发送过来的文件(y/n)\n",name);
			
		while(!isDel);   //等待处理完毕


	//bzero(getFileName,FILE_NAME_MAX_SIZE+1);

	if(ac)
	{
		do
		{
		bzero(getFileName,FILE_NAME_MAX_SIZE+1);
		printf("\n请输入文件保存路径(文件名不能遗漏):");
		scanf("%s",getFileName);
		//if(getFileName)
		getfp=fopen(getFileName,"w");
		if(!getfp)
			Error("Can Not Open The File To Write\n");
			// ac=false;
			// isDel=false;
			// getFile=false;
		}while(!getfp);
		return 2;
	}
		
	else 
	{
		send(sockfd, "no", 3, 0);
		ac=false;
		isDel=false;
		getFile=false;
		return 1;
	}


	return 2;	
}

static void send_File(int sockfd,char* username)	//发送文件
{

	 int length=0;	//传输长度
	 //传输缓存
	char buffer[MAXLINE]={'s','c','p','\0'};
	strcpy(buffer+3,username);
	//向server请求传输文件
	if( send(sockfd,buffer, strlen(buffer), 0) < 0)
            Error("send msg error: %s(errno: %d)\n");
	
	sendFile=true;    //正在请求发送文件
	//等待server回复	
	printf("\033[33mSystem message:正在等待<%s>确认,请勿操作...\033[0m\n",username);	
	while(!isDel);
	
	bzero(buffer,MAXLINE);
	int progress=1;
    while(ac && (length=fread(buffer,sizeof(char),MAXLINE,sendfp))>0)
     {
          if( send(sockfd, buffer, length, 0) < 0)
          {
              printf("Send File Failed.\n");
               break;
          }
          bzero(buffer,MAXLINE);
		  print_progress(progress);
		  progress+=1;
		  if(progress>500) progress=1;
		  //printf(">>>>>>>>>>>>>正在传输>>>>>>>>>\n");
     }
	
	 if(ac){send(sockfd, "@$@", 4, 0); printf("\n\033[34mSystem message:Send File Successful! \033[0m\n"); }

	 fclose(sendfp);
	 sendfp=nullptr;
	 bzero(sendFileName,FILE_NAME_MAX_SIZE+1);
	 ac=false;
	 isDel=false;
	 sendFile=false;	

}


static void get_File(int sockfd)		//接收文件
{
		 int length = 0;
		 char buffer[MAXLINE];
		 bzero(buffer, MAXLINE);
		 if(ac) send(sockfd, "yes", 4, 0);
		 int progress=1;
        while(ac && (length = recv(sockfd, buffer, MAXLINE, 0)) > 0)
        {
			//printf("%s\n",buffer);
			if(strcmp(buffer+length-4,"@$@")==0) 
			{
					buffer[length-4]='\0';
					fwrite(buffer, sizeof(char), length-4, getfp);
					break;
			}
				
                if(fwrite(buffer, sizeof(char), length, getfp) < length)
             {
                 printf("File Write Failed\n");
                 break;
             }
             bzero(buffer, MAXLINE);

			 print_progress(progress);
		  	 progress+=1;
		  	 if(progress>500) progress=1;
			 //printf(">>>>>>>>>>>>>正在传输>>>>>>>>>\n");
        }
		if(ac) printf("\n\033[34mSystem message:Receive File Successful!\33[0m\n");
		ac=false;
		isDel=false;
		getFile=false;
		fclose(getfp);
		getfp=nullptr;
		bzero(getFileName,FILE_NAME_MAX_SIZE+1);
}

static bool del_recvmess(char* mess)	//处理特殊信息
{
	//printf("mess:%s\n",mess);
		if(strcmp(mess,"no")==0)		//客户端拒收
	{
		Error("\033[31mSystem message:对方拒绝接收该文件\033[0m\n");
		ac=false;
		isDel=true;
		sendFile=false;
		return false;
	}
	if(strcmp(mess,"error")==0)		//用户不存在
	{
		ac=false;
		isDel=true;
		sendFile=false;
		Error("\033[31mSystem message:用户不存在或者已下线\033[0m\n");
		return false;
	}
	if(strcmp(mess,"yes")==0)
	{
		ac=true;
		isDel=true;
		return true;
	}
	return true;
}

void send_message(int sockfd)		//发送数据
{
	sem_init(&send_sem,0,1);
	
	char* send_mess=new char[MAXLINE];
	char* username=new char[MAXLINE];

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
				if(getFile)		//询问是否接受对方传来的文件
				{
					if(send_mess[0]=='y') { ac=true; isDel=true;while(getFile);}
					else if(send_mess[0]=='n') {ac=false; isDel=true; while(getFile);}
					else {printf("\n输入有误，请输入'y'或者'n'");}
					i--;
					continue;
				}
				sem_wait(&get_sem);
			}
		}
		
		//从第4个字符开始
        fgets(send_mess+3, MAXLINE-3, stdin);
		
        send_mess[strlen(send_mess)-1]='\0';
		// printf("mess:%s\n",send_mess);
		if(strcmp(send_mess,"exit")==0) 
		{
			shutdown(sockfd,SHUT_RDWR); //关闭 套接字 的输入输出端口
			break;}

		int scp;
		if((scp=SCP(send_mess,username))==2)
				send_File(sockfd,username);
			
				
		// printf("scp%d\n",scp);
        if(!scp && send(sockfd, send_mess, strlen(send_mess), 0) < 0)
            Error("send msg error: %s(errno: %d)\n");

		sem_post(&get_sem);
         memset(send_mess,0,strlen(send_mess));
    }
	
	delete [] send_mess;
}

void get_message(int sockfd)	//接收数据
{
	char* recvbuf=new char[MAXLINE];
	sem_init(&get_sem,0,1);

	while(1)
	{
		int n = recv(sockfd, recvbuf,MAXLINE, 0);
		if(n==0) break;
        recvbuf[n]='\0';
		
		if(sendFile)
		{		//printf("YESORNO:%s",recvbuf);
				del_recvmess(recvbuf);
				bzero(recvbuf,MAXLINE);
				continue;
		}
				
		
		//对方请求文件传输
		int acscp;
		if((acscp=acSCP(sockfd,recvbuf))==2)
				get_File(sockfd);
				
		else if(acscp==0)	//非scp请求
		{
			sem_wait(&get_sem);
        	printf("\033[32m%s\033[0m\n", recvbuf);
		 	sem_post(&get_sem);
		}

	}

}



int scanKeyboard()	//立即获得输入的字符
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

