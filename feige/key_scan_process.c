#include <stdio.h>
#include <unistd.h> 
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include "IPMSG.H"
#include "userlist.h"
#include "pack_unpack.h"
#include "filelist.h"
#include "public.h"
 
void help_cmd(void);
void ls_cmd(void);//查看在线用户
void sendto_cmd(char *input_name);//发送消息
void send_BR_EXIT(void);//退出
void ls_sendfile(void);
void ls_rcvfile(void);
void sendfile_cmd(char *input_name);
void getfile_cmd(char *input_name);

//扫描键盘
void *key_scan_process()
{
	char buf[BUFF_SIZE];
	int recvBytes;
	while(1)
	{
		printf("ipmsg> ");
		memset(&buf, 0, sizeof(buf)); 
		//读取键盘输入
		if ((recvBytes = read(1, buf, sizeof(buf))) != -1 ) 
		{
			buf[recvBytes-1] = '\0';//字符串结尾
			//printf("recv:%s,%d\n\n", buf, recvBytes);
			
			if (strncmp(buf, "sendto", 6) == 0)
			{
				sendto_cmd(&buf[7]);
			}
			else if (strcmp(buf, "ls") == 0)
			{
				ls_cmd();
			}
			else if (strcmp(buf, "help") == 0)
			{
				help_cmd();
			}
			else if (strncmp(buf, "getfile", 7) == 0)
			{
				getfile_cmd(&buf[8]);
			}
			else if (strncmp(buf, "sendfile", 8) == 0)
			{
				sendfile_cmd(&buf[9]);
			}
			else if (strcmp(buf, "SFL") == 0)
			{
				ls_sendfile ();
			}
			else if (strcmp(buf, "RFL") == 0)
			{
				ls_rcvfile ();
			}
			else if (strcmp(buf, "exit") == 0)    
			{
				send_BR_EXIT();
				break;
			}		
		}	
	}
	pthread_exit(NULL);
}

void help_cmd(void)
{
	printf("\n*******************************************************\n");
	printf("\t显示好友列表:\t\tls\n");
	printf("\t给好友发送消息:\t\tsendto[空格]好友名\n");
	printf("\t显示发送文件列表:\tSFL\n");
	printf("\t显示接收文件列表:\tRFL\n");
	printf("\t发送文件:\t\tsendfile[空格]好友名\n");
	printf("\t接收文件:\t\tgetfile[空格]文件名\n\t\t(当对方发给你程序时)\n");
	printf("\t退出程序:\t\texit\n");
	printf("*********************************************************\n\n");
}


void ls_cmd(void) //遍历
{
	struct user* cur = userlist_head->next;
	printf("\n***************************************************\n");
	printf("****\t好友列表:                                          ****\n");
	while (cur != NULL)
	{
		printf("****\tname=%s", cur->name);
		printf("\tip=%s \t\t           ****\n", inet_ntoa(cur->sin_addr));
		cur = cur->next;
	}
	printf("******************************************************\n");
	
	return;
}

//发送消息
void sendto_cmd(char *input_name)
{
	struct user *cur_user = getUser_name(input_name);
	
	if (cur_user == NULL)
	{
		printf("该用户不存在!\n");
		return;
	}
	
	char buff[BUFF_SIZE], codingbuff[BUFF_SIZE];
	
	int  set = 1, sendBytes, recvBytes;
	struct sockaddr_in send_sock_addr;
	//setsockopt(udp_sock, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &set, sizeof(int)); 
	memset(&send_sock_addr, 0, sizeof(struct sockaddr_in));
	send_sock_addr.sin_family = AF_INET;  
    send_sock_addr.sin_port = htons(MSG_PORT);  
	send_sock_addr.sin_addr = cur_user->sin_addr;
	
	while(1)
	{
		printf("[to %s]:", cur_user->name );
		memset(&buff, 0, sizeof(buff)); 
		
		//接收到键盘信息
		
		if ((recvBytes = read(0, buff, sizeof(buff)) ) != -1 )
		{
			buff[recvBytes-1] = '\0';
			if ( strcmp(buff, "exit") != 0)
			{
				if (recvBytes <= 1) continue;
				//打包
				coding(codingbuff, IPMSG_SENDMSG, buff);
				if((sendBytes = sendto(udp_sock, codingbuff, strlen(codingbuff), 0,  
						(struct sockaddr *)&send_sock_addr, sizeof(struct sockaddr))) == -1)
				{  
					printf("sendto fail\n");  
				}
			}
			else
			{
				return;
			}
		}
	}	
}

//退出程序
void send_BR_EXIT(void)
{
	struct sockaddr_in exit_send_addr;
	int sendBytes;
	char buff[BUFF_SIZE];
	memset(&buff, 0, sizeof(buff)); 
	//设置套接字类型
	int set = 1;
	setsockopt(udp_sock,SOL_SOCKET,SO_BROADCAST,&set,sizeof(set));	
	memset(&exit_send_addr, 0, sizeof(exit_send_addr)); 
	exit_send_addr.sin_family=AF_INET;
	exit_send_addr.sin_port=htons(MSG_PORT);
	inet_pton(AF_INET,"192.168.1.255",&exit_send_addr.sin_addr);
	int len=sizeof(exit_send_addr);
	//发送IPMSG_ANSENTRY信息
	coding(buff,IPMSG_BR_EXIT,NULL);
	//printf("%s\n",buff);
	if((sendBytes = sendto(udp_sock,buff,strlen(buff),0,(struct sockaddr *)&exit_send_addr,len))==-1)
	{  
		printf("sendto fail\n");  
	}
	exit(-1);
}

//查看发送文件列表
void ls_sendfile(void)
{
	struct sendfile* file = send_filelist_head->next;
	printf("\n***************************************************\n");
	printf("****\t发送文件列表:                                          \n");
	while (file != NULL)
	{
		printf("****\tname=%s", file->name);
		printf("\tip=%s \t\t           \n", inet_ntoa(file->sin_addr));
		file = file->next;
	}
	printf("******************************************************\n");
	return;
}

//查看接收文件列表
void ls_rcvfile(void)
{
	struct rcvfile* file = rcv_filelist_head->next;
	printf("\n***************************************************\n");
	printf("****\t接收文件列表:                                          \n");
	while (file != NULL)
	{
		printf("****\tname=%s", file->name);
		printf("\tip=%s \t\t           \n", inet_ntoa(file->sin_addr));
		file = file->next;
	}
	printf("******************************************************\n");
	return;
}

//接收文件
void getfile_cmd(char *input_name)
{
	printf("开始接收文件\n");
	char cmd_buf[BUFFER_SIZE];
	int con_sockt;
	struct rcvfile *file = get_rcvFile_name(input_name);
	int ren=0;
	int len=0;
	if(file == NULL)
	{
		printf("该文件不存在!\n");
		return;	
	}
	FILE* fd;
	if((fd=fopen(input_name,"rb+"))==NULL)
	{
		int sockfd;
		int ren;
		struct sockaddr_in myaddr;
		//连接
		myaddr.sin_family=AF_INET;
		myaddr.sin_port=htons(MSG_PORT);
		//要通过文件列表获取文件名和文件ip
		char *server_ip=inet_ntoa(file->sin_addr);
		inet_pton(AF_INET,server_ip,&myaddr.sin_addr);
		//创建套接字
		sockfd=socket(AF_INET,SOCK_STREAM,0);
		if(sockfd<0)
		{
			perror("socket");
			exit(-1);
		}
		char sendbuf[BUFFER_SIZE];
		printf("开始连接\n");
		ren=connect(sockfd,(struct sockaddr *)&myaddr,sizeof(myaddr));
		if(ren!=0)
		{
			perror("connect");
		}	
       
		bzero(cmd_buf,BUFFER_SIZE);  //把缓冲区清0
		char codingbuff[BUFF_SIZE];
		//打包发送消息
		int sendBytes;
		sprintf(cmd_buf,"%lx:%lx:%d",file->pkgnum,file->num,0);
		//printf("打包1：%s\n",cmd_buf);
		coding(codingbuff,IPMSG_GETFILEDATA,cmd_buf);
		//printf("打包2：%s\n",codingbuff);
		if((sendBytes = send(sockfd, codingbuff,strlen(codingbuff),0)) == -1)
		{  
			printf("send fail\n");
			return;
		}
		if((fd=fopen(input_name,"wb+"))==NULL)
		{
			perror("open");	
			exit(-1);
		}
		else
		{
			char sendbuf[BUFFER_SIZE];
			int size=0;
			int  recv_len = 0;
			do   
			{   
				bzero(sendbuf,sizeof(sendbuf));   
				recv_len = recv(sockfd,sendbuf,sizeof(sendbuf),0) ;   
				size = size +recv_len;   
				fwrite(sendbuf,sizeof(char),recv_len,fd);  
				printf("***size = %d***file->size=%ld***\n",size,file->size);   

			}while(size<(file->size));   
			if(size == file->size)
			{
				printf("successfully receive file! \n");
			}
			else
			{
				printf("cannot receive file !!! \n");
			}
			del_rcvFile_name(file->name);
			fclose(fd);
			close(sockfd);
			return;
		}
		//处理代码
	}
}

//发送文件
void sendfile_cmd(char *input_name)
{
	struct user *cur_user = getUser_name(input_name);
	
	if (cur_user == NULL)
	{
		printf("该用户不存在!\n");
		return;
	}
	int ren;
	struct sockaddr_in myaddr;
	char *server_ip=inet_ntoa(cur_user->sin_addr);
	char buff[BUFF_SIZE], codingbuff[BUFF_SIZE];
	int sendBytes, recvBytes;
	inet_pton(AF_INET,server_ip,&myaddr.sin_addr);
	printf("[to %s]:", cur_user->name);
	memset(&buff, 0, sizeof(buff)); 
	
	//接收到键盘信息
	if ((recvBytes = read(0, buff, sizeof(buff)) ) != -1 )
	{
		buff[recvBytes-1] = '\0';
		FILE *fd;
		//打开二进制文件
		fd=fopen(buff,"rb");
		 
		if(fd == NULL)
		{
			printf("File:%s not found in current path\n",buff);
			return;
		}
		//添加到发送文件列表
		add_sendFile(&myaddr.sin_addr,buff);
		struct sendfile* sendfiles;
		sendfiles=get_sendFile_name(buff);
		 
		if ( strcmp(buff, "exit") != 0)
		{
			memset(&udp_sock_addr, 0, sizeof(struct sockaddr_in));
			udp_sock_addr.sin_family = AF_INET;  
			udp_sock_addr.sin_port = htons(MSG_PORT); 
			udp_sock_addr.sin_addr = cur_user->sin_addr;
			char cmd_tmp[BUFF_SIZE];
			char num[BUFF_SIZE];
			sprintf(cmd_tmp,"%d:%s:%lx:%lx:%lx",sendfiles->num,buff,sendfiles->size,sendfiles->ltime,IPMSG_FILE_REGULAR);
			coding(codingbuff,IPMSG_SENDMSG|IPMSG_SENDCHECKOPT|IPMSG_FILEATTACHOPT,NULL);
			sprintf(num,"%s%c%s",codingbuff,'0',cmd_tmp);
			if((sendBytes = sendto(udp_sock, num, strlen(num), 0,  
					(struct sockaddr *)&udp_sock_addr, sizeof(udp_sock_addr))) == -1)
			{  
				printf("sendto fail\n");
				return;
			}
			fclose(fd);
			return;
		}
		else
		{
			del_sendFile_name(buff);
			exit(-1);
			return;
		}
	}	
}

