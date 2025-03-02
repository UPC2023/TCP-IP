#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "pack_unpack.h" 
#include "IPMSG.H"
#include "public.h"

void broadcast(void)
{
	struct sockaddr_in broad_cast_addr;
	int sendBytes;
	char msg[1024];

	//设置套接字类型
	int set = 1;
	setsockopt(udp_sock,SOL_SOCKET,SO_BROADCAST,&set,sizeof(set));	
	memset(&broad_cast_addr, 0, sizeof(broad_cast_addr)); 
	broad_cast_addr.sin_family=AF_INET;
	broad_cast_addr.sin_port=htons(MSG_PORT);
	inet_pton(AF_INET,"192.168.253.255",&broad_cast_addr.sin_addr);// IP address varies from person to person
	int len=sizeof(broad_cast_addr);
    //用户上线，打包，广播
	coding(msg,IPMSG_BR_ENTRY,myname);
	//printf("%s\n",msg);
	//lis_sock中包含本机IP，通过broad_cast_addr确定发送的方式
	if((sendBytes = sendto(udp_sock, msg, strlen(msg), 0,(struct sockaddr *)&broad_cast_addr,len)) == -1)
	{  
		printf("broadcast fail\n");
		exit(-1);
	}
}