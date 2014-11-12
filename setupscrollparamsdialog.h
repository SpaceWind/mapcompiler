#ifndef SETUPSCROLLPARAMSDIALOG_H
#define SETUPSCROLLPARAMSDIALOG_H

#include <QDialog>
#include <QRect>

namespace Ui {
class SetupScrollParamsDialog;
}

class SetupScrollParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetupScrollParamsDialog(QWidget *parent = 0);
    ~SetupScrollParamsDialog();

    void init(bool enableHScrolling_, bool enableVScrolling_, bool enableSmoothScrolling_, bool enableStartScrolling_,
              int internalLeft, int internalTop, int internalRight, int internalBottom,
              int externalLeft, int externalTop, int externalRight, int externalBottom,
              int startLeft, int startTop, int smoothH, int smoothV);


    int getSmoothH(){return smoothH;}
    int getSmoothV(){return smoothV;}
    int getStartH(){return startLeft;}
    int getStartV(){return startTop;}
    QRect getInternalRect(){return internalRect;}
    QRect getExternalRect(){return externalRect;}
    bool isHScrollEnabled() {return enableHScrolling;}
    bool isVScrollEnabled() {return enableVScrolling;}
    bool isSmoothScrollingEnabled() {return enableSmoothScrolling;}
    bool isStartScrollingEnabled()  {return enableStartScrolling;}


private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::SetupScrollParamsDialog *ui;
    QRect internalRect;
    QRect externalRect;
    int smoothH, smoothV, startLeft, startTop;
    bool enableHScrolling, enableVScrolling, enableSmoothScrolling, enableStartScrolling;
};

#endif // SETUPSCROLLPARAMSDIALOG_H
