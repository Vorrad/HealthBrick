#include "mainwindow.h"
#include "foodeditdialog.h"
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

    this->resize(1000,600);

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
//    on_openFileAction_triggered();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openFileAction_triggered()
{
    filePath = QFileDialog::getOpenFileName(this, "打开文件", " ",  QString("xml(*.xml);;所有文件(*.*)"));
    loadXML(filePath);
    report->setText(mainDoc->toString(2));

    root = mainDoc->documentElement();    // 获取XML文件根元素


    setToday();
    dateBox->addItems(getDates());
    dateBox->setCurrentIndex(dateBox->count()-1);
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
    file.close();

    mainDoc = doc;
    this->setWindowTitle(QString("HealthBrick - %1").arg(filePath));
    return true;
}

QWidget *MainWindow::createFoodList()
{
    QWidget* widget = new QWidget;
    widget->setFixedWidth(225);
    QVBoxLayout* layout = new QVBoxLayout(widget);

    QLabel* title = new QLabel("食物列表",widget);
    title->setAlignment(Qt::AlignHCenter);

    dateBox = new QComboBox(this);
    dateBox->setMaxVisibleItems(6);
    connect(dateBox,SIGNAL(currentTextChanged(QString)),this,SLOT(changeToday(QString)));

    foodList = new QListWidget;
    connect(foodList,SIGNAL(currentRowChanged(int)),this,SLOT(changeFoodSelect(int)));

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    QPushButton* addButton = new QPushButton("添加",widget);
    QPushButton* editButton = new QPushButton("编辑",widget);
    QPushButton* deleteButton = new QPushButton("删除",widget);
    connect(addButton,&QPushButton::clicked,this,&MainWindow::addFood);
    connect(editButton,&QPushButton::clicked,this,&MainWindow::editFood);
    connect(deleteButton,&QPushButton::clicked,this,&MainWindow::deleteFood);

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);

    summary = new QTextBrowser;
    summary->setFixedHeight(175);
    summary->setText(QString("%1年%2月%3日").arg(today.year()).arg(today.month()).arg(today.day()));

    layout->addWidget(title);
    layout->addWidget(dateBox);
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

void MainWindow::setToday()
{
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
    if (status == 1)
    {
        QMessageBox::warning(this, "", "未找到今日数据！");
        return;
    }
    else
    {
        foodListElement = dateElement.nextSiblingElement();

        QStringList foodList = getFoodList(foodListElement);
        setFoodList(foodList);
    }

    double carboTotal=0,proteinTotal=0,fatTotal=0;        // 总计

    QDomElement food = foodListElement.firstChildElement();
    while (!food.isNull())      // 累加对应属性
    {
        QDomElement curElement = food.firstChildElement()
                                        .nextSiblingElement()
                                        .nextSiblingElement()
                                        .nextSiblingElement();
        carboTotal += curElement.firstChild().toText().data().toDouble();
        curElement = curElement.nextSiblingElement().nextSiblingElement();
        proteinTotal += curElement.firstChild().toText().data().toDouble();
        curElement = curElement.nextSiblingElement().nextSiblingElement();
        fatTotal += curElement.firstChild().toText().data().toDouble();

        food = food.nextSiblingElement();
    }

    QDomElement oldTotalCount = dateElement.nextSiblingElement().nextSiblingElement();
    QDomElement newTotalCount = mainDoc->createElement("total_count");

    QDomElement carbo = mainDoc->createElement("carbo");
    QDomElement protein = mainDoc->createElement("protein");
    QDomElement fat = mainDoc->createElement("fat");

    QDomText carboText = mainDoc->createTextNode(QString("%1").arg(carboTotal));
    QDomText proteinText = mainDoc->createTextNode(QString("%1").arg(proteinTotal));
    QDomText fatText = mainDoc->createTextNode(QString("%1").arg(fatTotal));

    newTotalCount.appendChild(carbo);
    newTotalCount.appendChild(protein);
    newTotalCount.appendChild(fat);
    carbo.appendChild(carboText);
    protein.appendChild(proteinText);
    fat.appendChild(fatText);

    dayElement.replaceChild(newTotalCount,oldTotalCount);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)){
        QMessageBox::critical(this, "出错啦",
                                     QString("无法写入文件"));
        return;
    }
    QTextStream out(&file);
    mainDoc->save(out,QDomDocument::EncodingFromDocument);
    file.close();

    summary->setText(QString("%1年%2月%3日\n概览：\n碳水：%4 g\n蛋白质：%5 g\n脂肪：%6 g")
                     .arg(today.year()).arg(today.month()).arg(today.day())
                     .arg(carboTotal).arg(proteinTotal).arg(fatTotal));
    setReport();
}

void MainWindow::setReport()
{
    QString reportText;
    QDomElement day = root.firstChildElement();
    QDomElement totalCount;
    while (!day.nextSiblingElement().isNull()) day = day.nextSiblingElement();
    while (!day.isNull())
    {
        totalCount = day.firstChildElement().nextSiblingElement().nextSiblingElement();
        reportText.append(day.firstChildElement().firstChild().toText().data())
                .append("统计：\n")
                .append("碳水：")
                .append(totalCount.firstChildElement().firstChild().toText().data())
                .append(" g\n蛋白质：")
                .append(totalCount.firstChildElement().nextSiblingElement().firstChild().toText().data())
                .append(" g\n脂肪：")
                .append(totalCount.firstChildElement().nextSiblingElement().nextSiblingElement().firstChild().toText().data())
                .append(" g\n\n：");
        day = day.previousSiblingElement();
    }
    report->setText(reportText);
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

QStringList MainWindow::getDates()
{
    QStringList dateList;
    QDomElement day = root.firstChildElement();
    QString date;
    while (!day.isNull())
    {
        date = day.firstChildElement().firstChild().toText().data();
        dateList.append(date);
        day = day.nextSiblingElement();
    }
    return dateList;
}

QDomDocument* MainWindow::getDoc()
{
    return mainDoc;
}

QString MainWindow::getFilePath()
{
    return filePath;
}

QDomElement MainWindow::getFoodlistElement()
{
    return foodListElement;
}

int MainWindow::getFoodRow()
{
    return foodRow;
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

void MainWindow::changeFoodSelect(int row)
{
    foodRow = row;
    QDomElement currentFood = foodListElement.firstChildElement();

    for (int i=0;i<row;i++) currentFood = currentFood.nextSiblingElement(); // 单向查找至选定的食物
    QStringList properties = getFoodDetail(currentFood);
    QStringListIterator i(properties);
    QString detailText = "";

    detailText += QString("%1").arg(i.next());
    detailText += QString("\n净含量（g/mL）：%1").arg(i.next());
    detailText += QString("\n碳水含量（每100g）：%1").arg(i.next());
    detailText += QString("\n碳水含量：%1").arg(i.next());
    detailText += QString("\n蛋白质含量（每100g）：%1").arg(i.next());
    detailText += QString("\n蛋白质含量：%1").arg(i.next());
    detailText += QString("\n脂肪含量（每100g）：%1").arg(i.next());
    detailText += QString("\n脂肪含量：%1").arg(i.next());

    detail->setText(detailText);
}

void MainWindow::changeToday(const QString date_str)
{
    today = QDate::fromString(date_str,"yyyy/M/d");
    setToday();
}

void MainWindow::addFood()
{
    FoodEditDialog* dialog = new FoodEditDialog(FoodEditDialog::AddMode, this);
    dialog->show();
}

void MainWindow::editFood()
{
    FoodEditDialog* dialog = new FoodEditDialog(FoodEditDialog::EditMode, this);
    dialog->show();
}

void MainWindow::deleteFood()
{
    QDomElement food = foodListElement.firstChildElement();
    for (int i=0;i<foodRow;i++) food = food.nextSiblingElement();
    foodListElement.removeChild(food);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)){
        QMessageBox::critical(this, "出错啦",
                                     QString("无法写入文件"));
        return;
    }
    QTextStream out(&file);
    mainDoc->save(out,QDomDocument::EncodingFromDocument);
    file.close();

    setToday();
    return;
}
