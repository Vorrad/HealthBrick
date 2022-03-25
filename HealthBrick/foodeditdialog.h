#ifndef FOODEDITDIALOG_H
#define FOODEDITDIALOG_H

#include "mainwindow.h"

#include <QDialog>
#include <QLineEdit>
#include <QDomDocument>

class FoodEditDialog : public QDialog
{
    Q_OBJECT

public:
    enum EditMode{AddMode,EditMode};

    FoodEditDialog(enum EditMode mode,MainWindow* w);

    void addFood();

public slots:

    void confirmBtnClicked();

    void carboContSynchro();
    void carboSynchro();
    void proteinContSynchro();
    void proteinSynchro();
    void fatContSynchro();
    void fatSynchro();

private:

    enum EditMode dialogMode;
    MainWindow* parentWindow;

    QString filePath;
    QDomDocument* mainDoc;

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
