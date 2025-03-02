#include "mywidget.h"
#include "ui_mywidget.h"
#include <QHostAddress>

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    setWindowTitle("客户端");

    // 在线用户
    initList();

    // 进度条
    ui->sendProgressBar->setRange(0,100);
    ui->sendProgressBar->setValue(0);
    ui->recvProgressBar->setRange(0,100);
    ui->recvProgressBar->setValue(0);

    // 网络
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, &QTcpSocket::connected, this, &MyWidget::Connect);           // 连接
    connect(tcpSocket, &QTcpSocket::readyRead, this, &MyWidget::Read);              // recv
    connect(tcpSocket, &QTcpSocket::disconnected, this, &MyWidget::Disconnected);   // 断开连接

    // 文件
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MyWidget::timer_timeout);
    onRecv = false;
    onSend = false;
//页面美化
       connect(ui->btn_1, &QPushButton::clicked, this, &MyWidget::set_style);
       connect(ui->btn_2, &QPushButton::clicked, this, &MyWidget::set_style);
       connect(ui->btn_3, &QPushButton::clicked, this, &MyWidget::set_style);
       connect(ui->btn_4, &QPushButton::clicked, this, &MyWidget::set_style);
}

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
     }

}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::on_buttonConnect_clicked()
{
    // 不重复连接
    if (QAbstractSocket::ConnectingState == tcpSocket->state())
    {
        return;
    }

    // 主动和服务器建立连接
    tcpSocket->connectToHost(QHostAddress(ui->lineEditIP->text()), ui->lineEditPort->text().toInt());

    // 连接前清空在线用户信息
    clearList();
}

void MyWidget::on_buttonSend_clicked()
{
    // 先连接服务器后发送
    if (QAbstractSocket::UnconnectedState == tcpSocket->state())
    {
        ui->textEditRead->append("请先连接服务器！");
        return;
    }

    // 获取编辑框内容
    QString str = ui->textEditWrite->toPlainText();

    // 发送数据
    TextMessage message;
    strcpy(message.message, str.toUtf8().data());
    tcpSocket->write((const char*)&message, message.dataLength);

    // 显示
    ui->textEditRead->append(QString("我发送给所有人：%1").arg(message.message));
}

void MyWidget::on_buttonSendOne_clicked()
{
    int sock = isPrivatelyChat();
    if (sock != 0)
    {
        // 获取编辑框内容
        QString str = ui->textEditWrite->toPlainText();

        // 发送数据
        SendOne one;
        one.sock = sock;
        strcpy(one.message, str.toUtf8().data());
        tcpSocket->write((const char*)&one, one.dataLength);

        // 显示
        ui->textEditRead->append(QString("我发送给%1：%2").arg(getNameBySock(sock)).arg(one.message));
    }
}

void MyWidget::on_buttonClose_clicked()
{
    Logout logout;
    strcpy(logout.userName, name.toUtf8().data());
    tcpSocket->write((const char*)&logout, logout.dataLength);

    // 主动和对方断开连接
    tcpSocket->disconnectFromHost();
    tcpSocket->close();
}

void MyWidget::on_buttonSendFile_clicked()
{
    if (true == onSend)
    {
        ui->textEditRead->append("只能同时发送一个文件");
        return;
    }

    int sock = isPrivatelyChat();
    if (sock != 0)
    {
        SendRequest sendRequst;
        sendRequst.sock = sock;
        tcpSocket->write((const char*)&sendRequst, sendRequst.dataLength);
    }
}

void MyWidget::on_buttonContinue_clicked()
{
    int sock = isPrivatelyChat();
    if (sock != 0)
    {
        SendContinueRequest sendContinueRequst;
        sendContinueRequst.sock = sock;
        tcpSocket->write((const char*)&sendContinueRequst, sendContinueRequst.dataLength);
    }
}

int MyWidget::isPrivatelyChat()
{
    // 先连接服务器后发送
    if (QAbstractSocket::UnconnectedState == tcpSocket->state())
    {
        ui->textEditRead->append("请先连接服务器！");
        return 0;
    }
    // 选中人才能发送
    if (!isSelect())
    {
        ui->textEditRead->append("请先选择私聊对象！");
        return 0;
    }
    // 获得对方sock
    int sock = getSelectSock();
    if (sock == 0)
    {
        ui->textEditRead->append("sock找不到");
        return 0;
    }
    return sock;
}



