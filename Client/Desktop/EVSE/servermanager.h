#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QWidget>

namespace Ui {
class ServerManager;
}

class ServerManager : public QWidget
{
    Q_OBJECT

public:
    explicit ServerManager(QWidget *parent = 0);
    ~ServerManager();

signals:
    void ConnectionRequest(const QString & ip);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void tableRightClick(const QPoint&);
    void connectButtonClicked();

private:
    Ui::ServerManager *ui;
};

#endif // SERVERMANAGER_H
