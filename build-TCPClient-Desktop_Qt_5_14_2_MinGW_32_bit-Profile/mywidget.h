#ifndef MYWIDGET_H
#define MYWIDGET_H

#include "messageheader.h"
#include <QFile>
#include <QWidget>
#include <QTcpSocket>
#include <QListWidgetItem>
#include <QTimer>

namespace Ui {
class MyWidget;
}

struct Item
{
    Item(int sock, QListWidgetItem *item):sock(sock), item(item){}
    int sock;
    QListWidgetItem *item;

    operator == (const Item &item)
    {
        return this->sock == item.sock;
    }
};

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = 0);
    ~MyWidget();

private slots:
    void on_buttonConnect_clicked();    // 连接按钮按下
    void on_buttonSend_clicked();       // 发送按钮按下（群聊）
    void on_buttonSendOne_clicked();    // 发送按钮按下（私聊）
    void on_buttonClose_clicked();      // 接收按钮按下
    void on_buttonSendFile_clicked();   // 发送文件按钮按下
    void on_buttonContinue_clicked();   // 断点续传按钮按下

    void Connect();                     // 连接
    void Read();                        // 接收
    void Disconnected();                // 断开连接

    void timer_timeout();               // 文件发送

private:
    int isPrivatelyChat();                      // 是否可以私聊

    void initList();                            // 初始化列表
    void clearList();                           // 清空列表
    void addItem(int scok, char *name);         // 添加列表
    void removeItem(int scok);                  // 删除列表
    bool isSelect();                            // 是否有选中的人
    int getSelectSock();                        // 获取选择的人sock
    QString getNameBySock(int scok);            // 通过sock找到name
    int getSockByItem(QListWidgetItem *item);   // 通过item找到sock

    void OnNetMsg(DataHeader *header);          // 处理网络消息

    // 粘包问题 分包
    char szRecv[4096];      // 接收缓冲区
    char szMsgBuf[10240];   // 第二缓冲区 消息缓冲区
    int lastPos = 0;        // 消息缓冲区的数据尾部位置

    // 发送文件
    QTimer *timer;          // 定时器
    SendFile sendfile;
    QFile fileSend;
    qint64 sendSize;        // 已发送文件大小
    QString filePath;       // 文件路径
    bool onSend;            // 正在发送文件

    // 接收文件
    QFile fileRecv;
    qint64 recvSize;        // 已经接收文件的大小
    bool onRecv;            // 正在接收文件

    Ui::MyWidget *ui;

    QList<Item> clients;
    QTcpSocket *tcpSocket;
    QString name;
};

#endif // MYWIDGET_H
