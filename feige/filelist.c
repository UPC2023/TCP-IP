#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include "filelist.h"
#include "public.h"

//初始化发送文件链表
int init_send_filelist()
{
	send_filelist_head = (struct sendfile*)malloc(sizeof(struct sendfile));
	if (send_filelist_head == NULL)
	{
		perror("申请空间失败！");
		return 1;
	}
	memset(send_filelist_head, 0 , sizeof(struct sendfile));
	send_filelist_head->next = NULL;
	return 0;
}


//添加文件
int add_sendFile(struct in_addr* sin_addr, char name[])
{
	struct stat buf;
	struct sendfile* newFile = NULL;
	struct sendfile* rear = send_filelist_head;
	static unsigned int file_num=0;
	
	stat(name,&buf);
	
	if ( (newFile = (struct sendfile*)malloc(sizeof(struct sendfile))) == NULL )
	{
		printf("newFile failed!\n");
		return 1;
	}
	
	newFile->sin_addr.s_addr = sin_addr->s_addr;
	strncpy(newFile->name, name, 20);
	newFile->num=file_num;
	newFile->pkgnum=time(NULL);
	newFile->size=buf.st_size;
	newFile->ltime=buf.st_mtime;
	while (rear->next != NULL)
	{
		rear = rear->next;
	}

	rear->next = newFile;
	newFile->next = NULL;

	return 0;
}



//根据sin_addr 删除文件  正确删除返回0 错误返回1
int del_sendFile(struct in_addr sin_addr)//删除
{
	struct sendfile* pre, *suc;
	pre = send_filelist_head;
	suc = send_filelist_head->next;
	while (suc->sin_addr.s_addr != sin_addr.s_addr)
	{
		pre = suc;
		suc = suc->next;
	}
	pre->next = suc->next;
	free(suc);
	//printf("删除成功！\n");

	return 0;
}


//根据文件名删除文件  正确删除返回0 错误返回1
int del_sendFile_name(char nam[])//删除
{
	struct sendfile* pre, *suc;
	pre = send_filelist_head;
	suc = send_filelist_head->next;
	while (strcmp(suc->name,nam)!=0)
	{
		pre = suc;
		suc = suc->next;
	}
	pre->next = suc->next;
	free(suc);
	//printf("删除成功！\n");

	return 0;
}

//根据sin_addr 获取file结构体 没有返回NULL
struct sendfile* get_sendFile(struct in_addr *sin_addr)//根据ip查找文件信息
{
	struct sendfile* cur=send_filelist_head;
	while ((cur->next != NULL)  && (cur->sin_addr.s_addr != sin_addr->s_addr))
	{
		cur = cur->next;
	}
	if (cur->sin_addr.s_addr == sin_addr->s_addr)
		return cur;
	else
		return NULL;

}


//根据文件名获取file结构体

struct sendfile* get_sendFile_name(char name[])
{
	struct sendfile* cur;
	cur = send_filelist_head;
	while ((cur->next != NULL)  && strcmp(name, cur->next->name) != 0)
	{
		cur = cur->next;
	}
	if (cur->next != NULL && strcmp(name, cur->next->name) == 0)
		return cur->next;
	else
		return NULL;

}
//根据文件编号获得file结构体
struct sendfile* get_sendFile_num(int num)
{
	struct sendfile* cur;
	cur = send_filelist_head;
	while ((cur->next != NULL)  && (num!=cur->next->num))
	{
		cur = cur->next;
	}
	if (cur->next != NULL && (num == cur->next->num))
		return cur->next;
	else
		return NULL;

}



//根据ip 得到filename
char* get_sendFileName(struct in_addr* sin_addr)//根据ip查找filename
{
	int none = 0;
	
	struct sendfile * cur;
	cur = send_filelist_head->next;
	while (cur->sin_addr.s_addr != sin_addr->s_addr)
	{
		cur = cur->next;
		if (cur->sin_addr.s_addr == sin_addr->s_addr)
			break;

		if (cur->next == NULL && cur->sin_addr.s_addr != sin_addr->s_addr)
		{
			none = 1;
			break;
		}
		else
		{
			return NULL;
		}
	}
}

//初始化发送文件链表
int init_rcv_filelist(void)
{
	rcv_filelist_head = (struct rcvfile*)malloc(sizeof(struct rcvfile));
	if (rcv_filelist_head == NULL)
	{
		perror("申请空间失败！");
		return 1;
	}
	memset(rcv_filelist_head, 0 , sizeof(struct rcvfile));
	rcv_filelist_head->next = NULL;
	return 0;
}


//添加文件
int add_rcvFile(struct in_addr* sin_addr, char name[],long  num,long size)
{
	struct rcvfile* newFile = NULL;
	struct rcvfile* rear = rcv_filelist_head;
	if ( (newFile = (struct rcvfile*)malloc(sizeof(struct rcvfile))) == NULL )
	{
		printf("newFile failed!\n");
		return 1;
	}
	
	newFile->sin_addr.s_addr = sin_addr->s_addr;
	strncpy(newFile->name, name, 20);
	newFile->num=num;
	newFile->pkgnum=time(NULL);
	newFile->size=size;
	while (rear->next != NULL)
	{
		rear = rear->next;
	}

	rear->next = newFile;
	newFile->next = NULL;

	return 0;
}



//根据sin_addr 删除文件  正确删除返回0 错误返回1
int del_rcvFile(struct in_addr sin_addr)//删除
{
	struct rcvfile* pre, *suc;
	pre = rcv_filelist_head;
	suc = rcv_filelist_head->next;
	while (suc->sin_addr.s_addr != sin_addr.s_addr)
	{
		pre = suc;
		suc = suc->next;
	}
	pre->next = suc->next;
	free(suc);
	//printf("删除成功！\n");

	return 0;
}

//根据文件名删除文件  正确删除返回0 错误返回1
int del_rcvFile_name(char nam[])//删除
{
	struct rcvfile* pre, *suc;
	pre = rcv_filelist_head;
	suc = rcv_filelist_head->next;
	while (strcmp(suc->name,nam)!=0)
	{
		pre = suc;
		suc = suc->next;
	}
	pre->next = suc->next;
	free(suc);
	//printf("删除成功！\n");

	return 0;
}

//根据sin_addr 获取file结构体 没有返回NULL
struct rcvfile* get_rcvFile(struct in_addr *sin_addr)//根据ip查找文件信息
{
	struct rcvfile* cur=rcv_filelist_head;
	while ((cur->next != NULL)  && (cur->sin_addr.s_addr != sin_addr->s_addr))
	{
		cur = cur->next;
	}
	if (cur->sin_addr.s_addr == sin_addr->s_addr)
		return cur;
	else
		return NULL;

}


//根据文件名获取file结构体

struct rcvfile* get_rcvFile_name(char nam[])
{
	struct rcvfile* cur;
	cur = rcv_filelist_head;
	while ((cur->next != NULL)  && strcmp(nam, cur->next->name) != 0)
	{
		cur = cur->next;
	}
	if (cur->next != NULL && strcmp(nam, cur->next->name) == 0)
		return cur->next;
	else
		return NULL;

}


//根据ip 得到filename
char* get_rcvFileName(struct in_addr* sin_addr)//根据ip查找filename
{
	int none = 0;
	struct rcvfile * cur;
	cur = rcv_filelist_head->next;
	while (cur->sin_addr.s_addr != sin_addr->s_addr)
	{
		cur = cur->next;
		if (cur->sin_addr.s_addr == sin_addr->s_addr)
			break;

		if (cur->next == NULL && cur->sin_addr.s_addr != sin_addr->s_addr)
		{
			none = 1;
			break;
		}
		else
		{
			return NULL;
		}
	}
}


	
	



