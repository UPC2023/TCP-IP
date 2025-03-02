#ifndef _USERLIST_H_
#define _USERLIST_H_
#include<netinet/in.h>
//用户
struct user
{ 
	char name[20];	//用户名
	int  num;		//用户随机号
	char host[20];  //主机名
	struct in_addr sin_addr;//用户IP地址
	struct user *next;	//指向下一节点	
};

//添加用户 sin_addr唯一主键  重复name在后面加序号 添加完成返回0 错误返回1
int addUser(struct in_addr* sin_addr, char name[], char hostname[]);

//根据sin_addr 删除用户  正确删除返回0 错误返回1
int delUser(struct in_addr sin_addr);

//根据sin_addr 获取user结构体 没有返回NULL
struct user* getUser(struct in_addr* sin_addr);

//根据ip 得到name
char* getName(struct in_addr* sin_addr);

//根据用户名获取user结构体 没有返回NULL
struct user* getUser_name(char name[]);
int init_userlist();

#endif


















