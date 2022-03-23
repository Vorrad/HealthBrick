#ifndef FOODEDITDIALOG_H
#define FOODEDITDIALOG_H

#include <QDialog>
#include <QLineEdit>

class FoodEditDialog : public QDialog
{
    Q_OBJECT
public:
    FoodEditDialog();

private:
    QLineEdit* foodName;
    QLineEdit* foodWeight;
    QLineEdit* foodCarboCont;
    QLineEdit* foodProteinCont;
    QLineEdit* foodFatCont;
    QLineEdit* foodCarbo;
    QLineEdit* foodProtein;
    QLineEdit* foodFat;
};

#endif // FOODEDITDIALOG_H
