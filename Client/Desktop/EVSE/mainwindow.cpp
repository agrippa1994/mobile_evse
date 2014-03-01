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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow),
    _socket(this)
{
    ui->setupUi(this);

    qApp->setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    qApp->setPalette(darkPalette);

    qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");


    QObject::connect(&_socket, SIGNAL(readyRead()), SLOT(tcp_data()));
    QObject::connect(&_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), SLOT(tcp_stateChange(QAbstractSocket::SocketState)));
    QObject::connect(&_socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(tcp_error(QAbstractSocket::SocketError)));
    QObject::connect(ui->pushButton, SIGNAL(clicked()), SLOT(send()));

    QSettings settings("settings.ini", QSettings::IniFormat);
    QString readedIP = settings.value("IP", "10.0.10.1").toString();

    QString ip = QInputDialog::getText(this, "Server-Daten", "IP-Adresse", QLineEdit::Normal, readedIP);

    // Schreiben der IP in die INI-Datei
    settings.setValue("IP", ip);

    _socket.connectToHost(ip, 2425);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::tcp_data()
{
    QString msg = QString(_socket.readAll()).remove("\r\n");

    networkLog("<font color=\"#0000AA\">" + msg + "</font>");
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

void MainWindow::menu_info()
{
    QMessageBox::information(this, "Info",
                             "Programmierer: Manuel Stampfl\r\n"
                             "All rights reserved", "OK");
}

void MainWindow::send()
{
    QString & text = ui->lineEdit->text();
    if(text.length() == 0)
        return;


    if(sendAndRead(text))
        networkLog("<font color=\"#00FF00\">Daten erfolgreich gesendet</font>");
    else
        networkLog("<font color=\"#FF0000\">Daten konnten nicht übermittelt werden!</font>");

    ui->lineEdit->clear();
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
