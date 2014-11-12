#include "newformdialog.h"
#include "ui_newformdialog.h"

newFormDialog::newFormDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newFormDialog)
{
    ui->setupUi(this);
    QLineEdit * line = this->findChild<QLineEdit*>("lineEdit");
    line->setFocus();
}

newFormDialog::~newFormDialog()
{
    delete ui;
}

QString newFormDialog::getName()
{
    return mapName;
}

void newFormDialog::setLabel(QString text)
{
    QLabel * line = this->findChild<QLabel*>("label");
    this->setWindowTitle("New Function dialog");
    line->setText(text);
}

void newFormDialog::on_buttonBox_accepted()
{
    QLineEdit * line = this->findChild<QLineEdit*>("lineEdit");
    mapName = line->text();
    this->setResult(true);
}

void newFormDialog::on_buttonBox_rejected()
{
    mapName = "";
    setResult(false);
}
