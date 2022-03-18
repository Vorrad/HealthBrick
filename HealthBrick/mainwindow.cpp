#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDomDocument>
#include <QFile>

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


void MainWindow::on_loadXMLButton_clicked()
{
    QDomDocument doc("mydoc");
    QFile file(":/XML/demo.xml");
    if (!file.open(QIODevice::ReadOnly)){
        qDebug()<<"cannot open file";
        return;
    }
    if (!doc.setContent(&file)) {
        file.close();
        qDebug()<<"cannot set XML";
        return;
    }
    file.close();
    qDebug()<<QString("root element: %1").arg(doc.documentElement().tagName());
    QDomElement docElem = doc.documentElement();

    QDomNode n = docElem.firstChild();
    search(n);
}

void MainWindow::search(QDomNode &n)
{
    while(!n.isNull()) {
        QDomElement e = n.toElement();
        if (!e.isNull()){
            qDebug() << e.tagName(); // the node really is an element.
            if (e.hasChildNodes()){
                QDomNode n_child = e.firstChild();
                MainWindow::search(n_child);
            }
        }
        n = n.nextSibling();
    }
}

