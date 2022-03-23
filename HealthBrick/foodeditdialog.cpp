#include "foodeditdialog.h"

#include <QGridLayout>
#include <QLabel>
#include <QFrame>

FoodEditDialog::FoodEditDialog()
{
    foodName = new QLineEdit(this);
    foodWeight = new QLineEdit(this);
    foodCarboCont = new QLineEdit(this);
    foodCarbo = new QLineEdit(this);
    foodProteinCont = new QLineEdit(this);
    foodProtein = new QLineEdit(this);
    foodFatCont = new QLineEdit(this);
    foodFat = new QLineEdit(this);

    QLabel* labelName = new QLabel("食物名",this);
    QLabel* labelWeight = new QLabel("净含量",this);
    QLabel* labelCarboCont = new QLabel("碳水含量",this);
    QLabel* labelCarbo = new QLabel("碳水",this);
    QLabel* labelProteinCont = new QLabel("蛋白质含量",this);
    QLabel* labelProtein = new QLabel("蛋白质",this);
    QLabel* labelFatCont = new QLabel("脂肪含量",this);
    QLabel* labelFat = new QLabel("脂肪",this);

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(labelName,0,0);
    layout->addWidget(foodName,0,1);

    layout->addWidget(labelWeight,0,2);
    layout->addWidget(foodWeight,0,3);

    QFrame* frame1 = new QFrame(this);
    frame1->setFrameShape(QFrame::HLine);
    layout->addWidget(frame1,1,0,1,4);

    layout->addWidget(labelCarboCont,2,0);
    layout->addWidget(foodCarboCont,2,1);
    layout->addWidget(labelCarbo,2,2);
    layout->addWidget(foodCarbo,2,3);

    layout->addWidget(labelProteinCont,3,0);
    layout->addWidget(foodProteinCont,3,1);
    layout->addWidget(labelProtein,3,2);
    layout->addWidget(foodProtein,3,3);

    layout->addWidget(labelFatCont,4,0);
    layout->addWidget(foodFatCont,4,1);
    layout->addWidget(labelFat,4,2);
    layout->addWidget(foodFat,4,3);
    this->setLayout(layout);
}
