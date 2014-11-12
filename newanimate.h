#ifndef NEWANIMATE_H
#define NEWANIMATE_H

#include <QDialog>

namespace Ui {
class newAnimate;
}

class newAnimate : public QDialog
{
    Q_OBJECT

public:
    explicit newAnimate(QWidget *parent = 0);
    ~newAnimate();
    bool isAnimateSprite();
    bool isOnStartCallback();
    bool isOnEndCallback();
    void setAnimateSprite(bool state);
    void updateState();
    int getDL();
    int getDT();
    int getSpeed();
    QString getName();
    QString getObjectName();
    void setLists(QStringList sprList, QStringList objList){spriteList = sprList; objectList = objList; updateState();}
    void setFunctionsList(QStringList funcList);
    QString getOnStartCallback();
    QString getOnEndCallback();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_radioButton_toggled(bool checked);

private:
    Ui::newAnimate *ui;
    QStringList spriteList;
    QStringList objectList;
};

#endif // NEWANIMATE_H
