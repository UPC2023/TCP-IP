#ifndef _MESSAGEHEADER_H
#define _MESSAGEHEADER_H

/// 消息类型
enum CMD
{
	CMD_LOGIN = 10,	   // 登录
	CMD_LOGIN_RESULT,  // 登录结果

	CMD_LOGOUT,			// 登出
	CMD_LOGOUT_RESULT,	// 登出结果

	CMD_TEXT_MESSAGE,  // 普通消息（群发）
	CMD_SEND_ONE,	   // 普通消息（私发）

	CMD_NEW_USER_JOIN,	// 新用户加入
	CMD_USER_LEAVE,		// 用户离开

	CMD_GET_ONE_USER,  // 客户端得到所有的在线用户（一个）

	CMD_SEND_REQUEST,		  // 发送文件请求
	CMD_SEND_FILE,			  // 发送文件（私发）
	CMD_SEND_REQUEST_RESULT,  // 发送文件请求结果

	CMD_SEND_CONTINUE_REQUEST,		   // 断点续传请求
	CMD_SEND_CONTINUE_REQUEST_RESULT,  // 断点续传请求结果

	CMD_ERROR  // 错误
};

/// 基类
struct DataHeader
{
	DataHeader()
	{
		dataLength = sizeof(DataHeader);
		cmd		   = CMD_ERROR;
	}
	short dataLength;  // 数据长度
	short cmd;		   // 消息类型
};

/// 登录
struct Login : public DataHeader
{
	Login()
	{
		dataLength = sizeof(Login);
		cmd		   = CMD_LOGIN;
	}
	char userName[32];	// 用户名
};

/// 登录结果
struct LoginResult : public DataHeader
{
	LoginResult()
	{
		dataLength = sizeof(LoginResult);
		cmd		   = CMD_LOGIN_RESULT;
		result	   = 0;
	}
	int result;	 // 返回结果
};

/// 登出
struct Logout : public DataHeader
{
	Logout()
	{
		dataLength = sizeof(Logout);
		cmd		   = CMD_LOGOUT;
	}
	char userName[32];	// 用户名
};

/// 登出结果
struct LogoutResult : public DataHeader
{
	LogoutResult()
	{
		dataLength = sizeof(LogoutResult);
		cmd		   = CMD_LOGOUT_RESULT;
		result	   = 0;
	}
	int result;	 // 返回结果
};

/// 群发消息
struct TextMessage : public DataHeader
{
	TextMessage()
	{
		dataLength = sizeof(TextMessage);
		cmd		   = CMD_TEXT_MESSAGE;
	}
	int	 sock;			 // 发送方的socket
	char message[1024];	 // 文本信息
};

/// 单发消息
struct SendOne : public DataHeader
{
	SendOne()
	{
		dataLength = sizeof(SendOne);
		cmd		   = CMD_SEND_ONE;
	}
	int	 sock;			 // 发送方的socket
	char message[1024];	 // 文本信息
};

/// 新用户加入
struct NewUserJoin : public DataHeader
{
	NewUserJoin()
	{
		dataLength = sizeof(NewUserJoin);
		cmd		   = CMD_NEW_USER_JOIN;
	}
	int	 sock;			// 新加入用户的socket
	char userName[32];	// 用户名
};

/// 用户离开
struct UserLeave : public DataHeader
{
	UserLeave()
	{
		dataLength = sizeof(UserLeave);
		cmd		   = CMD_USER_LEAVE;
	}
	int	 sock;			// 离开用户的socket
	char userName[32];	// 用户名
};

/// 获取所有在线用户信息
struct GetOneUser : public DataHeader
{
	GetOneUser()
	{
		dataLength = sizeof(GetOneUser);
		cmd		   = CMD_GET_ONE_USER;
	}
	int	 sock;			// 在线用户socket
	char userName[32];	// 用户名
};

/// 发送文件请求
struct SendRequest : public DataHeader
{
	SendRequest()
	{
		dataLength = sizeof(SendRequest);
		cmd		   = CMD_SEND_REQUEST;
	}
	int sock;  // write时为接收者socket read时为发送者socket
};

/// 发送文件
struct SendFile : public DataHeader
{
	SendFile()
	{
		dataLength = sizeof(SendFile);
		cmd		   = CMD_SEND_FILE;
	}
	char	  message[1024];  // 文件信息
	char	  fileName[32];	  // 文件名
	long long fileSize;		  // 文件大小
	long long sendSize;		  // 发送大小
	long long sendNumber;	  // 发送次数
	int		  sock;			  // write时为接收者socket read时为发送者socket
};

/// 发送文件请求结果
struct SendRequestResult : public DataHeader
{
	SendRequestResult()
	{
		dataLength = sizeof(SendRequestResult);
		cmd		   = CMD_SEND_REQUEST_RESULT;
	}
	int sock;	 // write时为接收者socket read时为发送者socket
	int result;	 // 返回结果
};

/// 断点续传请求
struct SendContinueRequest : public DataHeader
{
	SendContinueRequest()
	{
		dataLength = sizeof(SendContinueRequest);
		cmd		   = CMD_SEND_CONTINUE_REQUEST;
	}
	int sock;  // write时为接收者socket read时为发送者socket
};

/// 断点续传请求结果
struct SendContinueRequestResult : public DataHeader
{
	SendContinueRequestResult()
	{
		dataLength = sizeof(SendContinueRequestResult);
		cmd		   = CMD_SEND_CONTINUE_REQUEST_RESULT;
	}
	int		  sock;			 // write时为接收者socket read时为发送者socket
	int		  result;		 // 返回结果
	char	  fileName[32];	 // 文件名
	long long recvSize;		 // 已发送文件大小
};

#endif	// !_MESSAGEHEADER_H
