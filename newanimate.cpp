#include "newanimate.h"
#include "ui_newanimate.h"

newAnimate::newAnimate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newAnimate)
{
    ui->setupUi(this);
}

newAnimate::~newAnimate()
{
    delete ui;
}

bool newAnimate::isAnimateSprite()
{
    return findChild<QRadioButton*>("radioButton")->isChecked();
}

bool newAnimate::isOnStartCallback()
{
    return findChild<QCheckBox*>("checkBox")->isChecked();
}

bool newAnimate::isOnEndCallback()
{
    return findChild<QCheckBox*>("checkBox_2")->isChecked();
}

void newAnimate::setAnimateSprite(bool state)
{
    if (state)
        findChild<QRadioButton*>("radioButton")->setChecked(true);
    else
        findChild<QRadioButton*>("radioButton_2")->setChecked(true);
}

void newAnimate::updateState()
{
    QComboBox * cb = findChild<QComboBox*>("comboBox");
    cb->clear();
    if (findChild<QRadioButton*>("radioButton")->isChecked())
        cb->addItems(spriteList);
    else
        cb->addItems(objectList);
}

int newAnimate::getDL()
{
    return findChild<QLineEdit*>("lineEdit_2")->text().toInt();
}

int newAnimate::getDT()
{
    return findChild<QLineEdit*>("lineEdit_3")->text().toInt();
}

int newAnimate::getSpeed()
{
    return findChild<QLineEdit*>("lineEdit_4")->text().toInt();
}

QString newAnimate::getName()
{
    return findChild<QLineEdit*>("lineEdit")->text();
}

QString newAnimate::getObjectName()
{
    return findChild<QComboBox*>("comboBox")->currentText();
}

void newAnimate::setFunctionsList(QStringList funcList)
{
    QComboBox * cb1 = findChild<QComboBox*>("comboBox_2");
    QComboBox * cb2 = findChild<QComboBox*>("comboBox_3");
    cb1->addItems(funcList);
    cb2->addItems(funcList);
}

QString newAnimate::getOnStartCallback()
{
    return findChild<QComboBox*>("comboBox_2")->currentText();
}

QString newAnimate::getOnEndCallback()
{
    return findChild<QComboBox*>("comboBox_3")->currentText();
}

void newAnimate::on_buttonBox_accepted()
{
    setResult(1);
}

void newAnimate::on_buttonBox_rejected()
{
    setResult(0);
}

void newAnimate::on_radioButton_toggled(bool checked)
{
    if (checked)
        findChild<QLabel*>("label_2")->setText("Sprite to Animate: ");
    else
        findChild<QLabel*>("label_2")->setText("Object to Animate: ");
    updateState();
}
