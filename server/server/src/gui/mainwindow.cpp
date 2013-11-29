#include <evse/gui/mainwindow.hpp>
#include "ui_mainwindow.h"

evse::gui::mainwindow::mainwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

evse::gui::mainwindow::~mainwindow()
{
    delete ui;
}
