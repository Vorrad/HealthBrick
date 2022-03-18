#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDomNode>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_newWindowAction_triggered();
    void on_closeWindowAction_triggered();

    void on_loadXMLButton_clicked();

private:
    Ui::MainWindow *ui;

    void search(QDomNode &n);
};
#endif // MAINWINDOW_H
