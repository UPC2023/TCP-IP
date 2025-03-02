#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h> 
#include <string.h>
#include "create_socket.h"
#include "userlist.h"
#include "broadcast.h"
#include "filelist.h" 

char myname[20];
int udp_sock;
int tcp_sock;
struct sockaddr_in  udp_sock_addr;
struct sockaddr_in  tcp_sock_addr;

struct rcvfile *rcv_filelist_head = NULL;
struct sendfile *send_filelist_head = NULL;
struct user *userlist_head = NULL;

void *udp_msg_process();
void *key_scan_process();
void *tcp_send_file();

int main()
{
	pthread_t udp_rcv,key_scan,tcp_send;
	int ret=0;
	
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
	
	puts("请输入昵称:");
	fgets(myname, 20, stdin);//接收回车
	myname[strlen(myname)-1]='\0';
	
	init_userlist();
	init_rcv_filelist();
	init_send_filelist();
	
	creat_socket();//创建网络服务
	broadcast();//广播上线

	ret=pthread_create(&udp_rcv,0,udp_msg_process,NULL);
	if(ret !=0)
	{
		perror("pthread_create");
		exit(-1);
	}
	ret=pthread_create(&key_scan,0,key_scan_process,NULL);
	if(ret !=0)
	{
		perror("pthread_create");
		exit(-1);
	}
	ret=pthread_create(&tcp_send,0,tcp_send_file,NULL);
	if(ret !=0)
	{
		perror("pthread_create");
		exit(-1);
	}

	pthread_join(udp_rcv,NULL);
	pthread_join(key_scan,NULL);
	pthread_join(tcp_send,NULL);
	return 0;
}
