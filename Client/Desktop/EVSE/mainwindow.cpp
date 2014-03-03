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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), _socket(this)
{
    ui->setupUi(this);

    QObject::connect(&_socket, SIGNAL(readyRead()), SLOT(tcp_data()));
    QObject::connect(&_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), SLOT(tcp_stateChange(QAbstractSocket::SocketState)));
    QObject::connect(&_socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(tcp_error(QAbstractSocket::SocketError)));

    QObject::connect(ui->actionVerbindung_ffnen, SIGNAL(triggered()), SLOT(menu_network()));
    QObject::connect(ui->actionVerbindung_schlie_en, SIGNAL(triggered()), SLOT(menu_network()));

    QObject::connect(ui->intervalButton, SIGNAL(clicked()), SLOT(btn_commandwindow()));
    QObject::connect(ui->startButton, SIGNAL(clicked()), SLOT(btn_commandwindow()));
    QObject::connect(ui->stopButton, SIGNAL(clicked()), SLOT(btn_commandwindow()));
    QObject::connect(ui->digitalButton, SIGNAL(clicked()), SLOT(btn_commandwindow()));
    QObject::connect(ui->pwmButton, SIGNAL(clicked()), SLOT(btn_commandwindow()));

    QObject::connect(ui->pushButton, SIGNAL(clicked()), SLOT(send()));

    if(!showIPDialogAndConnect())
        exit(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::tcp_data()
{
    QStringList messages = QString(_socket.readAll()).split("\r\n", QString::SkipEmptyParts);
    for(int i=0; i < messages.length(); i++)
    {
        messages[i] = messages[i].remove("\r\n");
        QStringList tokens = messages[i].split(" ", QString::SkipEmptyParts);
        for(int u=0;u<tokens.size();u++)
        {
            QStringList val_and_key = tokens[u].split(":");
            if(val_and_key.size() != 2)
                continue;

            QString key = val_and_key[0];
            QString val = val_and_key[1];

            onKeyAndValue(key, val);
            dataForMainTable(key, val);

        }

        if(ui->actionNachrichten_anzeigen->isChecked())
            networkLog("<font color=\"#0000AA\">" + messages[i] + "</font>");
    }
}

void MainWindow::tcp_stateChange(QAbstractSocket::SocketState state)
{
    switch(state)
    {
    case QAbstractSocket::UnconnectedState:
        networkLog("<font color=\"#FF0000\">Verbindung geschlossen</font>");
        break;

    case QAbstractSocket::HostLookupState:
        networkLog("<font color=\"#B45F04\">Host wird aufgelöst</font>");
        break;

    case QAbstractSocket::ConnectingState:
        networkLog("<font color=\"#B45F04\">Verbindung wird aufgebaut ...</font>");
        break;

    case QAbstractSocket::ConnectedState:
        networkLog("<font color=\"#00FF00\">Verbindung wurde erfolgreich aufgebaut</font>");
        break;

    case QAbstractSocket::ClosingState:
        networkLog("<font color=\"#FF0000\">Verbindung wird geschlossen</font>");
        break;
    }
}

void MainWindow::tcp_error(QAbstractSocket::SocketError)
{
    networkLog("<font color=\"#FF0000\">Ein Fehler ist aufgetreten</font>");
}

void MainWindow::btn_commandwindow()
{
    if(sender() == ui->intervalButton)
    {
        if(sendAndRead(QString().sprintf("config --updatespeed %d", ui->intevalSpinBox->value())))
            networkLog("<font color=\"#00FF00\">Daten erfolgreich gesendet</font>");
        else
            networkLog("<font color=\"#FF0000\">Daten konnten nicht übermittelt werden!</font>");
    }
    else if(sender() == ui->startButton)
    {
        if(sendAndRead(QString().sprintf("startloading --current %d", ui->startSpinBox->value())))
            networkLog("<font color=\"#00FF00\">Daten erfolgreich gesendet</font>");
        else
            networkLog("<font color=\"#FF0000\">Daten konnten nicht übermittelt werden!</font>");
    }
    else if(sender() == ui->stopButton)
    {
        if(sendAndRead("stoploading"))
            networkLog("<font color=\"#00FF00\">Daten erfolgreich gesendet</font>");
        else
            networkLog("<font color=\"#FF0000\">Daten konnten nicht übermittelt werden!</font>");
    }

    else if(sender() == ui->digitalButton)
    {
        if(sendAndRead(QString().sprintf("config --digitalWrite %d --value %d", ui->digitalSpinBoxPin->value(), ui->digitalSpinBoxVal->value())))
            networkLog("<font color=\"#00FF00\">Daten erfolgreich gesendet</font>");
        else
            networkLog("<font color=\"#FF0000\">Daten konnten nicht übermittelt werden!</font>");
    }
    else if(sender() == ui->pwmButton)
    {
        if(sendAndRead(QString().sprintf("config --pwm %d", ui->pwmSpinBox->value())))
            networkLog("<font color=\"#00FF00\">Daten erfolgreich gesendet</font>");
        else
            networkLog("<font color=\"#FF0000\">Daten konnten nicht übermittelt werden!</font>");
    }

}

void MainWindow::menu_info()
{
    QMessageBox::information(this, "Info",
                             "Programmierer: Manuel Stampfl\r\n"
                             "All rights reserved", "OK");
}

void MainWindow::send()
{
    QString text = ui->lineEdit->text();
    if(text.length() == 0)
        return;


    if(sendAndRead(text))
        networkLog("<font color=\"#00FF00\">Daten erfolgreich gesendet</font>");
    else
        networkLog("<font color=\"#FF0000\">Daten konnten nicht übermittelt werden!</font>");

    ui->lineEdit->clear();
}

void MainWindow::menu_network()
{
    if(sender() == ui->actionVerbindung_ffnen)
    {
        showIPDialogAndConnect();
    }
    else if(sender() == ui->actionVerbindung_schlie_en)
    {
        if(_socket.state() != QAbstractSocket::UnconnectedState)
            _socket.close();
    }
}

void MainWindow::setEVSEState(int row)
{
    for(int c = 0; c < ui->stateTable->columnCount(); c ++)
    {
        for(int r = 0; r < ui->stateTable->rowCount(); r ++)
        {
            ui->stateTable->item(r, c)->setBackground(QBrush());
        }
    }

    for(int i=0;i<ui->stateTable->columnCount();i++)
    {
        ui->stateTable->item(row, i)->setBackground(QBrush(QColor(0, 255, 0)));
    }
}

void MainWindow::networkLog(const QString &str)
{
    ui->networkBrowser->append("<font style=\"font-weight:bold\">" + QDateTime::currentDateTime().toString("[dd.MM.yyyy hh:mm:ss:zzz] ") + "</font>" + str);
}

bool MainWindow::sendAndRead(const QString &send)
{
    const int timeout = 500;

    if(_socket.state() != QAbstractSocket::ConnectedState)
        return 0;

    QString s = send + "\r\n";
    _socket.write(s.toStdString().c_str());
    if(_socket.waitForBytesWritten(timeout))
        return true;

    return false;
}

void MainWindow::dataForMainTable(const QString &key, const QString &value)
{
    QTableWidget *table = ui->mainTable;
    QList<QTableWidgetItem *> items = table->findItems(key, Qt::MatchCaseSensitive);

    if(items.length() != 0)
    {
        table->item(table->row(items[0]), 1)->setText(value);
        return;
    }

    int rowIdx = table->rowCount();
    table->insertRow(rowIdx);

    table->setItem(rowIdx, 0,  new QTableWidgetItem(key));
    table->setItem(rowIdx, 1,  new QTableWidgetItem(value));

    return;
}

void MainWindow::onKeyAndValue(const QString &key, const QString &value)
{
    if(key == "state")
    {
        setEVSEState(value.toInt());
    }
}

bool MainWindow::showIPDialogAndConnect()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    QString readedIP = settings.value("IP", "10.0.10.1").toString();

    bool bOK = false;
    QString ip = QInputDialog::getText(this, "Server-Daten", "IP-Adresse", QLineEdit::Normal, readedIP, &bOK);
    if(!bOK)
        return false;

    // Schreiben der IP in die INI-Datei
    settings.setValue("IP", ip);

    if(_socket.state() != QAbstractSocket::UnconnectedState)
        _socket.close();

    _socket.connectToHost(ip, 2425);

    return true;
}
