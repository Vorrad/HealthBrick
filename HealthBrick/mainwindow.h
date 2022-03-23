#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QDomNode>
#include <QListWidgetItem>
#include <QListWidget>
#include <QTextBrowser>
#include <QDate>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:



public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();    

    QWidget* createFoodList();  // 创建窗口1：今日菜单
    QWidget* createDetails();   // 创建窗口2：营养详情
    QWidget* createReport();    // 创建窗口3：营养报告
    void setFoodList(const QStringList nameList);

    QStringList getFoodList(QDomElement list);  // 输入food_list，获取食物名列表
    QStringList getFoodDetail(QDomElement food);// 输入food，获取食物属性列表


    QDomElement searchElement(const QString name, QDomElement root);

private slots:

    void on_openFileAction_triggered();  // 打开文件
    void on_newWindowAction_triggered();    // 新建窗口
    void on_closeWindowAction_triggered();  // 关闭窗口

    bool loadXML(const QString filePath); // 读XML文档

    void on_writeXMLButton_clicked();

    void changeFoodSelect(int row);

    void addFood();

private:

    Ui::MainWindow *ui;

    QDate today;
    QDomDocument* mainDoc;       // 窗口显示的数据文件主体
    QDomNode root;
    QDomElement dayElement;
    QDomElement dateElement;
    QDomElement foodListElement;

    QListWidget* foodList;
    QTextBrowser* summary;
    QTextBrowser* detail;
    QTextBrowser* report;


};
#endif // MAINWINDOW_H
