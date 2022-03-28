#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QDomNode>
#include <QListWidgetItem>
#include <QListWidget>
#include <QTextBrowser>
#include <QDate>
#include <QComboBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:

    void on_openFileAction_triggered();  // 打开文件
    void on_newWindowAction_triggered();    // 新建窗口
    void on_closeWindowAction_triggered();  // 关闭窗口

    bool loadXML(const QString filePath); // 读XML文档

    void changeFoodSelect(int row);
    void changeToday(const QString date_str); // 改变当前日期选择

    void addFood();
    void editFood();
    void deleteFood();

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();    

    QWidget* createFoodList();  // 创建窗口1：今日菜单
    QWidget* createDetails();   // 创建窗口2：营养详情
    QWidget* createReport();    // 创建窗口3：营养报告

    void setFoodList(const QStringList nameList);
    void setToday();            // 配置当前日期
    void setReport();

    QStringList getFoodList(QDomElement list);  // 输入food_list，获取食物名列表
    QStringList getFoodDetail(QDomElement food);// 输入food，获取食物属性列表
    QStringList getDates();
    QDomDocument* getDoc();
    QString getFilePath();
    QDomElement getFoodlistElement();
    int getFoodRow();


    QDomElement searchElement(const QString name, QDomElement root);

private:

    Ui::MainWindow *ui;

    QDate today;
    QString filePath;
    QDomDocument* mainDoc;       // 窗口显示的数据文件主体
    QDomNode root;
    QDomElement dayElement;
    QDomElement dateElement;
    QDomElement foodListElement;

    QComboBox* dateBox;
    QListWidget* foodList;
    QTextBrowser* summary;
    QTextBrowser* detail;
    QTextBrowser* report;

    int foodRow;                // 当前选中的行数

};
#endif // MAINWINDOW_H
