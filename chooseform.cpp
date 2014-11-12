#include "chooseform.h"
#include "ui_chooseform.h"

ChooseForm::ChooseForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseForm)
{
    ui->setupUi(this);
    isCustomItems = false;
}

ChooseForm::~ChooseForm()
{
    delete ui;
}

void ChooseForm::setLabels(QString first, QString second)
{
    findChild<QLabel*>("label")->setText(first);
    findChild<QLabel*>("label_2")->setText(second);
}

void ChooseForm::setEditable(bool first, bool second)
{
    findChild<QComboBox*>("comboBox")->setEnabled(first);
    findChild<QComboBox*>("comboBox_2")->setEnabled(second);
}

void ChooseForm::setLinesEnabled(bool first, bool second, bool third, bool forth)
{
    findChild<QLineEdit*>("lineEdit")->setVisible(first);
    findChild<QLineEdit*>("lineEdit_2")->setVisible(second);
    findChild<QLineEdit*>("lineEdit_3")->setVisible(third);
    findChild<QLineEdit*>("lineEdit_4")->setVisible(forth);

}

void ChooseForm::setupComboBoxes(QStringList first, QStringList second)
{
    QComboBox * cb = findChild<QComboBox*>("comboBox");
    cb->addItems(first);
    cb = findChild<QComboBox*>("comboBox_2");
    cb->addItems(second);
}

void ChooseForm::addCustomItems(QStringList items)
{
    comboItems.append(items);
}

QString ChooseForm::getText(int index)
{
    switch (index)
    {
    case 0:
        return findChild<QComboBox*>("comboBox")->currentText();
        break;
    case 1:
        return findChild<QComboBox*>("comboBox_2")->currentText();
        break;
    case 2:
        return findChild<QLineEdit*>("lineEdit")->text();
        break;
    case 3:
        return findChild<QLineEdit*>("lineEdit_2")->text();
        break;
    case 4:
        return findChild<QLineEdit*>("lineEdit_3")->text();
        break;
    case 5:
        return findChild<QLineEdit*>("lineEdit_4")->text();
        break;
    default:
        return "";
        break;
    }
}

void ChooseForm::on_buttonBox_accepted()
{
    setResult(1);
}

void ChooseForm::on_buttonBox_rejected()
{
    setResult(0);
}

void ChooseForm::on_comboBox_currentIndexChanged(int index)
{
    if (isCustomItems)
    {
        QComboBox* cb = findChild<QComboBox*>("comboBox_2");
        cb->clear();
        if (comboItems.count()>index)
            cb->addItems(comboItems.at(index));
    }
}
