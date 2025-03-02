#ifndef _SERVER_H
#define _SERVER_H

#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <vector>
#include "MessageHeader.h"

#define SOCKET		   int
#define INVALID_SOCKET (SOCKET)(~0)
#define SOCKET_ERROR   (-1)

/// 客户端
struct Client
{
	Client(SOCKET sock, const char *userName, const sockaddr_in *addr)
	{
		this->sock = sock;
		strcpy(this->userName, userName);
		this->addr = *addr;
	}
	SOCKET		sock;			  // 服务器socket
	char		userName[32];	  // 用户名
	sockaddr_in addr;			  // 用户地址
	int			lastPos;		  // 消息缓冲区的数据尾部位置
	char		szMsgBuf[10240];  // 第二缓冲区 消息缓冲区
};

/// 服务器
class Server {
private:
	SOCKET				  serverSock;	 // 服务器socket
	std::vector< Client > clients;		 // 客户端socket
	char				  szRecv[4096];	 // 接收一级缓冲区

public:
	// 构造
	Server()
	{
		serverSock = INVALID_SOCKET;
	}

	// 析构
	virtual ~Server()
	{
		Close();
	}

	// 初始化Socket
	SOCKET InitSocket()
	{
		// 如果有旧链接则关闭
		if (INVALID_SOCKET != serverSock)
		{
			printf("<socket=%d>关闭旧连接...\n", (int)serverSock);
			Close();
		}

		// 建立客户端socket
		serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == serverSock)
		{
			printf("错误，建立socket失败...\n");
		}
		else
		{
			printf("建立socket=<%d>成功...\n", (int)serverSock);
		}
		return serverSock;
	}

	// 绑定IP和端口号
	int Bind(const char *ip, unsigned short port)
	{
		// 绑定前需要先有服务器socket
		if (INVALID_SOCKET == serverSock)
		{
			InitSocket();
		}

		sockaddr_in _sin = {};
		_sin.sin_family	 = AF_INET;
		_sin.sin_port	 = htons(port);

		// 参数ip为null则监听所有ip
		if (ip)
		{
			_sin.sin_addr.s_addr = inet_addr(ip);
		}
		else
		{
			_sin.sin_addr.s_addr = INADDR_ANY;
		}

		int ret = bind(serverSock, (sockaddr *)&_sin, sizeof(_sin));
		if (SOCKET_ERROR == ret)
		{
			printf("错误,绑定网络端口<%d>失败...\n", port);
		}
		else
		{
			printf("绑定网络端口<%d>成功...\n", port);
		}
		return ret;
	}

	// 监听端口号
	int Listen(int n)
	{
		int ret = listen(serverSock, n);
		if (SOCKET_ERROR == ret)
		{
			printf("socket=<%d>错误,监听网络端口失败...\n", serverSock);
		}
		else
		{
			printf("socket=<%d>监听网络端口成功...\n", serverSock);
		}
		return ret;
	}

	// 接受客户端连接
	SOCKET Accept()
	{
		sockaddr_in clientAddr = {};
		int			nAddrLen   = sizeof(sockaddr_in);
		SOCKET		clientSock = INVALID_SOCKET;

		clientSock = accept(serverSock, (sockaddr *)&clientAddr, (socklen_t *)&nAddrLen);

		if (INVALID_SOCKET == clientSock)
		{
			printf("socket=<%d>错误,接受到无效客户端SOCKET...\n", (int)serverSock);
		}
		else
		{
			clients.push_back(Client(clientSock, "", &clientAddr));
			printf("socket=<%d>新客户端加入：socket = %d,IP = %s \n", (int)serverSock, (int)clientSock, inet_ntoa(clientAddr.sin_addr));
		}
		return clientSock;
	}

	// 关闭Socket
	void Close()
	{
		if (serverSock != INVALID_SOCKET)
		{
			for (int n = (int)clients.size() - 1; n >= 0; --n)
			{
				close(clients[n].sock);
			}
			close(serverSock);
		}
	}

	// 处理网络消息
	bool OnRun()
	{
		if (isRun())
		{
			fd_set fdRead;				  // 描述符（socket）集合
			FD_ZERO(&fdRead);			  // 清理集合
			FD_SET(serverSock, &fdRead);  // 将描述符（socket）加入集合

			// 放置socket并且得到最大socket
			SOCKET maxSock = serverSock;
			for (int n = (int)clients.size() - 1; n >= 0; --n)
			{
				FD_SET(clients[n].sock, &fdRead);  // 集合中添加socket
				if (maxSock < clients[n].sock)
				{
					maxSock = clients[n].sock;
				}
			}

			/// nfds 是一个整数值 是指fd_set集合中所有描述符(socket)的范围，而不是数量
			/// 既是所有文件描述符最大值+1 在Windows中这个参数可以写0
			timeval t	= { 1, 0 };
			int		ret = select(maxSock + 1, &fdRead, NULL, NULL, &t);
			if (ret < 0)
			{
				printf("select任务结束。\n");
				Close();
				return false;
			}

			// 有链接
			if (FD_ISSET(serverSock, &fdRead))
			{
				FD_CLR(serverSock, &fdRead);
				Accept();
			}

			// 可读
			for (int n = (int)clients.size() - 1; n >= 0; --n)
			{
				if (FD_ISSET(clients[n].sock, &fdRead))
				{
					if (-1 == RecvData(clients[n]))
					{
						auto iter = clients.begin() + n;
						if (iter != clients.end())
						{
							// 通知其他用户 用户离开
							SendUserLeave(iter->sock, iter->userName);
							// 删除在线用户
							clients.erase(iter);
						}
					}
				}
			}

			return true;
		}
		return false;
	}

	// 是否工作中
	bool isRun()
	{
		return serverSock != INVALID_SOCKET;
	}

	// 接收数据
	int RecvData(Client &client)
	{
		int nLen = (int)recv(client.sock, szRecv, sizeof(DataHeader), 0);

		if (nLen <= 0)
		{
			printf("客户端<Socket=%d>已退出。\n", client.sock);
			return -1;
		}

		// 将收取到的数据拷贝到消息缓冲区
		memcpy(client.szMsgBuf + client.lastPos, szRecv, nLen);
		// 消息缓冲区的数据尾部位置后移
		client.lastPos += nLen;

		// 判断消息缓冲区的数据长度大于消息头DataHeader长度
		while (client.lastPos >= sizeof(DataHeader))
		{
			// 这时就可以知道当前消息的长度
			DataHeader *header = (DataHeader *)client.szMsgBuf;
			// 判断消息缓冲区的数据长度大于消息长度
			if (client.lastPos >= header->dataLength)
			{
				// 消息缓冲区剩余未处理数据的长度
				int nSize = client.lastPos - header->dataLength;
				// 处理网络消息
				OnNetMsg(client.sock, header);
				// 将消息缓冲区剩余未处理数据前移
				memcpy(client.szMsgBuf, client.szMsgBuf + header->dataLength, nSize);
				// 消息缓冲区的数据尾部位置前移
				client.lastPos = nSize;
			}
			else
			{
				break;	// 消息缓冲区剩余数据不够一条完整消息
			}
		}

		return 0;
	}

	// 响应网络消息
	virtual void OnNetMsg(SOCKET sock, DataHeader *header)
	{
		switch (header->cmd)
		{
		case CMD_LOGIN:
		{
			Login *login = (Login *)header;
			printf("收到客户端<Socket=%d>请求：CMD_LOGIN,数据长度：%d,userName=%s\n", sock, login->dataLength, login->userName);

			// 记录用户名
			for (int n = (int)clients.size() - 1; n >= 0; --n)
			{
				if (clients[n].sock == sock)
				{
					strcpy(clients[n].userName, login->userName);
				}
			}

			// 通知其他用户 新的用户加入
			SendNewUserJoin(sock, login->userName);

			// 登录返回消息
			SendLoginResult(sock);

			// 更新客户端在线信息
			SendGetOneUser(sock);

			break;
		}

		case CMD_LOGOUT:
		{
			Logout *logout = (Logout *)header;
			printf("收到客户端<Socket=%d>请求：CMD_LOGOUT,数据长度：%d,userName=%s \n", sock, logout->dataLength, logout->userName);

			SendLogoutResult(sock);
			break;
		}

		case CMD_TEXT_MESSAGE:
		{
			TextMessage *message = (TextMessage *)header;
			printf("收到客户端<Socket=%d>CMD_TEXT_MESSAGE,数据长度：%d,message=%s \n", sock, message->dataLength, message->message);

			SendTextMessage(sock, message);
			break;
		}

		case CMD_SEND_ONE:
		{
			SendOne *one = (SendOne *)header;
			printf("收到客户端<Socket=%d>CMD_SEND_ONE,数据长度：%d,sendTo=%d,message=%s \n", sock, one->dataLength, one->sock, one->message);

			SendOneToOne(sock, one->sock, one);
			break;
		}

		case CMD_SEND_REQUEST:
		{
			SendRequest *sendRequest = (SendRequest *)header;
			printf("收到客户端<Socket=%d>CMD_SEND_REQUEST,数据长度：%d,sendTo=%d \n", sock, sendRequest->dataLength, sendRequest->sock);

			SendOneToOne(sock, sendRequest->sock, sendRequest);
			break;
		}

		case CMD_SEND_REQUEST_RESULT:
		{
			SendRequestResult *ret = (SendRequestResult *)header;
			printf("收到客户端<Socket=%d>CMD_SEND_REQUEST_RESULT,数据长度：%d,sendTo=%d \n", sock, ret->dataLength, ret->sock);

			SendOneToOne(sock, ret->sock, ret);
			break;
		}

		case CMD_SEND_FILE:
		{
			SendFile *sendFile = (SendFile *)header;
			printf("收到客户端<Socket=%d>CMD_SEND_FILE,数据长度：%d,sendTo=%d,sendNumber=%lld,sendSize=%lld \n", sock, sendFile->dataLength, sendFile->sock, sendFile->sendNumber, sendFile->sendSize);

			SendOneToOne(sock, sendFile->sock, sendFile);
			break;
		}

		case CMD_SEND_CONTINUE_REQUEST:
		{
			SendContinueRequest *ret = (SendContinueRequest *)header;
			printf("收到客户端<Socket=%d>CMD_SEND_CONTINUE_REQUEST,数据长度：%d,sendTo=%d \n", sock, ret->dataLength, ret->sock);

			SendOneToOne(sock, ret->sock, ret);
			break;
		}

		case CMD_SEND_CONTINUE_REQUEST_RESULT:
		{
			SendContinueRequestResult *ret = (SendContinueRequestResult *)header;
			printf("收到客户端<Socket=%d>CMD_SEND_CONTINUE_REQUEST_RESULT,数据长度：%d,sendTo=%d \n", sock, ret->dataLength, ret->sock);

			SendOneToOne(sock, ret->sock, ret);
			break;
		}
		}
	}

	// 发送指定Socket数据
	int SendData(SOCKET sock, DataHeader *header)
	{
		if (isRun() && header)
		{
			return send(sock, (const char *)header, header->dataLength, 0);
		}
		return SOCKET_ERROR;
	}

	// 群发消息（不发给自己）
	void SendDataToOther(SOCKET sock, DataHeader *header)
	{
		for (int n = (int)clients.size() - 1; n >= 0; --n)
		{
			if (clients[n].sock != sock)
			{
				SendData(clients[n].sock, header);
			}
		}
	}

	void SendLoginResult(SOCKET sock)
	{
		LoginResult ret;
		SendData(sock, &ret);
	}

	void SendLogoutResult(SOCKET sock)
	{
		LogoutResult ret;
		SendData(sock, &ret);
	}

	void SendTextMessage(SOCKET sock, TextMessage *message)
	{
		message->sock = sock;
		SendDataToOther(sock, message);
	}

	void SendNewUserJoin(SOCKET sock, char *userName)
	{
		NewUserJoin userJoin;
		userJoin.sock = sock;
		strcpy(userJoin.userName, userName);
		SendDataToOther(sock, &userJoin);
	}

	void SendUserLeave(SOCKET sock, char *userName)
	{
		UserLeave userLeave;
		userLeave.sock = sock;
		strcpy(userLeave.userName, userName);
		SendDataToOther(sock, &userLeave);
	}

	void SendGetOneUser(SOCKET sock)
	{
		GetOneUser user;
		for (int n = (int)clients.size() - 1; n >= 0; --n)
		{
			if (clients[n].sock != sock)
			{
				user.sock = clients[n].sock;
				strcpy(user.userName, clients[n].userName);
				SendData(sock, &user);
			}
		}
	}

	template < class T >
	void SendOneToOne(SOCKET sock, SOCKET sendTo, T message)
	{
		message->sock = sock;
		SendData(sendTo, message);
	}
};

#endif	// !_SERVER_H
