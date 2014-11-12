#ifndef CALLFUNCTIONDIALOG_H
#define CALLFUNCTIONDIALOG_H

#include <QDialog>
#include <QTextEdit>

namespace Ui {
class callFunctionDialog;
}

class callFunctionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit callFunctionDialog(QWidget *parent = 0);
    ~callFunctionDialog();
    void setFunctions(QString str);
    QString getFunction(){return selectedFunc;}
    bool checkDelay(){return isDelay;}
    QString getDelay(){return QString::number(delayTime);}

private slots:
    void on_callFunctionDialog_accepted();

    void on_callFunctionDialog_rejected();

    void on_checkBox_toggled(bool checked);

private:
    Ui::callFunctionDialog *ui;
    QString selectedFunc;
    bool isDelay;
    int delayTime;
};

#endif // CALLFUNCTIONDIALOG_H
