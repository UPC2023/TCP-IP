#ifndef _FILELIST_H_ 
#define _FILELIST_H_

struct sendfile 
{
	unsigned int num;//文件序号
	char name[20];	//文件名
	long size;      //文件大小
    long pkgnum;    //文件包号
	long ltime;     //修改时间
	struct in_addr sin_addr;//用户IP地址
	struct sendfile *next;	//指向下一节点	
};

struct rcvfile
{
	unsigned long  num;		//文件序号
	char name[20];	//文件名
	long size;      //文件大小
	long ltime;     //修改时间
	long pkgnum;    //包编号
	struct in_addr sin_addr;//用户IP地址
	struct rcvfile *next;	//指向下一节点	
};

int init_send_filelist();//初始化文件链表
int add_sendFile(struct in_addr* sin_addr, char name[]);//添加发送文件列表
int del_sendFile(struct in_addr sin_addr);//删除
int del_sendFile_name(char nam[]);//根据文件名删除
struct sendfile* get_sendFile(struct in_addr* sin_addr);//根据ip查找文件信息
struct sendfile* get_sendFile_name(char name[]);//根据文件名获取file结构体
struct sendfile* get_sendFile_num(int num);
char* get_sendFileName(struct in_addr* sin_addr);//根据ip查找filename

int init_rcv_filelist(void);
int add_rcvFile(struct in_addr* sin_addr, char name[],long num,long size);//添加接收文件列表
int del_rcvFile(struct in_addr sin_addr);//删除
int del_rcvFile_name(char nam[]);//根据文件名删除文件  正确删除返回0 错误返回1
struct rcvfile* get_rcvFile(struct in_addr* sin_addr);//根据ip查找文件信息
struct rcvfile* get_rcvFile_name(char nam[]);//根据文件名获取file结构体
char* get_rcvFileName(struct in_addr* sin_addr);//根据ip查找filename

#endif
