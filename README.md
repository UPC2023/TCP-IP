飞鸽传书IPMSG报告

第一章 系统概述

本系统名为IP Messenger，简称IPMSG，中文名为“飞鸽传书”，是一款用C语言写的局域网聊天和文件传输工具。它基于TCP协议传输文件，保证文件的完整性和正确性；基于UDP的即时消息传递，满足用户对于即时通讯的实时性要求。

本系统具有不依赖网络、界面简洁、上手容易的优点。它专注于文件和消息的收发，没有额外的复杂功能，适合在工作中进行实时通信和文档共享。













第二章 系统需求分析
2.1 任务概述
该系统应满足用户对通讯的的基本需求，确保用户能够顺畅地登录系统并参与信息交流
该系统应具有以下基本功能:
1) 用户上下线管理
2) 消息收发功能
3) 文件收发功能
4) 文件断点续传功能

……
2.2 功能性需求
1) 用户上下线管理：
   用户上线：用户可以通过账号登录系统，实现上线状态，并广播通知其他在线用户。
   用户下线：用户可以选择退出系统，实现下线状态，并广播通知其他在线用户。
2) 消息收发功能：
   接收消息：用户可以实时接收来自其他用户发送的文本消息。
   发送消息：用户可以编辑并发送文本消息给单个用户或群发。
3) 文件传输功能：
   接收文件：用户可以接收其他用户发送的文件。
   发送文件：用户可以上传文件并发送给其他用户。
4) 文件断点续传功能：
   断点续传：在文件传输过程中，如果传输中断，系统将记录传输进度。用户可以在网络恢复后，从上次中断的地方继续传输文件，无需重新开始，确保文件传输的完整性和效率。


2.3 非功能性需求
界面美观整洁个性化，允许每位用户根据个人喜好调整界面风格，拥有更好的用户体验。

2.4 开发技术简介
核心语言：采用C语言作为主要开发语言。
客户端图形界面框架：利用QT和C++搭建跨平台的图形用户界面。

2.4.1 开发工具简介
在Linux的Ubuntu平台上，采用GCC作为C语言的编译工具，在Windows上，利用QT Creator IDE构建客户端。

2.4.2 开发环境简介
主要是Linux与Windows两大操作系统环境。




第三章 系统总体设计
3.1 Ubuntu部分功能设计
3.1.1 基础功能设计
用户登录系统时，需设定一个昵称。如需查看可用指令，可输入“help”。		
图3.1 用户上线效果图
根据“帮助”，输入ls,可查看好友列表。
c



图3.2 查看好友效果图

3.1.2 消息功能设计
输入sendto+空格+用户名，将进入消息编辑模式。然后将所要编辑的消息输入于终端窗口，按回车发送，对方会显示消息弹窗。输入exit结束对话。





图3.2 查看好友效果图


图3.3 聊天窗口图

3.1.3 文件功能设计
输入sendfile+空格+用户名，将进入文件发送模式。然后将所要发送的文件输入于终端窗口，按回车发送。此时文件会加入到待发送列表，输入SFL可以查看。






图3.4 文件传输图
对方接收文件后，文件从发送文件列表中删除。


图3.5 文件传输被接收图

如果输入当前路径不存在的文件，会报错并退出sendfile程序。


图3.6 报错图

对方发来文件会有提示，并存入接收文件列表（命令为RFL)，此时输入getfile+空格+文件名即可接收，接收后文件从接受列表中删除。





图3.7 文件接收图

3.2 QT客户端部分功能设计
3.2.1 基础功能设计
客户端界面设计了四种主题色调，供用户根据个人偏好自由选择。










图3.8 图3.9 四种颜色展示图
输入IP地址，点击connect，弹出登录提示框，输入用户名，点击ok成功登录。



图3.10 登录过程展示图



图3.11 登录成功展示图

3.2.2 私聊群发功能设计
私聊：选择私聊用户，在对话框中编辑信息并点击私聊即可成功发送。
群发：在对话框中编辑消息，点击群发即可发送。
图3.12 消息收发展示图


3.2.2 文件功能设计
点击发送文件，对方同意后选择文件发送。







图3.12 文件收发展示图

如果在传输文件的过程中，接收方意外掉线，重新上线后可以点击“续传”继续接收文件。
















图3.13 断点续传展示图

第四章 系统功能实现原理
4.1 QT客户端模块原理
4.1.1 界面换肤原理
编写四个QSS文件作为样式，将需要用到的图片和样式表加入到resource中。

在main函数中添加了蓝色背景的QSS文件，作为默认的页面风格。

在UI设计中加了四个颜色按钮并命名，在MyWidget类中将四个按钮的点击信号连接到同一个槽函数set_style中。

在set_style函数中，根据触发信号的按钮的objectName属性，选择不同的QSS文件路径。然后打开并读取QSS文件内容，通过QApplication::setStyleSheet方法将其设置为应用程序的全局样式。

关键代码：
MyWidget部分代码如下：
MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    setWindowTitle("客户端");
   ……
//页面美化
       connect(ui->btn_1, &QPushButton::clicked, this, &MyWidget::set_style);
       connect(ui->btn_2, &QPushButton::clicked, this, &MyWidget::set_style);
       connect(ui->btn_3, &QPushButton::clicked, this, &MyWidget::set_style);
       connect(ui->btn_4, &QPushButton::clicked, this, &MyWidget::set_style);
}

set_style函数代码如下：
void MyWidget::set_style()
{
    QPushButton* btn;
    btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return; // 如果sender不是QPushButton，直接返回
    QString filePath;
    if("btn_1" == btn->objectName())        //蓝色
    {
        filePath = ":/s1.qss";
    }else if("btn_2" == btn->objectName())  //粉色
    {
        filePath = ":/s2.qss";
    }
    else if("btn_3" == btn->objectName())  //黄色
        {
            filePath = ":/s3.qss";
        }
    else if("btn_4" == btn->objectName())  //紫色
        {
            filePath = ":/s4.qss";
        }
    QFile file(filePath);//QSS文件所在的路径
    if (file.exists() ) {
        //以只读的方式打开
        file.open(QFile::ReadOnly);
        //以字符串的方式保存读出的结果
        QString styleSheet = QLatin1String(file.readAll());
        //设置全局样式
        qApp->setStyleSheet(styleSheet);
        // 关闭文件
file.close();
     }

}


4.2 服务器与客户端连接原理
在main函数中，首先创建了一个Server对象，然后依次调用InitSocket()、Bind()、Listen()方法来初始化服务器。这些方法分别用于创建服务器套接字、绑定IP地址和端口号、以及监听客户端连接。
while循环持续运行，只要服务器还在运行（isRun()返回true），就会调用OnRun()方法处理网络事件。服务器不运行则关闭。

关键代码：
Main程序部分代码如下：
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
        





4.3 Ubuntu部分原理
4.3.1 多任务原理
主函数中使用线程来处理不同的网络操作，主要有接收UDP消息、监听键盘输入和发送文件这三种操作。










图4.1 线程示例图
创建了三个线程：udp_rcv、key_scan和tcp_send。其中udp_msg_process()是处理UDP消息的线程函数，key_scan_process()是监听键盘输入的线程函数，tcp_send_file()是发送文件的线程函数。使用pthread_join()等待每个线程结束，最后退出程序。

主函数代码：

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


4.3.2 用户上下线原理
用户上线时，客户端会通过UDP广播发送一个包含用户信息的消息，这个消息会被发送到局域网内的所有设备。服务器收到这个消息后，会解析消息内容，获取用户的相关信息。服务器将用户的上线信息存储在内存中，并通知其他在线用户该用户的上线情况。

关键代码：

  void broadcast(void)
{
	struct sockaddr_in broad_cAast_addr;
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


4.3.3 消息编码与解码原理
运用sprintf()和sscanf()函数进行消息编码和解码，sscanf 用于从字符串中解析出格式化的数据。coding 函数中的printf()函数将要发送的信息编码成一个字符串，并将其保存到 buffer 中。transcode 函数中的sscanf()将字符串 buffer 解析并填充到结构体cmd object中。
void transcode(struct cmd *object, char *buffer, int len)
{
	sscanf(buffer, "1:%[^:]:%[^:]:%[^:]:%d:%[^\n]", object->id, object->name, object->hostname, &object->cmdid, object->buf);
}

int coding(char *buffer, unsigned int cmd, char *append)
{
	char hname[20];
	time_t h;
	time(&h);
	gethostname(hname, sizeof(hname));
	if (append == NULL) append = "";
	sprintf(buffer, "1:%ld:%s:%s:%d:%s",h, myname, hname, cmd, append);
}



总结
本系统，能够有效满足局域网内用户对于即时通讯和文件传输的需求，并具有实时性、传输效率和易用性等。 本研究的不足之处在于，只有Linux C部分的代码可以完整实现跨设备通讯，QT部分是以桥接模式设计，多设备通讯时可能出现卡顿，难以登录现象。未来希望能对于大量用户并发传输的稳定性进一步优化，扩展系统功能以支持广域网通信。 尽管存在一定的不足，但“飞鸽传书”系统在局域网通信领域的研究和应用仍具有积极意义。希望通过不断的改进和完善，本系统能够更好地服务于广大用户，提高工作效率和通信质量。
