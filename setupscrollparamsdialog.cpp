#include "setupscrollparamsdialog.h"
#include "ui_setupscrollparamsdialog.h"

SetupScrollParamsDialog::SetupScrollParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupScrollParamsDialog)
{
    ui->setupUi(this);
    findChild<QCheckBox*>("enableHScrollingCheckBox")->setFocus();
}

SetupScrollParamsDialog::~SetupScrollParamsDialog()
{
    delete ui;
}

void SetupScrollParamsDialog::init(bool enableHScrolling_, bool enableVScrolling_, bool enableSmoothScrolling_, bool enableStartScrolling_,
                                   int internalLeft, int internalTop, int internalRight, int internalBottom,
                                   int externalLeft, int externalTop, int externalRight, int externalBottom,
                                   int startLeft_, int startTop_, int smoothH_, int smoothV_)
{
    enableHScrolling = enableHScrolling_;
    this->findChild<QCheckBox*>("enableHScrollingCheckBox")->setChecked(enableHScrolling_);
    enableVScrolling = enableVScrolling_;
    this->findChild<QCheckBox*>("enableVScrollingCheckBox")->setChecked(enableVScrolling_);
    enableSmoothScrolling = enableSmoothScrolling_;
    this->findChild<QCheckBox*>("enableSmoothScrollingCheckBox")->setChecked(enableSmoothScrolling_);
    enableStartScrolling = enableStartScrolling_;
    this->findChild<QCheckBox*>("enableStartScrollingCheckBox")->setChecked(enableStartScrolling_);

    internalRect.setLeft(internalLeft);
    internalRect.setRight(internalRight);
    internalRect.setTop(internalTop);
    internalRect.setBottom(internalBottom);

    findChild<QLineEdit*>("leftBound")->setText(QString::number(internalLeft));
    findChild<QLineEdit*>("topBound")->setText(QString::number(internalTop));
    findChild<QLineEdit*>("rightBound")->setText(QString::number(internalRight));
    findChild<QLineEdit*>("bottomBound")->setText(QString::number(internalBottom));

    externalRect.setLeft(externalLeft);
    externalRect.setRight(externalRight);
    externalRect.setTop(externalTop);
    externalRect.setBottom(externalBottom);

    findChild<QLineEdit*>("minLeft")->setText(QString::number(externalLeft));
    findChild<QLineEdit*>("minTop")->setText(QString::number(externalTop));
    findChild<QLineEdit*>("maxLeft")->setText(QString::number(externalRight));
    findChild<QLineEdit*>("maxTop")->setText(QString::number(externalBottom));

    smoothH = smoothH_;
    smoothV_ = smoothV_;
    findChild<QLineEdit*>("hSmooth")->setText(QString::number(smoothH));
    findChild<QLineEdit*>("vSmooth")->setText(QString::number(smoothV));

    startLeft = startLeft_;
    startTop = startTop_;
    findChild<QLineEdit*>("startLeft")->setText(QString::number(startLeft));
    findChild<QLineEdit*>("startTop")->setText(QString::number(startTop));





}

void SetupScrollParamsDialog::on_buttonBox_accepted()
{
    QCheckBox * enableHScrollingCheckBox        = this->findChild<QCheckBox*>("enableHScrollingCheckBox");
    QCheckBox * enableVScrollingCheckBox        = this->findChild<QCheckBox*>("enableVScrollingCheckBox");
    QCheckBox * enableSmoothScrollingCheckBox   = this->findChild<QCheckBox*>("enableSmoothScrollingCheckBox");
    QCheckBox * enableStartScrollingCheckBox    = this->findChild<QCheckBox*>("enableStartScrollingCheckBox");

    enableHScrolling      = enableHScrollingCheckBox->isChecked();
    enableVScrolling      = enableVScrollingCheckBox->isChecked();
    enableSmoothScrolling = enableSmoothScrollingCheckBox->isChecked();
    enableStartScrolling  = enableStartScrollingCheckBox->isChecked();

    if (enableHScrolling || enableVScrolling)
    {
        internalRect.setLeft   (findChild<QLineEdit*>("leftBound")->text().toInt());
        internalRect.setTop    (findChild<QLineEdit*>("topBound")->text().toInt());
        internalRect.setRight  (findChild<QLineEdit*>("rightBound")->text().toInt());
        internalRect.setBottom (findChild<QLineEdit*>("bottomBound")->text().toInt());

        externalRect.setLeft   (findChild<QLineEdit*>("minLeft")->text().toInt());
        externalRect.setTop    (findChild<QLineEdit*>("minTop")->text().toInt());
        externalRect.setRight  (findChild<QLineEdit*>("maxLeft")->text().toInt());
        externalRect.setBottom (findChild<QLineEdit*>("maxTop")->text().toInt());
    }

    if (enableSmoothScrolling)
    {
        smoothH = findChild<QLineEdit*>("hSmooth")->text().toInt();
        smoothV = findChild<QLineEdit*>("vSmooth")->text().toInt();
    }
    if (enableStartScrolling)
    {
        startLeft = findChild<QLineEdit*>("startLeft")->text().toInt();
        startTop  = findChild<QLineEdit*>("startTop")->text().toInt();
    }
    setResult(true);
}

void SetupScrollParamsDialog::on_buttonBox_rejected()
{
    setResult(false);
}
