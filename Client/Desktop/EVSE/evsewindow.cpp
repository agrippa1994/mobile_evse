#include "evsewindow.h"
#include "ui_evsewindow.h"

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


EVSEWindow::EVSEWindow(class MainWindow *parent, const QString &Host) : QWidget(parent), ui(new Ui::EVSEWindow), _parent(parent), _Host(Host)
{
    ui->setupUi(this);

    setWindowTitle("EVSE Remote Control @ " + Host);

    QObject::connect(&_socket, SIGNAL(readyRead()), SLOT(tcp_data()));
    QObject::connect(&_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), SLOT(tcp_stateChange(QAbstractSocket::SocketState)));
    QObject::connect(&_socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(tcp_error(QAbstractSocket::SocketError)));

    QObject::connect(ui->intervalButton, SIGNAL(clicked()), SLOT(btn_commandwindow()));
    QObject::connect(ui->startButton, SIGNAL(clicked()), SLOT(btn_commandwindow()));
    QObject::connect(ui->stopButton, SIGNAL(clicked()), SLOT(btn_commandwindow()));
    QObject::connect(ui->digitalButton0, SIGNAL(clicked()), SLOT(btn_commandwindow()));
    QObject::connect(ui->digitalButton1, SIGNAL(clicked()), SLOT(btn_commandwindow()));
    QObject::connect(ui->pwmButton, SIGNAL(clicked()), SLOT(btn_commandwindow()));

    QObject::connect(ui->pushButton, SIGNAL(clicked()), SLOT(send()));

    ConnectToHost();
}

EVSEWindow::~EVSEWindow()
{
    if(_socket.state() != QAbstractSocket::UnconnectedState)
        _socket.close();

    delete ui;
}

void EVSEWindow::tcp_data()
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
    }
}

void EVSEWindow::tcp_stateChange(QAbstractSocket::SocketState state)
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

void EVSEWindow::tcp_error(QAbstractSocket::SocketError)
{
    networkLog("<font color=\"#FF0000\">Ein Fehler ist aufgetreten</font>");

    ConnectToHost();
}

void EVSEWindow::btn_commandwindow()
{
    bool bSend = false;

    if(sender() == ui->intervalButton)      bSend = sendAndRead(QString().sprintf("config --updatespeed %d", ui->intevalSpinBox->value()));
    else if(sender() == ui->startButton)    bSend = sendAndRead(QString().sprintf("startloading --current %d", ui->startSpinBox->value()));
    else if(sender() == ui->stopButton)     bSend = sendAndRead("stoploading");
    else if(sender() == ui->digitalButton0) bSend = sendAndRead(QString().sprintf("config --digitalWrite %d --value %d", ui->digitalSpinBoxPin->value(), 0));
    else if(sender() == ui->digitalButton1) bSend = sendAndRead(QString().sprintf("config --digitalWrite %d --value %d", ui->digitalSpinBoxPin->value(), 1));
    else if(sender() == ui->pwmButton)      bSend = sendAndRead(QString().sprintf("config --pwm %d", ui->pwmSpinBox->value()));

    if(bSend)
        networkLog("<font color=\"#00FF00\">Daten erfolgreich gesendet</font>");
    else
        networkLog("<font color=\"#FF0000\">Daten konnten nicht übermittelt werden!</font>");
}

void EVSEWindow::send()
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


void EVSEWindow::setEVSEState(int row)
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

void EVSEWindow::networkLog(const QString &str)
{
    ui->networkBrowser->append("<font style=\"font-weight:bold\">" + QDateTime::currentDateTime().toString("[dd.MM.yyyy hh:mm:ss:zzz] ") + "</font>" + str);
}

bool EVSEWindow::sendAndRead(const QString &send)
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

void EVSEWindow::dataForMainTable(const QString &key, const QString &value)
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

void EVSEWindow::onKeyAndValue(const QString &key, const QString &value)
{
    if(key == "state")
    {
        setEVSEState(value.toInt());
    }
}

void EVSEWindow::ConnectToHost()
{
    if(_socket.state() != QAbstractSocket::UnconnectedState)
        _socket.close();

    _socket.connectToHost(_Host, 2425);
}
