#include "foodeditdialog.h"

#include <QGridLayout>
#include <QLabel>
#include <QFrame>
#include <QPushButton>
#include <QDebug>
#include <QDomElement>
#include <QDate>
#include <QFile>
#include <QMessageBox>

FoodEditDialog::FoodEditDialog(enum EditMode mode, MainWindow* w)
{
    this->setWindowIcon(QIcon(":/icons/icon/番茄.svg"));
    this->setWindowTitle("HealthBrick - 添加食物信息");

    dialogMode = mode;
    parentWindow = w;
    filePath = w->getFilePath();
    mainDoc = w->getDoc();

    foodName = new QLineEdit(this);
    foodWeight = new QLineEdit(this);
    foodCarboCont = new QLineEdit(this);
    foodCarbo = new QLineEdit(this);
    foodProteinCont = new QLineEdit(this);
    foodProtein = new QLineEdit(this);
    foodFatCont = new QLineEdit(this);
    foodFat = new QLineEdit(this);

    connect(foodCarboCont,&QLineEdit::textChanged,this,&FoodEditDialog::carboContSynchro);
    connect(foodCarbo,&QLineEdit::textChanged,this,&FoodEditDialog::carboSynchro);
    connect(foodProteinCont,&QLineEdit::textChanged,this,&FoodEditDialog::proteinContSynchro);
    connect(foodProtein,&QLineEdit::textChanged,this,&FoodEditDialog::proteinSynchro);
    connect(foodFatCont,&QLineEdit::textChanged,this,&FoodEditDialog::fatContSynchro);
    connect(foodFat,&QLineEdit::textChanged,this,&FoodEditDialog::fatSynchro);

    QLabel* labelName = new QLabel("食物名",this);
    QLabel* labelWeight = new QLabel("净含量(g)",this);
    QLabel* labelCarboCont = new QLabel("碳水含量(g/100g)",this);
    QLabel* labelCarbo = new QLabel("碳水(g)",this);
    QLabel* labelProteinCont = new QLabel("蛋白质含量(g/100g)",this);
    QLabel* labelProtein = new QLabel("蛋白质(g)",this);
    QLabel* labelFatCont = new QLabel("脂肪含量(g/100g)",this);
    QLabel* labelFat = new QLabel("脂肪(g)",this);

    QGridLayout* textLayout = new QGridLayout;
    textLayout->addWidget(labelName,0,0);
    textLayout->addWidget(foodName,0,1);

    textLayout->addWidget(labelWeight,0,2);
    textLayout->addWidget(foodWeight,0,3);

    QFrame* frame1 = new QFrame(this);
    frame1->setFrameShape(QFrame::HLine);
    textLayout->addWidget(frame1,1,0,1,4);

    textLayout->addWidget(labelCarboCont,2,0);
    textLayout->addWidget(foodCarboCont,2,1);
    textLayout->addWidget(labelCarbo,2,2);
    textLayout->addWidget(foodCarbo,2,3);

    textLayout->addWidget(labelProteinCont,3,0);
    textLayout->addWidget(foodProteinCont,3,1);
    textLayout->addWidget(labelProtein,3,2);
    textLayout->addWidget(foodProtein,3,3);

    textLayout->addWidget(labelFatCont,4,0);
    textLayout->addWidget(foodFatCont,4,1);
    textLayout->addWidget(labelFat,4,2);
    textLayout->addWidget(foodFat,4,3);

    QGridLayout* btnLayout = new QGridLayout;
    QPushButton* confirmBtn = new QPushButton("确认",this);
    connect(confirmBtn,&QPushButton::clicked,this,&FoodEditDialog::confirmBtnClicked);
    QPushButton* cancelBtn = new QPushButton("取消",this);
    connect(cancelBtn,&QPushButton::clicked,this,&FoodEditDialog::close);
    btnLayout->addWidget(confirmBtn,0,0);
    btnLayout->addWidget(cancelBtn,0,1);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(textLayout);
    mainLayout->addLayout(btnLayout);

    this->setLayout(mainLayout);

    if (mode == FoodEditDialog::EditMode)
    {
        this->setWindowTitle("HealthBrick - 编辑食物信息");

        QDomElement foodlist = w->getFoodlistElement();
        QDomElement food = foodlist.firstChildElement();
        int row = w->getFoodRow();
        for (int i=0;i<row;i++) food = food.nextSiblingElement();

        // 填充表项
        QDomElement property = food.firstChildElement();
        foodName->setText(property.firstChild().toText().data());
        property = property.nextSiblingElement();
        foodWeight->setText(property.firstChild().toText().data());
        property = property.nextSiblingElement();
        foodCarboCont->setText(property.firstChild().toText().data());
        property = property.nextSiblingElement();
        foodCarbo->setText(property.firstChild().toText().data());
        property = property.nextSiblingElement();
        foodProteinCont->setText(property.firstChild().toText().data());
        property = property.nextSiblingElement();
        foodProtein->setText(property.firstChild().toText().data());
        property = property.nextSiblingElement();
        foodFatCont->setText(property.firstChild().toText().data());
        property = property.nextSiblingElement();
        foodFat->setText(property.firstChild().toText().data());
    }
}

void FoodEditDialog::addFood()
{
    QDate today = QDateTime::currentDateTime().date();

    QDomElement root = mainDoc->documentElement();    // <healthlog>
    QDomElement day = root.firstChildElement();         // <day>
    while (!day.isNull() && day.firstChildElement().firstChild().toText().data()!=
           QString("%1/%2/%3").arg(today.year()).arg(today.month()).arg(today.day()))
    {
        day = day.nextSiblingElement();
    }
    if (day.isNull())  // 如果没有当前日期
    {
        QDomElement newDay = mainDoc->createElement("day");
        root.appendChild(newDay);

        QDomElement newDate = mainDoc->createElement("date");
        QDomText dateText = mainDoc->createTextNode(QString("%1/%2/%3").arg(today.year()).arg(today.month()).arg(today.day()));
        newDate.appendChild(dateText);
        newDay.appendChild(newDate);

        QDomElement newFoodlist = mainDoc->createElement("food_list");
        newDay.appendChild(newFoodlist);

        QDomElement newTotal = mainDoc->createElement("total_count");
        newDay.appendChild(newTotal);

        QDomElement foodlistRoot = newFoodlist;

        QDomElement newFood = mainDoc->createElement("food");
        foodlistRoot.appendChild(newFood);

        QDomElement newName = mainDoc->createElement("name");
        QDomElement newWeight = mainDoc->createElement("weight");
        QDomElement newCarbo_cont = mainDoc->createElement("carbo_cont");
        QDomElement newCarbo = mainDoc->createElement("carbo");
        QDomElement newProtein_cont = mainDoc->createElement("protein_cont");
        QDomElement newProtein = mainDoc->createElement("protein");
        QDomElement newFat_cont = mainDoc->createElement("fat_cont");
        QDomElement newFat = mainDoc->createElement("fat");

        QDomText nameText = mainDoc->createTextNode(foodName->text());
        QDomText weightText = mainDoc->createTextNode(foodWeight->text());
        QDomText carbo_contText = mainDoc->createTextNode(foodCarboCont->text());
        QDomText carboText = mainDoc->createTextNode(foodCarbo->text());
        QDomText protein_contText = mainDoc->createTextNode(foodProteinCont->text());
        QDomText proteinText = mainDoc->createTextNode(foodProtein->text());
        QDomText fat_contText = mainDoc->createTextNode(foodFatCont->text());
        QDomText fatText = mainDoc->createTextNode(foodFat->text());

        newName.appendChild(nameText);
        newWeight.appendChild(weightText);
        newCarbo_cont.appendChild(carbo_contText);
        newCarbo.appendChild(carboText);
        newProtein_cont.appendChild(protein_contText);
        newProtein.appendChild(proteinText);
        newFat_cont.appendChild(fat_contText);
        newFat.appendChild(fatText);

        newFood.appendChild(newName);
        newFood.appendChild(newWeight);
        newFood.appendChild(newCarbo_cont);
        newFood.appendChild(newCarbo);
        newFood.appendChild(newProtein_cont);
        newFood.appendChild(newProtein);
        newFood.appendChild(newFat_cont);
        newFood.appendChild(newFat);

        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly)){
            QMessageBox::critical(this, "出错啦",
                                         QString("无法写入文件"));
            return;
        }
        QTextStream out(&file);
        mainDoc->save(out,QDomDocument::EncodingFromDocument);
        file.close();

        this->close();
        return;
    }
    else
    {
        QDomElement foodlist = day.firstChildElement().nextSiblingElement();

        QDomElement newFood = mainDoc->createElement("food");
        foodlist.appendChild(newFood);

        QDomElement newName = mainDoc->createElement("name");
        QDomElement newWeight = mainDoc->createElement("weight");
        QDomElement newCarbo_cont = mainDoc->createElement("carbo_cont");
        QDomElement newCarbo = mainDoc->createElement("carbo");
        QDomElement newProtein_cont = mainDoc->createElement("protein_cont");
        QDomElement newProtein = mainDoc->createElement("protein");
        QDomElement newFat_cont = mainDoc->createElement("fat_cont");
        QDomElement newFat = mainDoc->createElement("fat");

        QDomText nameText = mainDoc->createTextNode(foodName->text());
        QDomText weightText = mainDoc->createTextNode(foodWeight->text());
        QDomText carbo_contText = mainDoc->createTextNode(foodCarboCont->text());
        QDomText carboText = mainDoc->createTextNode(foodCarbo->text());
        QDomText protein_contText = mainDoc->createTextNode(foodProteinCont->text());
        QDomText proteinText = mainDoc->createTextNode(foodProtein->text());
        QDomText fat_contText = mainDoc->createTextNode(foodFatCont->text());
        QDomText fatText = mainDoc->createTextNode(foodFat->text());

        newName.appendChild(nameText);
        newWeight.appendChild(weightText);
        newCarbo_cont.appendChild(carbo_contText);
        newCarbo.appendChild(carboText);
        newProtein_cont.appendChild(protein_contText);
        newProtein.appendChild(proteinText);
        newFat_cont.appendChild(fat_contText);
        newFat.appendChild(fatText);

        newFood.appendChild(newName);
        newFood.appendChild(newWeight);
        newFood.appendChild(newCarbo_cont);
        newFood.appendChild(newCarbo);
        newFood.appendChild(newProtein_cont);
        newFood.appendChild(newProtein);
        newFood.appendChild(newFat_cont);
        newFood.appendChild(newFat);


        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly)){
            QMessageBox::critical(this, "出错啦",
                                         QString("无法写入文件"));
            return;
        }
        QTextStream out(&file);
        mainDoc->save(out,QDomDocument::EncodingFromDocument);
        file.close();

        this->close();
        return;
    }

}

void FoodEditDialog::confirmBtnClicked()
{
    if (dialogMode == FoodEditDialog::AddMode)
    {
        this->setWindowTitle("HealthBrick - 添加食物");
        if (foodName->text()=="")
        {
            QMessageBox::critical(this,"出错啦","食物名不能为空");
            return;
        }
        if (foodWeight->text().toDouble()==0.0)
        {
            QMessageBox::critical(this,"出错啦","食物净含量不能为0");
            return;
        }
        addFood();
        parentWindow->setToday();
    }
    else if (dialogMode == FoodEditDialog::EditMode)
    {
        this->setWindowTitle("HealthBrick - 编辑食物信息");

        if (foodName->text()=="")
        {
            QMessageBox::critical(this,"出错啦","食物名不能为空");
            return;
        }
        if (foodWeight->text().toDouble()==0.0)
        {
            QMessageBox::critical(this,"出错啦","食物净含量不能为0");
            return;
        }
        parentWindow->deleteFood();
        addFood();
        parentWindow->setToday();
    }
}

void FoodEditDialog::carboContSynchro()
{
    double weight = foodWeight->text().toDouble();
    Q_ASSERT(weight>0);
    double carbo = foodCarboCont->text().toDouble()*weight/100;
    foodCarbo->setText(QString::number(carbo));
    return;
}

void FoodEditDialog::carboSynchro()
{
    double weight = foodWeight->text().toDouble();
    Q_ASSERT(weight>0);
    double carboCont = foodCarbo->text().toDouble()/weight*100;
    foodCarboCont->setText(QString::number(carboCont));
    return;
}

void FoodEditDialog::proteinContSynchro()
{
    double weight = foodWeight->text().toDouble();
    Q_ASSERT(weight>0);
    double protein = foodProteinCont->text().toDouble()*weight/100;
    foodProtein->setText(QString::number(protein));
    return;
}

void FoodEditDialog::proteinSynchro()
{
    double weight = foodWeight->text().toDouble();
    Q_ASSERT(weight>0);
    double proteinCont = foodProtein->text().toDouble()/weight*100;
    foodProteinCont->setText(QString::number(proteinCont));
    return;
}

void FoodEditDialog::fatContSynchro()
{
    double weight = foodWeight->text().toDouble();
    Q_ASSERT(weight>0);
    double fat = foodFatCont->text().toDouble()*weight/100;
    foodFat->setText(QString::number(fat));
    return;
}

void FoodEditDialog::fatSynchro()
{
    double weight = foodWeight->text().toDouble();
    Q_ASSERT(weight>0);
    double fatCont = foodFat->text().toDouble()/weight*100;
    foodFatCont->setText(QString::number(fatCont));
    return;
}
