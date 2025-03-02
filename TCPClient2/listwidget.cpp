#include "mywidget.h"
#include "ui_mywidget.h"

void MyWidget::initList()
{
    ui->listWidget->setViewMode(QListView::ListMode);
}

void MyWidget::clearList()
{
    ui->listWidget->clear();
    clients.clear();
}

void MyWidget::addItem(int scok, char *name)
{
    QListWidgetItem *addItem = new QListWidgetItem(tr(name), ui->listWidget);
    clients.append(Item(scok, addItem));
}

void MyWidget::removeItem(int scok)
{
    for (int n = (int)clients.size() - 1; n >= 0; --n)
    {
        if (scok == clients[n].sock)
        {
            auto iter = clients.begin() + n;
            if (iter != clients.end())
            {
                delete iter->item;
                clients.erase(iter);
                break;
            }
        }
    }
}

bool MyWidget::isSelect()
{
    return !ui->listWidget->selectedItems().empty();
}

int MyWidget::getSelectSock()
{
    QListWidgetItem *item = ui->listWidget->selectedItems().at(0);
    return getSockByItem(item);
}

QString MyWidget::getNameBySock(int scok)
{
    for (auto item : clients)
    {
        if (scok == item.sock)
        {
            return item.item->text();
        }
    }

    return QString("none");
}

int MyWidget::getSockByItem(QListWidgetItem *item)
{
    for (auto temp : clients)
    {
        if (item == temp.item)
        {
            return temp.sock;
        }
    }

    return 0;
}
