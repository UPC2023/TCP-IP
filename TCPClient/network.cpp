#include "mywidget.h"
#include "ui_mywidget.h"

#include <QinputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QSettings>

void MyWidget::Connect()
{
    ui->textEditRead->setText("成功和服务器建立好连接");

    // 连接完服务器需要立刻登陆
    bool flag = false;
    while (!flag)
    {
        bool ok = false;
        name = QInputDialog::getText(this, "请先登陆", "用户名", QLineEdit::Normal, "", &ok);
        if (ok && !name.isEmpty())
        {
            Login login;
            strcpy(login.userName, name.toUtf8().data());
            tcpSocket->write((const char*)&login, login.dataLength);
            flag = true;
        }
    }
}

void MyWidget::Read()
{
    // 获取对方发送的内容
    int nLen = tcpSocket->read(szRecv, 4096);
    // 将收取到的数据拷贝到消息缓冲区
    memcpy(szMsgBuf + lastPos, szRecv, nLen);
    // 消息缓冲区的数据尾部位置后移
    lastPos += nLen;
    // 判断消息缓冲区的数据长度大于消息头DataHeader长度
    while (lastPos >= (int)sizeof(DataHeader))
    {
        // 这时就可以知道当前消息的长度
        DataHeader* header = (DataHeader*)szMsgBuf;
        // 判断消息缓冲区的数据长度大于消息长度
        if (lastPos >= header->dataLength)
        {
            // 消息缓冲区剩余未处理数据的长度
            int nSize = lastPos - header->dataLength;
            // 处理网络消息
            OnNetMsg(header);
            // 将消息缓冲区剩余未处理数据前移
            memcpy(szMsgBuf, szMsgBuf + header->dataLength, nSize);
            // 消息缓冲区的数据尾部位置前移
            lastPos = nSize;
        }
        else
        {
            break;  // 消息缓冲区剩余数据不够一条完整消息
        }
    }
}

void MyWidget::Disconnected()
{
    ui->textEditRead->append("成功和服务器断开连接");
}

void MyWidget::OnNetMsg(DataHeader *header)
{
    // 追加到编辑区中
    switch (header->cmd)
    {
        case CMD_LOGIN_RESULT:
        {
            LoginResult *login = (LoginResult*)header;
            if (0 == login->result)
            {
                ui->textEditRead->append(QString("%1登陆成功").arg(name));
            }
            break;
        }

        case CMD_LOGOUT_RESULT:
        {
            LogoutResult *logout = (LogoutResult*)header;
            if (0 == logout->result)
            {
                ui->textEditRead->append(QString("%1登出成功").arg(name));
            }
            break;
        }

        case CMD_NEW_USER_JOIN:
        {
            NewUserJoin *userJoin = (NewUserJoin*)header;
            ui->textEditRead->append(QString("%1上线").arg(userJoin->userName));
            addItem(userJoin->sock, userJoin->userName);
            break;
        }

        case CMD_USER_LEAVE:
        {
            UserLeave *userLeave = (UserLeave*)header;
            ui->textEditRead->append(QString("%1下线").arg(userLeave->userName));
            removeItem(userLeave->sock);
            break;
        }

        case CMD_TEXT_MESSAGE:
        {
            TextMessage *message = (TextMessage*)header;
            ui->textEditRead->append(QString("群聊消息 %1：message:%2").arg(getNameBySock(message->sock)).arg(message->message));
            break;
        }

        case CMD_SEND_ONE:
        {
            SendOne *one = (SendOne*)header;
            ui->textEditRead->append(QString("私聊消息 %1：message:%2").arg(getNameBySock(one->sock)).arg(one->message));
            break;
        }

        case CMD_GET_ONE_USER:
        {
            GetOneUser *user = (GetOneUser*)header;
            ui->textEditRead->append(QString("服务端消息：在线成员：%1").arg(user->userName));
            addItem(user->sock, user->userName);
            break;
        }

        case CMD_SEND_REQUEST:
        {
            SendRequest *sendRequest = (SendRequest*)header;

            SendRequestResult ret;
            ret.sock = sendRequest->sock;

            if (onRecv) // 只能同时收一个文件
            {
                ret.result = 1;
            }
            else
            {
                // 用户判断是否接收
                int result = QMessageBox::question(this, "文件请求", QString("%1请求发送文件").arg(getNameBySock(sendRequest->sock)), QMessageBox::Yes | QMessageBox::No);
                ret.result = (result != QMessageBox::Yes);
            }

            tcpSocket->write((const char*)&ret, ret.dataLength);
            break;
        }

        case CMD_SEND_REQUEST_RESULT:
        {
            SendRequestResult *sendRequest = (SendRequestResult*)header;
            if (0 == sendRequest->result)
            {
                filePath = QFileDialog::getOpenFileName(this, "open", "../");
                if (!filePath.isEmpty()) // 如果选择文件路径有效
                {
                    QFileInfo info(filePath);   // 获取文件信息
                    sendSize = 0; // 已发送文件的大小
                    fileSend.setFileName(filePath); // 指定文件的名字

                    if (!fileSend.open(QIODevice::ReadOnly | QIODevice::Unbuffered)) // 只读方式打开文件
                    {
                        ui->textEditRead->append("只读方式打开文件失败");
                    }
                    else
                    {
                        sendfile.fileSize = info.size();    // 获取文件大小
                        strcpy(sendfile.fileName, info.fileName().toUtf8().data()); // 获取文件名字
                        sendfile.sendNumber = 0;    // 第一个包
                        sendfile.sock = sendRequest->sock;

                        // 开始发送
                        ui->textEditRead->append(QString("正在发送文件:%1").arg(sendfile.fileName));
                        ui->sendProgressBar->setValue(0);
                        timer->start(1);
                        onSend = true;
                    }
                }
                else
                {
                    ui->textEditRead->append("选择的文件无效");
                }
            }
            else
            {
                ui->textEditRead->append(QString("%1拒绝接收文件").arg(getNameBySock(sendRequest->sock)));
            }
            break;
        }

        // 接收文件
        case CMD_SEND_FILE:
        {
            SendFile *sendFile = (SendFile*)header;

            if (0 == sendFile->sendNumber)	// 文件的第一个包
            {
                onRecv = true;
                recvSize = 0;
                fileRecv.setFileName(sendFile->fileName);   // 指定文件的名字
                if (!fileRecv.open(QIODevice::WriteOnly | QIODevice::Unbuffered))   // 只写方式方式，打开文件
                {
                    ui->textEditRead->append("只写方式打开文件失败");
                    return;
                }

                ui->recvProgressBar->setValue(0);
                ui->textEditRead->append(QString("开始接收%1发送的文件：%2").arg(getNameBySock(sendFile->sock)).arg(sendFile->fileName));
            }

            fileRecv.write(sendFile->message, sendFile->sendSize);
            recvSize += sendFile->sendSize;

            ui->recvProgressBar->setValue(100 * ((double)recvSize/sendFile->fileSize));   // 更新进度条

            if (recvSize == sendFile->fileSize && 0 != sendFile->sendSize) // 文件接收完毕
            {
                fileRecv.close(); // 关闭文件
                onRecv = false;

                ui->recvProgressBar->setValue(100);
                ui->textEditRead->append(QString("接收文件%1结束").arg(sendFile->fileName));
            }

            break;
        }

        case CMD_SEND_CONTINUE_REQUEST:
        {
            SendContinueRequest *sendContinueRequest = (SendContinueRequest*)header;

            QSettings *ini = new QSettings("send.ini", QSettings::IniFormat);

            SendContinueRequestResult ret;
            ret.sock = sendContinueRequest->sock;
            ret.recvSize = ini->value("sendSize").toInt();
            strcpy(ret.fileName, ini->value("fileName").toString().toUtf8().data());

            int result = QMessageBox::question(this, "文件请求", QString("%1请求断点续传").arg(getNameBySock(sendContinueRequest->sock)), QMessageBox::Yes | QMessageBox::No);
            if (result == QMessageBox::Yes)  // 续传
            {
                ret.result = 0;
                tcpSocket->write((const char*)&ret, ret.dataLength);
                // 续传文件
                filePath = ini->value("filePath").toString();
                sendSize = ret.recvSize; // 发送文件的大小
                fileSend.setFileName(filePath); // 指定文件的名字

                if (!fileSend.open(QIODevice::ReadOnly | QIODevice::Unbuffered)) // 只读方式打开文件
                {
                    ui->textEditRead->append("只读方式打开文件失败");
                }
                else
                {
                    ui->sendProgressBar->setValue(0);

                    sendfile.fileSize = ini->value("fileSize").toInt();
                    strcpy(sendfile.fileName, ret.fileName);
                    sendfile.sendNumber = ini->value("sendNumber").toInt();
                    sendfile.sock = sendContinueRequest->sock;

                    fileSend.seek(sendSize);

                    ui->textEditRead->append(QString("继续从%1字节发送文件……").arg(sendSize));

                    timer->start(1);
                    onSend = true;
                }
            }
            else // 不续传
            {
                ret.result = 1;
                tcpSocket->write((const char*)&ret, ret.dataLength);
            }

            delete ini;

            break;
        }

        case CMD_SEND_CONTINUE_REQUEST_RESULT:
        {
            SendContinueRequestResult *ret = (SendContinueRequestResult*)header;

            if (0 == ret->result)
            {
                onRecv = true;
                recvSize = ret->recvSize;

                fileRecv.close();
                fileRecv.setFileName(ret->fileName);

                if (!fileRecv.open(QIODevice::WriteOnly | QIODevice::Unbuffered | QIODevice::Append))   // 只写方式方式，打开文件
                {
                    ui->textEditRead->append("只写方式打开文件失败");
                }
                else
                {
                    ui->textEditRead->append(QString("继续从%1字节接收%2的文件").arg(recvSize).arg(getNameBySock(ret->sock)));
                }
            }
            else
            {
                onRecv = false;
                ui->textEditRead->append(QString("%1拒绝断点续传").arg(getNameBySock(ret->sock)));
            }
            break;
        }
    }
}

void MyWidget::timer_timeout()
{
    sendfile.sendSize = fileSend.read(sendfile.message, 1024);    // 从文件中读数据

    if (sendfile.sendSize > 0)
    {
        if (tcpSocket->write((const char*)&sendfile, sendfile.dataLength) <= 0)  // 发送数据
        {
            // 网络连接断开 文件还没传输完成
            ui->textEditRead->append(QString("sendSize:%1  fileSize: %2").arg(sendSize).arg(sendfile.fileSize));
            fileSend.close();
            timer->stop();
            onSend = false;

            QSettings *ini = new QSettings("send.ini", QSettings::IniFormat);
            ini->setValue("filePath", filePath);
            ini->setValue("fileName", sendfile.fileName);
            ini->setValue("fileSize", sendfile.fileSize);
            ini->setValue("sendNumber", sendfile.sendNumber);
            ini->setValue("sendSize", sendSize);
            delete ini;
        }
        else
        {
            sendSize += sendfile.sendSize;    // 发送的数据需要累积
            ++sendfile.sendNumber;
            ui->sendProgressBar->setValue(100 * ((double)sendSize/sendfile.fileSize));
        }
    }

    // 是否发送文件完毕
    if (sendSize == sendfile.fileSize)
    {
        ui->textEditRead->append(QString("sendSize:%1  fileSize: %2").arg(sendSize).arg(sendfile.fileSize));

        ui->textEditRead->append("文件发送完毕");
        fileSend.close();
        ui->sendProgressBar->setValue(100);

        timer->stop();
        onSend = false;
    }
}
