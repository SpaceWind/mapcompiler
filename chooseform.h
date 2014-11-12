#ifndef CHOOSEFORM_H
#define CHOOSEFORM_H

#include <QDialog>

namespace Ui {
class ChooseForm;
}

class ChooseForm : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseForm(QWidget *parent = 0);
    ~ChooseForm();

    void setLabels(QString first, QString second);
    void setEditable (bool first, bool second);
    void setLinesEnabled(bool first, bool second, bool third=false,bool forth=false);

    void setupComboBoxes(QStringList first, QStringList second);
    void setCustomItems(bool val){isCustomItems = val;}
    void addCustomItems(QStringList items);

    QString getText(int index); //index [0; 3]

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::ChooseForm *ui;
    QList<QStringList> comboItems;
    bool isCustomItems;
};

#endif // CHOOSEFORM_H
