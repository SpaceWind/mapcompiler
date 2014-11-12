#include "newspritedialog.h"
#include "ui_newspritedialog.h"

newSpriteDialog::newSpriteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newSpriteDialog)
{
    ui->setupUi(this);
    loadSpriteList();
    findChild<QGroupBox*>("groupBox")->setEnabled(false);
    findChild<QGroupBox*>("groupBox_2")->setEnabled(false);
    findChild<QGroupBox*>("groupBox_3")->setEnabled(false);
    this->isMove = this->isAnimation = this->isLayout = false;
}

newSpriteDialog::~newSpriteDialog()
{
    delete ui;
}

LayoutDesc newSpriteDialog::getLayout()
{
    LayoutDesc ld;
    if (checkLayout())
    {
            ld.namePrefix = findChild<QLineEdit*>("lineEdit_5")->text();
            ld.indexValue = ld.namePrefix+"_indexer";
            ld.leftValue  = findChild<QComboBox*>("comboBox")->currentText();
            ld.topValue   = findChild<QComboBox*>("comboBox_2")->currentText();
            ld.leftShift  = findChild<QLineEdit*>("left_shift")->text();
            ld.topShift  = findChild<QLineEdit*>("top_shift")->text();
            ld.setAnimation = findChild<QCheckBox*>("checkBox_3")->isChecked();
            ld.type = findChild<QComboBox*>("sprite_types")->currentText();
            ld.typePostFix = "";
    }
    return ld;
}

QString newSpriteDialog::getLeftPos()
{
    return QString::number(findChild<QLineEdit*>("lineEdit_4")->text().toInt());
}

QString newSpriteDialog::getTopPos()
{
    return QString::number(findChild<QLineEdit*>("lineEdit_3")->text().toInt());
}

QString newSpriteDialog::getAnimationName()
{
    return findChild<QComboBox*>("animations")->currentText();
}

QString newSpriteDialog::getName()
{
    return findChild<QLineEdit*>("lineEdit_5")->text();
}

QString newSpriteDialog::getType()
{
    return findChild<QComboBox*>("sprite_types")->currentText();
}

void newSpriteDialog::setLabelsForAddObject()
{
    findChild<QCheckBox*>("checkBox")->setText("Create Layout For Object");
    findChild<QCheckBox*>("checkBox_2")->setText("Move Object");
    findChild<QGroupBox*>("groupBox")->setTitle("Object Layout");
    findChild<QLabel*>("label_8")->setText("Object Type");
    findChild<QLabel*>("label_9")->setText("Object Name");
}

spriteDesc newSpriteDialog::getSpriteDesc(QString type)
{
    spriteDesc result;
    QDir dir;
    dir.setPath("data/sprites/");
    QStringList filters;
    filters << "*.sprite";
    dir.setNameFilters(filters);
    QStringList files = dir.entryList();
    for (int i=0; i<files.count(); i++)
    {
        QString str = files.at(i);
        Defines d;
        d.Load("data/sprites/"+str);
        if (type == d.Get("registername"))
        {
            result.h = d.Get("h").toInt();
            result.w = d.Get("w").toInt();
            result.animations = d.GetList("animationlist");
            return result;
        }
    }
    return result;
}

void newSpriteDialog::setValueList(QStringList vals)
{
    foreach (const QString& str, vals)
    {
        findChild<QComboBox*>("comboBox")->addItem(str);
        findChild<QComboBox*>("comboBox_2")->addItem(str);
    }
}

void newSpriteDialog::setValueList(QHash<QString, QString> vals)
{
    QHash<QString, QString>::const_iterator i = vals.constBegin();
    while (i != vals.constEnd())
    {
        findChild<QComboBox*>("comboBox")->addItem(i.key());
        findChild<QComboBox*>("comboBox_2")->addItem(i.key());
    }
}

void newSpriteDialog::clearContent()
{
    sprites.clear();
    findChild<QComboBox*>("sprite_types")->clear();
    QComboBox *animationCB = findChild<QComboBox*>("animations");
    animationCB->clear();

}

void newSpriteDialog::addNewObject(QString name, int width, int height, QStringList anims)
{
    spriteDesc sd;
    sd.animations= anims;
    sd.w = width;
    sd.h = height;
    sprites[name] = sd;
    updateContent(name);
    findChild<QComboBox*>("sprite_types")->addItem(name);
}

void newSpriteDialog::loadSpriteList()
{
    QDir dir;
    dir.setPath("data/sprites/");
    QStringList filters;
    filters << "*.sprite";
    dir.setNameFilters(filters);
    QStringList files = dir.entryList();
    for (int i=0; i<files.count(); i++)
    {
        QString str = files.at(i);
        Defines d;
        d.Load("data/sprites/"+str);
        spriteDesc desc;
        desc.h = d.Get("h").toInt();
        desc.w = d.Get("w").toInt();
        desc.animations = d.GetList("animationlist");
        sprites[d.Get("registername")] = desc;
        findChild<QComboBox*>("sprite_types")->addItem(d.Get("registername"));
    }
}

void newSpriteDialog::updateContent(QString sspriteType)
{
    QComboBox *animationCB = findChild<QComboBox*>("animations");
    if (sprites.contains(sspriteType))
    {
        animationCB->clear();
        for (int i=0; i<sprites[sspriteType].animations.count(); i++)
        {
            QString str = sprites[sspriteType].animations.at(i);
            animationCB->addItem(str);
        }
        findChild<QLineEdit*>("left_shift")->setText(QString::number(sprites[sspriteType].w));
        findChild<QLineEdit*>("top_shift")->setText(QString::number(sprites[sspriteType].h));

        animationCB->addItem("!start");
        animationCB->addItem("!stop");
    }
}

void newSpriteDialog::on_sprite_types_currentTextChanged(const QString &arg1)
{
    updateContent(arg1);
}

void newSpriteDialog::on_checkBox_toggled(bool checked)
{
    findChild<QGroupBox*>("groupBox")->setEnabled(checked);
    isLayout = checked;
}

void newSpriteDialog::on_checkBox_2_toggled(bool checked)
{
    findChild<QGroupBox*>("groupBox_2")->setEnabled(checked);
    isMove = checked;
}

void newSpriteDialog::on_checkBox_3_toggled(bool checked)
{
    findChild<QGroupBox*>("groupBox_3")->setEnabled(checked);
    isAnimation = checked;
}

void newSpriteDialog::on_buttonBox_accepted()
{
    setResult(true);
}
