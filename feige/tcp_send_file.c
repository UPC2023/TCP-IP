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

//发送文件
void *tcp_send_file()
{
	int ret=0;
	struct cmd cmd_obj;
	char rcv[1024];
	int con_sockt;//连接套接字
	FILE *fd;
	int recvbytes;
	int len;
	struct sendfile sendfiles;
	struct sendfile *send_files;
	char recvbuf[BUFF_SIZE];

	len=sizeof(tcp_sock_addr);
	ret=listen(tcp_sock,30);
	if(ret!=0)
	{
		perror("listen");
		exit(-1);
	}
	while(1)
	{
		//printf("等待TCP连接\n");
		con_sockt=accept(tcp_sock,(struct sockaddr *)&tcp_sock_addr,&len);
		printf("TCP连接成功\n");
		recvbytes=recv(con_sockt,recvbuf,sizeof(recvbuf),0);
		recvbuf[recvbytes] = '\0';
		//printf("接收到的****包：%s",recvbuf);
		memset(&cmd_obj, 0, sizeof(struct cmd));
		sscanf(recvbuf, "%*[^:]:%[^:]:%[^:]:%[^:]:%d:%[^\n]",rcv, cmd_obj.name, cmd_obj.hostname, &cmd_obj.cmdid, cmd_obj.buf);
		//printf("解析的包为：%s,,,%d",cmd_obj.hostname,cmd_obj.cmdid);
	    if(GET_MODE(cmd_obj.cmdid) == IPMSG_GETFILEDATA)
		{
			sscanf(cmd_obj.buf,"%lx:%d",&sendfiles.pkgnum,&sendfiles.num);
			send_files=get_sendFile_num(sendfiles.num);
			fd=fopen(send_files->name,"rb"); 
			if(fd == NULL)
			{
				printf("File:%s not found in current path\n",sendfiles.name);
				exit(-1);
			}
			char sendbuf[BUFFER_SIZE];
			bzero(sendbuf,BUFFER_SIZE);  //把缓冲区清0

			int file_block_length=0;
			//每次读1024个字节，下一次读取时内部指针自动偏移到上一次读取到位置
			while((file_block_length =fread(sendbuf,sizeof(char),BUFFER_SIZE,fd))>0)  
			{  
				printf("%d\n",file_block_length);
				if(write(con_sockt,sendbuf,file_block_length)<0)  
				{  
					perror("send");  
					exit(1);  
				}  
				bzero(sendbuf,BUFFER_SIZE);//发送一次数据之后把缓冲区清零
			}  
			del_sendFile_name(send_files->name);
			fclose(fd);  
			printf("Transfer file finished !\n");
			close(con_sockt);
			return;
		}
	}
}
