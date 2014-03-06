#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "servermanager.h"

namespace Ui
{
    class MainWindow;
}

class ServerManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Ui::MainWindow *UI() { return ui; }

private slots:
    void menu_newConnection();
    void menu_serverManager();

    void servermanager_connectionRequest(const QString &ip);

private:
    Ui::MainWindow *ui;
    ServerManager _serverManager;

    bool addSubWindow(const QString &host);
};

#endif // MAINWINDOW_H
