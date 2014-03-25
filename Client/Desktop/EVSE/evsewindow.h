#ifndef EVSEWINDOW_H
#define EVSEWINDOW_H

#include <QWidget>
#include <QTcpSocket>
#include <QMap>
#include <QPoint>

namespace Ui
{
    class EVSEWindow;
}

class EVSEWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EVSEWindow(class MainWindow *parent, const QString &Host);
    ~EVSEWindow();

    const QString & Host() const { return _Host; }

private slots:
    void tcp_data();
    void tcp_stateChange(QAbstractSocket::SocketState);
    void tcp_error(QAbstractSocket::SocketError);
    void btn_commandwindow();

    void send();

    void tableRightClick(const QPoint& p);

protected:
    virtual void hideEvent(class QHideEvent *p);

private:
    Ui::EVSEWindow*     ui;
    class MainWindow*   _parent;
    QString             _Host;
    QTcpSocket          _socket;
    int                 _state;
    bool                _shouldHide;

    QMap<QString, QString> _valKeys;

    void setEVSEState(int row);
    void networkLog(const QString & str);
    bool sendAndRead(const QString & send);

    void dataForMainTable(const QString & key, const QString & value);
    void onKeyAndValue(const QString & key, const QString & value);

    void ConnectToHost();
};

#endif // EVSEWINDOW_H
