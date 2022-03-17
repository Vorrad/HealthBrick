#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_newWindowAction_triggered()
{
    MainWindow* newWindow = new MainWindow(this);
        QRect parentGeometry = this->geometry();
        // 新建窗口与父窗口
        newWindow->setGeometry(parentGeometry.x()+30,
                               parentGeometry.y()+30,
                               parentGeometry.width(),
                               parentGeometry.height());
        newWindow->show();
}

void MainWindow::on_closeWindowAction_triggered()
{
    this->close();
}

