#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include <QFileDialog>
#include <QLayout>
#include <QTextBrowser>
#include <QListWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>
#include <QFileDialog>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    today = QDateTime::currentDateTime().date();

    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->addWidget(createFoodList());
    hLayout->addWidget(createDetails());
    hLayout->addWidget(createReport());

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(hLayout);

    QWidget* centralWidget = new QWidget;
    centralWidget->setLayout(layout);
    this->setCentralWidget(centralWidget);

    // 测试用
    loadXML(":/XML/demo.xml");
    report->setText(mainDoc->toString(2));

    int status=1;   // 状态标志 0：找到当前日期  1：未找到当前日期

    root = mainDoc->documentElement();    // 获取XML文件根元素
    dayElement = root.firstChildElement();
    QDomElement dateElement = dayElement.firstChildElement();
    while (!dayElement.isNull())
    {
        if (dateElement.firstChild().toText().data() == // 检查日期
                QString("%1/%2/%3").arg(today.year()).arg(today.month()).arg(today.day()))
        {
            status = 0;
            break;
        }
        dayElement = dayElement.nextSiblingElement();
        dateElement = dayElement.firstChildElement();
    }
    if (status == 1) QMessageBox::warning(this, "", "未找到今日数据！");
    else {
        qDebug()<<"today found! today:"+dateElement.firstChild().toText().data();
        foodListElement = dateElement.nextSiblingElement();

        QStringList foodList = getFoodList(foodListElement);
        setFoodList(foodList);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openFileAction_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this, "打开文件", " ",  QString("xml(*.xml);;所有文件(*.*)"));
    loadXML(filePath);
    report->setText(mainDoc->toString(2));

    QDomNode root = mainDoc->documentElement();    // 获取XML文件根元素
    qDebug()<<root.toElement().nodeType();
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

bool MainWindow::loadXML(const QString filePath)
{
    if (filePath == "") return false;

    QDomDocument* doc = new QDomDocument("mydoc");
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)){
        QMessageBox::critical(this, "出错啦",
                                     QString("无法打开文件 \"%1\"").arg(filePath));
        delete doc;
        return false;
    }

    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!doc->setContent(&file, false, &errorStr, &errorLine, &errorColumn))    //形参2：是否创建命名空间
    {
        QMessageBox::critical(this, "出错啦",QString("XML 文件解析出错，位置 %1 行, %2 列: %3")
                              .arg(errorLine).arg(errorColumn).arg(errorStr));

        delete doc;
        return false;
    }
    qDebug()<<"打开成功";
    file.close();
    mainDoc = doc;
    return true;
}

QWidget *MainWindow::createFoodList()
{
    QWidget* widget = new QWidget;
    widget->setFixedWidth(225);
    QVBoxLayout* layout = new QVBoxLayout(widget);

    QLabel* title = new QLabel("食物列表",widget);
    title->setAlignment(Qt::AlignHCenter);

    foodList = new QListWidget;
    for (int i=0;i<10;i++)
    {
        foodList->addItem(QString("面包%1").arg(i));
    }
    connect(foodList,SIGNAL(currentRowChanged(int)),this,SLOT(changeFoodSelect(int)));

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    QPushButton* addButton = new QPushButton("添加",widget);
    QPushButton* editButton = new QPushButton("编辑",widget);
    QPushButton* deleteButton = new QPushButton("删除",widget);

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);

    summary = new QTextBrowser;
    summary->setFixedHeight(175);
    summary->setText(QString("今天是%1年%2月%3日").arg(today.year()).arg(today.month()).arg(today.day()));

    layout->addWidget(title);
    layout->addWidget(foodList);
    layout->addLayout(buttonLayout);
    layout->addWidget(summary);

    return widget;
}

QWidget *MainWindow::createDetails()
{
    QWidget* widget = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(widget);

    QLabel* title = new QLabel("食物详情",widget);
    title->setAlignment(Qt::AlignHCenter);

    detail = new QTextBrowser;

    layout->addWidget(title);
    layout->addWidget(detail);

    return widget;
}

QWidget *MainWindow::createReport()
{
    QWidget* widget = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(widget);

    QLabel* title = new QLabel("营养报告",widget);
    title->setAlignment(Qt::AlignHCenter);

    report = new QTextBrowser;

    layout->addWidget(title);
    layout->addWidget(report);

    return widget;
}

void MainWindow::setFoodList(const QStringList nameList)
{
    foodList->clear();
    foodList->addItems(nameList);
}

QStringList MainWindow::getFoodList(QDomElement list)
{
    QDomElement foodElement = list.firstChildElement();
    QStringList foodList;

    while (!foodElement.isNull())
    {
        QString foodName = searchElement("name",foodElement).firstChild().toText().data();
        foodList.append(foodName);
        foodElement = foodElement.nextSiblingElement();
    }

    return foodList;
}

QStringList MainWindow::getFoodDetail(QDomElement food)
{
    QDomElement property = food.firstChildElement();
    QStringList foodDetail;  // {name, weight, carbo_cont, protein_cont, fat_cont, carbo, protein, fat}



    while (!property.isNull())
    {
        QString data = property.firstChild().toText().data();
        foodDetail.append(data);
        property = property.nextSiblingElement();
    }

    return foodDetail;
}

QDomElement MainWindow::searchElement(const QString name, QDomElement root)
{
    QDomElement element = root.firstChildElement();

    while (!element.isNull())
    {
        if (element.tagName() == name) return element;
        element = element.nextSiblingElement();
    }

    return QDomElement();
}

void MainWindow::on_writeXMLButton_clicked() // 写XML文档
{
    QString path = QFileDialog::getSaveFileName(this,"选择路径","D:/","XML(*.xml)");
    if(path.isEmpty())
        return;
    qDebug()<<path;

    QDomDocument doc;
    //创建处理指令
    QDomProcessingInstruction xmlInstruction =
            doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"");
    //添加注释
    QDomComment comment = doc.createComment("这是一个注释");
    //创建处理指令
    QDomProcessingInstruction styleInstruction =
            doc.createProcessingInstruction("xml-stylesheet","type=\"text/css\" href=\"style.css\"");
    doc.appendChild(xmlInstruction);//开始文档（XML 声明）
    doc.appendChild(comment);//注释
    doc.appendChild(styleInstruction);//处理指令

    //根元素
    QDomElement root = doc.createElement("Blogs");
    root.setAttribute("Version","1.0");
    doc.appendChild(root);

    //元素
    QDomElement child = doc.createElement("blogs");
    root.appendChild(child);

    //元素 作者 个人说明
    QDomElement author = doc.createElement("作者");
    QDomElement instruction = doc.createElement("个人说明");
    child.appendChild(author);
    child.appendChild(instruction);

    //元素中的内容
    QDomText authorText = doc.createTextNode("一去二三里");
    QDomText instructionText = doc.createTextNode("青春不老，奋斗不止！");
    author.appendChild(authorText);
    instruction.appendChild(instructionText);

    //保存xml文件
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    doc.save(out,QDomDocument::EncodingFromDocument);
    file.close();
}

void MainWindow::changeFoodSelect(int row)
{
    QDomElement currentFood = foodListElement.firstChildElement();

    for (int i=0;i<row;i++) currentFood = currentFood.nextSiblingElement(); // 单向查找至选定的食物
    QStringList properties = getFoodDetail(currentFood);
    QStringListIterator i(properties);
    QString detailText = "";

    detailText += QString("%1").arg(i.next());
    detailText += QString("\n净含量（g/mL）：%1").arg(i.next());
    detailText += QString("\n碳水含量（每100g）：%1").arg(i.next());
    detailText += QString("\n蛋白质含量（每100g）：%1").arg(i.next());
    detailText += QString("\n脂肪含量（每100g）：%1").arg(i.next());
    detailText += QString("\n碳水含量：%1").arg(i.next());
    detailText += QString("\n蛋白质含量：%1").arg(i.next());
    detailText += QString("\n脂肪含量：%1").arg(i.next());

    detail->setText(detailText);
}
