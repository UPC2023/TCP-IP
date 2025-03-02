/********************************************************************************
** Form generated from reading UI file 'mywidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYWIDGET_H
#define UI_MYWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MyWidget
{
public:
    QGridLayout *gridLayout;
    QPushButton *btn_4;
    QPushButton *buttonSendOne;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer;
    QLabel *head;
    QTextEdit *textEditWrite;
    QProgressBar *recvProgressBar;
    QSpacerItem *verticalSpacer_2;
    QPushButton *buttonContinue;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *verticalSpacer;
    QPushButton *buttonConnect;
    QPushButton *btn_2;
    QSpacerItem *horizontalSpacer_7;
    QLabel *labelIP;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *buttonSend;
    QTextEdit *textEditRead;
    QPushButton *buttonClose;
    QLabel *labelPort;
    QListWidget *listWidget;
    QSpacerItem *horizontalSpacer_6;
    QLineEdit *lineEditIP;
    QPushButton *buttonSendFile;
    QProgressBar *sendProgressBar;
    QLineEdit *lineEditPort;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *btn_1;
    QPushButton *btn_3;

    void setupUi(QWidget *MyWidget)
    {
        if (MyWidget->objectName().isEmpty())
            MyWidget->setObjectName(QString::fromUtf8("MyWidget"));
        MyWidget->resize(632, 433);
        MyWidget->setStyleSheet(QString::fromUtf8(""));
        gridLayout = new QGridLayout(MyWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        btn_4 = new QPushButton(MyWidget);
        btn_4->setObjectName(QString::fromUtf8("btn_4"));

        gridLayout->addWidget(btn_4, 0, 10, 1, 1);

        buttonSendOne = new QPushButton(MyWidget);
        buttonSendOne->setObjectName(QString::fromUtf8("buttonSendOne"));

        gridLayout->addWidget(buttonSendOne, 7, 5, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 0, 4, 1, 1);

        horizontalSpacer = new QSpacerItem(104, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 7, 1, 1, 1);

        head = new QLabel(MyWidget);
        head->setObjectName(QString::fromUtf8("head"));
        head->setMinimumSize(QSize(80, 80));
        head->setStyleSheet(QString::fromUtf8(""));

        gridLayout->addWidget(head, 1, 1, 2, 1);

        textEditWrite = new QTextEdit(MyWidget);
        textEditWrite->setObjectName(QString::fromUtf8("textEditWrite"));

        gridLayout->addWidget(textEditWrite, 4, 1, 1, 9);

        recvProgressBar = new QProgressBar(MyWidget);
        recvProgressBar->setObjectName(QString::fromUtf8("recvProgressBar"));
        recvProgressBar->setValue(24);

        gridLayout->addWidget(recvProgressBar, 6, 1, 1, 10);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 2, 2, 1, 1);

        buttonContinue = new QPushButton(MyWidget);
        buttonContinue->setObjectName(QString::fromUtf8("buttonContinue"));

        gridLayout->addWidget(buttonContinue, 7, 7, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_5, 0, 6, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 1, 2, 1, 1);

        buttonConnect = new QPushButton(MyWidget);
        buttonConnect->setObjectName(QString::fromUtf8("buttonConnect"));

        gridLayout->addWidget(buttonConnect, 1, 10, 2, 1);

        btn_2 = new QPushButton(MyWidget);
        btn_2->setObjectName(QString::fromUtf8("btn_2"));

        gridLayout->addWidget(btn_2, 0, 7, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_7, 0, 9, 1, 1);

        labelIP = new QLabel(MyWidget);
        labelIP->setObjectName(QString::fromUtf8("labelIP"));

        gridLayout->addWidget(labelIP, 2, 4, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(104, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 7, 10, 1, 1);

        buttonSend = new QPushButton(MyWidget);
        buttonSend->setObjectName(QString::fromUtf8("buttonSend"));

        gridLayout->addWidget(buttonSend, 7, 4, 1, 1);

        textEditRead = new QTextEdit(MyWidget);
        textEditRead->setObjectName(QString::fromUtf8("textEditRead"));
        textEditRead->setReadOnly(true);

        gridLayout->addWidget(textEditRead, 3, 1, 1, 9);

        buttonClose = new QPushButton(MyWidget);
        buttonClose->setObjectName(QString::fromUtf8("buttonClose"));

        gridLayout->addWidget(buttonClose, 7, 8, 1, 1);

        labelPort = new QLabel(MyWidget);
        labelPort->setObjectName(QString::fromUtf8("labelPort"));

        gridLayout->addWidget(labelPort, 1, 4, 1, 1);

        listWidget = new QListWidget(MyWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setStyleSheet(QString::fromUtf8("selection-color: rgb(255, 255, 255);"));

        gridLayout->addWidget(listWidget, 3, 10, 2, 1);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_6, 0, 8, 1, 1);

        lineEditIP = new QLineEdit(MyWidget);
        lineEditIP->setObjectName(QString::fromUtf8("lineEditIP"));

        gridLayout->addWidget(lineEditIP, 2, 5, 1, 5);

        buttonSendFile = new QPushButton(MyWidget);
        buttonSendFile->setObjectName(QString::fromUtf8("buttonSendFile"));

        gridLayout->addWidget(buttonSendFile, 7, 6, 1, 1);

        sendProgressBar = new QProgressBar(MyWidget);
        sendProgressBar->setObjectName(QString::fromUtf8("sendProgressBar"));
        sendProgressBar->setValue(24);

        gridLayout->addWidget(sendProgressBar, 5, 1, 1, 10);

        lineEditPort = new QLineEdit(MyWidget);
        lineEditPort->setObjectName(QString::fromUtf8("lineEditPort"));

        gridLayout->addWidget(lineEditPort, 1, 5, 1, 5);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 1, 0, 1, 1);

        btn_1 = new QPushButton(MyWidget);
        btn_1->setObjectName(QString::fromUtf8("btn_1"));

        gridLayout->addWidget(btn_1, 0, 5, 1, 1);

        btn_3 = new QPushButton(MyWidget);
        btn_3->setObjectName(QString::fromUtf8("btn_3"));

        gridLayout->addWidget(btn_3, 0, 1, 1, 1);


        retranslateUi(MyWidget);

        QMetaObject::connectSlotsByName(MyWidget);
    } // setupUi

    void retranslateUi(QWidget *MyWidget)
    {
        MyWidget->setWindowTitle(QCoreApplication::translate("MyWidget", "MyWidget", nullptr));
        btn_4->setText(QCoreApplication::translate("MyWidget", "\347\264\253\350\211\262", nullptr));
        buttonSendOne->setText(QCoreApplication::translate("MyWidget", "\347\247\201\350\201\212", nullptr));
        head->setText(QString());
        buttonContinue->setText(QCoreApplication::translate("MyWidget", "\347\273\255\344\274\240", nullptr));
        buttonConnect->setText(QCoreApplication::translate("MyWidget", "connect", nullptr));
        btn_2->setText(QCoreApplication::translate("MyWidget", "\347\262\211\350\211\262", nullptr));
        labelIP->setText(QCoreApplication::translate("MyWidget", "\346\234\215\345\212\241\345\231\250IP", nullptr));
        buttonSend->setText(QCoreApplication::translate("MyWidget", "\347\276\244\345\217\221", nullptr));
        buttonClose->setText(QCoreApplication::translate("MyWidget", "close", nullptr));
        labelPort->setText(QCoreApplication::translate("MyWidget", "\346\234\215\345\212\241\345\231\250\347\253\257\345\217\243", nullptr));
        lineEditIP->setText(QCoreApplication::translate("MyWidget", "192.168.182.156", nullptr));
        buttonSendFile->setText(QCoreApplication::translate("MyWidget", "\345\217\221\351\200\201\346\226\207\344\273\266", nullptr));
        lineEditPort->setText(QCoreApplication::translate("MyWidget", "8080", nullptr));
        btn_1->setText(QCoreApplication::translate("MyWidget", "\350\223\235\350\211\262", nullptr));
        btn_3->setText(QCoreApplication::translate("MyWidget", "\351\273\204\350\211\262", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MyWidget: public Ui_MyWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYWIDGET_H
