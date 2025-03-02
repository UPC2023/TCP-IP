#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "IPMSG.H"
#include "pack_unpack.h"
#include "userlist.h"
#include "filelist.h"
#include "public.h"

struct cmd cmd_obj;
char recvbuf[BUFF_SIZE];

void udp_msg_handle(struct cmd *msg ,struct sockaddr_in* send_addr)
{
	unsigned long tmp=0;
	//接受到用户广播上线信息
	if ((GET_MODE(msg->cmdid ) == IPMSG_BR_ENTRY))
	{	
		//没有此用户则添加此用户
		//根据sin_addr 判断
		if ( getUser(&send_addr->sin_addr) == NULL ) 
		{
			addUser(&send_addr->sin_addr, msg->name, msg->hostname);
		}
		char buff[BUFF_SIZE];
		memset(buff, 0, sizeof(buff)); 
		//发送IPMSG_ANSENTRY信息
		coding(buff,IPMSG_ANSENTRY,myname);
		int sendBytes;
		if((sendBytes = sendto(udp_sock, buff, strlen(buff), 0,  
			(struct sockaddr *)send_addr, sizeof(struct sockaddr))) == -1)
		{  
			printf("sendto fail\n");  
		}
	}
	//接收到应答上线信息
	if (GET_MODE(msg->cmdid )  == IPMSG_ANSENTRY)
	{
		//没有此用户则添加此用户
		//根据sin_addr 判断
		if ( getUser(&send_addr->sin_addr) == NULL ) 
		{
			addUser(&send_addr->sin_addr, msg->buf, msg->hostname);
		}
	}
	//接收到用户下线信息
	if (GET_MODE(msg->cmdid ) == IPMSG_BR_EXIT)
	{
		//有此用户则删除此用户
		struct user* logout_user;
		if ( (logout_user = getUser(&send_addr->sin_addr)) != NULL ) 
		{
			//根据sin_addr 删除用户
			delUser(send_addr->sin_addr);
		}
	}
	//接收到消息
	if (GET_MODE(msg->cmdid )  == IPMSG_SENDMSG)
	{
		char codingbuff[BUFF_SIZE];
		if((msg->cmdid & IPMSG_SENDCHECKOPT)==IPMSG_SENDCHECKOPT)
		{
		coding(codingbuff,IPMSG_RECVMSG,msg->id);
	    sendto(udp_sock, codingbuff, strlen(codingbuff), 0,  
		(struct sockaddr *)&udp_sock_addr, sizeof(udp_sock_addr));
		}
		printf("[recv msg from: %s :%s]#",msg->name,inet_ntoa(send_addr->sin_addr));
		printf("%s\n",msg->buf);
	}

	//接收到文件
	if (( msg->cmdid & IPMSG_FILEATTACHOPT) == IPMSG_FILEATTACHOPT )
	{
		char codingbuff[BUFF_SIZE];
		coding(codingbuff,IPMSG_RECVMSG,msg->id);
		sendto(udp_sock, codingbuff, strlen(codingbuff), 0,  
				(struct sockaddr *)&udp_sock_addr, sizeof(udp_sock_addr));
		struct rcvfile rcvfiles;
		memset(&rcvfiles,0,sizeof(rcvfiles));
		rcvfiles.sin_addr=udp_sock_addr.sin_addr;
		char *p1,*p2,i,*pp;
		p1=strrchr(recvbuf,0);
		printf("接收到包含文件信息的UDP数据包:%s \n", recvbuf);
		//printf("接收到的数据包解析：%s\n",p1);
		p2=(p1+1);
		//printf("接收到的数据包再解析：%s\n",p2);
		sscanf(p2, "%lx:%[^:]:%lx", &rcvfiles.num, rcvfiles.name,&rcvfiles.size);
		pp = strtok(p2, ":");
		for(i=0;i<strlen(pp);i++)
		{
			tmp = tmp*10 + (*p2 - 0x30);
			p2++;
		}
		rcvfiles.num = tmp;
		printf("用户: %s向您发送文件：",inet_ntoa(udp_sock_addr.sin_addr));
		printf("%s\n",rcvfiles.name);
		add_rcvFile(&rcvfiles.sin_addr,rcvfiles.name,rcvfiles.num,rcvfiles.size);
	}
}

void *udp_msg_process()
{
    int addrLen = sizeof(udp_sock_addr); 
	int recvbytes;
	memset(&udp_sock_addr, 0, sizeof(struct sockaddr_in)); 

	while(1)
	{
	    //接收用户信息,接收到广播信息和广播机器的IP，
		//用来接收任何不管来源
		if((recvbytes = recvfrom(udp_sock, recvbuf, sizeof(recvbuf), 0, 
				(struct sockaddr *)&udp_sock_addr, &addrLen)) != -1)
		{  
			recvbuf[recvbytes] = '\0';
			//printf("接收到UDP数据包:%s 开始译码\n", recvbuf);

			memset(&cmd_obj, 0, sizeof(struct cmd));
			transcode(&cmd_obj, recvbuf, recvbytes);
			//printf("解析完命令开始执行\n");
			udp_msg_handle(&cmd_obj, &udp_sock_addr);	
			//printf("执行完成\n\n");
		}
		else
		{  
			printf("UDP接收失败\n");  
		}  
	}	
}
