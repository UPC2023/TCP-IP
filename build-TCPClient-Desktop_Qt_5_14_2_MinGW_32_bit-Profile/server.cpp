#include "Server.h"

int main()
{
	Server server;
	server.InitSocket();
	server.Bind(nullptr, 8080);
	server.Listen(5);

	while (server.isRun())
	{
		server.OnRun();
		//printf("空闲时间处理其它业务..\n");
	}
	
	server.Close();
	printf("已退出。\n");
	getchar();
	return 0;
}