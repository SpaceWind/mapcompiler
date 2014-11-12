#ifndef NEWSPRITEDIALOG_H
#define NEWSPRITEDIALOG_H

#include <QDialog>
#include <QDir>
#include <QStringList>
#include "defines.h"

namespace Ui {
class newSpriteDialog;
}

struct spriteDesc;
struct LayoutDesc;
class newSpriteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit newSpriteDialog(QWidget *parent = 0);
    ~newSpriteDialog();

    LayoutDesc getLayout();
    bool checkLayout(){return isLayout;}
    bool checkMoveSprite(){return isMove;}
    QString getLeftPos();
    QString getTopPos();
    bool checkSetAnimation(){return isAnimation;}
    QString getAnimationName();
    QString getName();
    QString getType();
    void setLabelsForAddObject();
    static spriteDesc getSpriteDesc(QString type);


    void setValueList (QStringList vals);
    void setValueList (QHash<QString, QString> vals);
    void clearContent();
    void addNewObject(QString name, int width, int height, QStringList anims);

private slots:
    void on_sprite_types_currentTextChanged(const QString &arg1);

    void on_checkBox_toggled(bool checked);

    void on_checkBox_2_toggled(bool checked);

    void on_checkBox_3_toggled(bool checked);

    void on_buttonBox_accepted();

private:
    Ui::newSpriteDialog *ui;
    void loadSpriteList();
    void updateContent(QString sspriteType);

    QHash<QString,spriteDesc> sprites;
    QStringList valuesList;

    QStringList objectList;
    QStringList customAnimationList;
    bool isSpriteMode;

    bool isLayout, isMove, isAnimation;


};
struct spriteDesc
{
    QStringList animations;
    int w,h;
};
struct LayoutDesc
{
    QString type, namePrefix, indexValue;
    QString typePostFix;
    QString leftValue, topValue;
    QString leftShift, topShift;
    bool setAnimation;
};

#endif // NEWSPRITEDIALOG_H
