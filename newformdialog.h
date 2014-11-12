#ifndef NEWFORMDIALOG_H
#define NEWFORMDIALOG_H

#include <QDialog>

namespace Ui {
class newFormDialog;
}

class newFormDialog : public QDialog
{
    Q_OBJECT

public:
    explicit newFormDialog(QWidget *parent = 0);
    ~newFormDialog();
    QString getName();
    void setLabel(QString text);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::newFormDialog *ui;
    QString mapName;
};

#endif // NEWFORMDIALOG_H
