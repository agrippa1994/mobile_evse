#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColor>
#include <QBrush>
#include <QMessageBox>
#include <QInputDialog>
#include <QSettings>
#include <QDebug>
#include <QVariant>
#include <QDateTime>
#include <QWidget>
#include <QStyleFactory>
#include <QMdiSubWindow>

#include "evsewindow.h"
#include "servermanager.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->actionNeue_Verbindung, SIGNAL(triggered()), SLOT(menu_newConnection()));
    QObject::connect(ui->actionServermanager, SIGNAL(triggered()), SLOT(menu_serverManager()));

    QObject::connect(&_serverManager, SIGNAL(ConnectionRequest(QString)), SLOT(servermanager_connectionRequest(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::menu_newConnection()
{
    bool OK = false;

    QString label = "Geben Sie eine Hostadresse bzw. eine IP ein, mit der Sie sich verbinden möchten:";
    QString titel = "Verbindung herstellen";

    QString ip = QInputDialog::getText(this, titel, label, QLineEdit::Normal, "10.0.10.1", &OK);
    if(!OK || ip.length() == 0)
        return;

    if(!addSubWindow(ip))
        QMessageBox::warning(this, "Info", "Es ist schon ein Fenster mit dieser IP vorhanden", "OK");
}

void MainWindow::menu_serverManager()
{
    _serverManager.show();
}

void MainWindow::servermanager_connectionRequest(const QString &ip)
{
    if(!addSubWindow(ip))
        QMessageBox::warning(this, "Info", "Es ist schon ein Fenster mit dieser IP vorhanden", "OK");
}

bool MainWindow::addSubWindow(const QString &host)
{
    QList<QMdiSubWindow *> list = ui->mdiArea->subWindowList();
    for(int i=0;i<list.size();i++)
    {
        EVSEWindow *wnd = dynamic_cast<EVSEWindow *>( list.at(i)->widget() );

        if(wnd == 0)
            continue;

        if(host == wnd->Host())
            return false;
    }

    ui->mdiArea->addSubWindow(new EVSEWindow(this, host))->show();
    return true;
}
