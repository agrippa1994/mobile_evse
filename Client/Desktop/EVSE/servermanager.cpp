#include "servermanager.h"
#include "ui_servermanager.h"

#include <QHideEvent>
#include <QShowEvent>

#include <QVariant>
#include <QByteArray>
#include <QDataStream>
#include <QMessageBox>
#include <QMenu>
#include <QSettings>
#include <QPair>
#include <QList>

ServerManager::ServerManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerManager)
{
    ui->setupUi(this);

    QObject::connect(ui->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), SLOT(tableRightClick(QPoint)));
}

ServerManager::~ServerManager()
{
    delete ui;
}

void ServerManager::showEvent(QShowEvent *p)
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    QSettings settings("settings.ini", QSettings::IniFormat);

    QList<QVariant> list = settings.value("servermanager").toList();

    for(int i=0; i < list.length(); i++)
    {
        QStringList host_and_name = list[i].toStringList();
        if(host_and_name.length() != 2)
            continue;

        QString name = host_and_name[0];
        QString host = host_and_name[1];

        int newRow = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(newRow);

        ui->tableWidget->setItem(newRow, 0 , new QTableWidgetItem(name));
        ui->tableWidget->setItem(newRow, 1, new QTableWidgetItem(host));
    }

    p->accept();
}

void ServerManager::hideEvent(QHideEvent *p)
{
    QSettings settings("settings.ini", QSettings::IniFormat);

    QList<QVariant> list;
    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        QStringList host_and_name;

        QTableWidgetItem *nameItem = ui->tableWidget->item(i,0);
        QTableWidgetItem *hostItem = ui->tableWidget->item(i,1);

        if(nameItem == 0 || hostItem == 0)
            continue;

        host_and_name.append(nameItem->text());
        host_and_name.append(hostItem->text());

        list.push_back(host_and_name);
    }

    settings.setValue("servermanager", list);

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    p->accept();
}

void ServerManager::tableRightClick(const QPoint& p)
{
    QPoint global = ui->tableWidget->mapToGlobal(p);

    QMenu menu;

    QTableWidgetItem *item;
    if((item = ui->tableWidget->itemAt(p)))
    {
        QAction *openAndCloseThis = menu.addAction("Verbindung aufbauen und dieses Fenster schließen");
        QAction *openAndLetOpen = menu.addAction("Verbindung aufbauen");
        menu.addSeparator();
        QAction *del = menu.addAction("Löschen");

        QAction *selected = menu.exec(global);

        if(openAndCloseThis == selected)
        {
            emit ConnectionRequest(ui->tableWidget->item(item->row(), 1)->text());
            hide();
        }
        else if(openAndLetOpen == selected)
        {
            emit ConnectionRequest(ui->tableWidget->item(item->row(), 1)->text());
        }
        else if(del == selected)
        {
            ui->tableWidget->removeRow(item->row());
        }
    }
    else
    {
        QAction *insert = menu.addAction("Einfügen");
        if(insert == menu.exec(global))
        {
            int rowCount = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(rowCount);
        }
    }

}

