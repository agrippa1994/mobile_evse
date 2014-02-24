#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTimer>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void tcp_connected();
    void tcp_disconnected();
    void tcp_stateChange(QAbstractSocket::SocketState);
    void tcp_error(QAbstractSocket::SocketError);

    void timer();

    void menu_info();

private:
    Ui::MainWindow *ui;
    QTcpSocket _socket;

    void setEVSEState(int row);
    void networkLog(const QString & str);
    QString sendAndRead(const QString & send);
};

#endif // MAINWINDOW_H
