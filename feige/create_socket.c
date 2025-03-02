#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "public.h"

void creat_socket(void)
{
	if ((udp_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1 ) 
	{
		printf("create socket error\n");
		exit(-1);
	}
	if ((tcp_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1 ) 
	{
		printf("create socket error\n");
		exit(-1);
	}
	int set = 1;
	setsockopt(udp_sock,SOL_SOCKET,SO_REUSEADDR,&set,sizeof(int));

    memset(&udp_sock_addr, 0, sizeof(struct sockaddr_in));
	udp_sock_addr.sin_family = AF_INET;  
    udp_sock_addr.sin_port = htons(MSG_PORT);  
    udp_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   
    memset(&tcp_sock_addr, 0, sizeof(struct sockaddr_in));
	tcp_sock_addr.sin_family = AF_INET;  
    tcp_sock_addr.sin_port = htons(MSG_PORT);  
	//不指定客户端的IP，随便连。
    tcp_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//绑定目的是限制了服务端进程创建的socket只接受那些目的地为此IP地址的客户链接
	if(bind(udp_sock, (struct sockaddr *)&udp_sock_addr, sizeof(udp_sock_addr)) == -1)
	{  
        printf("bind fail1\n");  
        exit(-1);
    }
	if(bind(tcp_sock, (struct sockaddr *)&tcp_sock_addr, sizeof(tcp_sock_addr)) == -1)
	{  
        printf("bind fail\n");  
        exit(-1);
    }
}


	
	