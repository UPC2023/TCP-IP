#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "userlist.h" 
#include "public.h"

int init_userlist()//初始化
{
	userlist_head = (struct user*)malloc(sizeof(struct user));
	if (userlist_head == NULL)
	{
		perror("申请空间失败！");
		return 1;
	}
	memset(userlist_head, 0 , sizeof(struct user));
	userlist_head->next = NULL;
	return 0;
}


//添加用户
int addUser(struct in_addr* sin_addr, char name[], char hostname[])
{
	
	struct user* newUser = NULL;
	struct user* rear = userlist_head;
	
	if ( (newUser = (struct user*)malloc(sizeof(struct user))) == NULL )
	{
		printf("newUser failed!\n");
		return 1;
	}
	
	newUser->sin_addr.s_addr = sin_addr->s_addr;
	strncpy(newUser->name, name, 20);
	strncpy(newUser->host, hostname, 20);

	while (rear->next != NULL)
	{
		rear = rear->next;
	}

	rear->next = newUser;
	newUser->next = NULL;

	return 0;
}

//根据sin_addr 删除用户  正确删除返回0 错误返回1
int delUser(struct in_addr sin_addr)//删除
{
	struct user* pre, *suc;
	pre = userlist_head;
	suc = userlist_head->next;
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

//根据sin_addr 获取user结构体 没有返回NULL
struct user* getUser(struct in_addr* sin_addr)//根据ip查找用户信息
{
	struct user* cur;
	cur = userlist_head;
	while ((cur->next != NULL)  && (cur->sin_addr.s_addr != sin_addr->s_addr))
	{
		cur = cur->next;
	}
	if (cur->sin_addr.s_addr == sin_addr->s_addr)
		return cur;
	else
		return NULL;

}


//根据用户名获取user结构体
struct user* getUser_name(char name[])
{
	struct user* cur;
	cur = userlist_head;
	while ((cur->next != NULL)  && (strcmp(name, cur->next->name) != 0))
	{
		cur = cur->next;
	}
	if (cur->next != NULL && strcmp(name, cur->next->name) == 0)
		return cur->next;
	else
		return NULL;

}


//根据ip 得到name
char* getName(struct in_addr* sin_addr)//根据ip查找用户的name
{
	int none = 0;
	
	struct user* cur;
	cur = userlist_head->next;
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


	
	



