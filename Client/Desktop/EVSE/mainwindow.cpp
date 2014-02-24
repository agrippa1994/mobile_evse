#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColor>
#include <QBrush>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow),
    _socket(this)
{
    ui->setupUi(this);

    QObject::connect(&_socket, SIGNAL(connected()), SLOT(tcp_connected()));
    QObject::connect(&_socket, SIGNAL(disconnected()), SLOT(tcp_disconnected()));
    QObject::connect(&_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), SLOT(tcp_stateChange(QAbstractSocket::SocketState)));
    QObject::connect(&_socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(tcp_error(QAbstractSocket::SocketError)));

    _socket.connectToHost("10.0.10.1", 2425);
    QTimer::singleShot(0, this, SLOT(timer()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::tcp_connected()
{
    networkLog("Client hat sich mit der Tankstelle verbunden");
}

void MainWindow::tcp_disconnected()
{
    networkLog("Client hat die Verbindung zur Tankstelle getrennt / verloren");
}

void MainWindow::tcp_stateChange(QAbstractSocket::SocketState state)
{
    switch(state)
    {
    case QAbstractSocket::UnconnectedState:
        networkLog("Verbindung geschlossen");
        break;

    case QAbstractSocket::HostLookupState:
        networkLog("Host wird aufgelöst");
        break;

    case QAbstractSocket::ConnectingState:
        networkLog("Verbindung wird aufgebaut ...");
        break;

    case QAbstractSocket::ConnectedState:
        networkLog("Verbindung wurde erfolgreich aufgebaut");
        break;

    case QAbstractSocket::ClosingState:
        networkLog("Verbindung wird geschlossen");
        break;
    }
}

void MainWindow::tcp_error(QAbstractSocket::SocketError)
{
    networkLog("Ein Fehler ist aufgetreten");
}

void MainWindow::timer()
{

    QTimer::singleShot(500, this, SLOT(timer()));
}

void MainWindow::menu_info()
{
    QMessageBox::information(this, "Info",
                             "Programmierer: Manuel Stampfl\r\n"
                             "All rights reserved", "OK");
}

void MainWindow::setEVSEState(int row)
{
    for(int c = 0; c < ui->stateTable->columnCount(); c ++)
        for(int r = 0; r < ui->stateTable->rowCount(); r ++)
            ui->stateTable->item(r, c)->setBackground(QBrush());

    for(int i=0;i<ui->stateTable->columnCount();i++)
        ui->stateTable->item(row, i)->setBackground(QBrush(QColor(0, 255, 0)));
}

void MainWindow::networkLog(const QString &str)
{
    ui->networkBrowser->append(str);
    ui->statusBar->setStatusTip(str);
}

QString MainWindow::sendAndRead(const QString &send)
{
    const int timeout = 5000;

    if(_socket.state() != QAbstractSocket::ConnectedState)
        return 0;

    QString s = send + "\r\n";
    _socket.write(s.toStdString().c_str());
    if(_socket.waitForReadyRead(timeout))
        return QString(_socket.readAll()).remove("\r\n");

    return 0;
}
