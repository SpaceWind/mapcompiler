#include "callfunctiondialog.h"
#include "ui_callfunctiondialog.h"

callFunctionDialog::callFunctionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::callFunctionDialog)
{
    ui->setupUi(this);
    findChild<QComboBox*>("comboBox")->setFocus();
}

callFunctionDialog::~callFunctionDialog()
{
    delete ui;
}

void callFunctionDialog::setFunctions(QString str)
{
    QStringList strings = str.split("\n");
    int rootTableStart = strings.indexOf("@.");
    int rootTableEnd   = strings.indexOf("/.");
    for (int i=rootTableStart+1; i<rootTableEnd; ++i)
        findChild<QComboBox*>("comboBox")->addItem(strings.at(i));
}

void callFunctionDialog::on_callFunctionDialog_accepted()
{
    setResult(true);
    selectedFunc = findChild<QComboBox*>("comboBox")->currentText();
    delayTime = findChild<QLineEdit*>("lineEdit")->text().toInt();
}

void callFunctionDialog::on_callFunctionDialog_rejected()
{
    setResult(false);
    selectedFunc = "";
    delayTime = 0;
}

void callFunctionDialog::on_checkBox_toggled(bool checked)
{
    findChild<QLineEdit*>("lineEdit")->setEnabled(checked);
    isDelay = checked;
}
