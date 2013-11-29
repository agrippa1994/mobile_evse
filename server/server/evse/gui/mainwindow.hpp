#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

namespace evse {
    namespace gui {

    class mainwindow : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit mainwindow(QWidget *parent = 0);
        ~mainwindow();

    private:
        Ui::MainWindow *ui;
    };
    }
}


#endif
