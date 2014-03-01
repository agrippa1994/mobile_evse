#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

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
    void tcp_data();
    void tcp_stateChange(QAbstractSocket::SocketState);
    void tcp_error(QAbstractSocket::SocketError);

    void menu_info();

    void send();
private:
    Ui::MainWindow *ui;
    QTcpSocket _socket;
    int _state;

    void setEVSEState(int row);
    void networkLog(const QString & str);
    bool sendAndRead(const QString & send);
};

#endif // MAINWINDOW_H
