#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include "newformdialog.h"
#include "mapsyntaxhighlighter.h"
#include "setupscrollparamsdialog.h"
#include "newspritedialog.h"
#include "chooseform.h"
#include "defines.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

public slots:
    void rehighlightText();
private slots:
    void on_pushButton_clicked();


    void on_tabWidget_tabBarClicked(int index);

    void on_textEdit_textChanged();

    void on_pushButton_ssp_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_9_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_pushButton_27_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_25_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    QPushButton * findButton(QObject * parent, QString name);
    bool eventFilter(QObject *target, QEvent *event);
    QByteArray compileMap(QStringList scriptMap);
    QString decompileMap(QByteArray cMap);

    void setUpShortCats(int groupIndex);
    void removeShortCats(int groupIndex);
    MapSyntaxHighlighter * hl;
    QTimer * rehighlightTimer;

    LayoutDesc spriteLayout;
    bool isSpriteLayoutSetUp;
    LayoutDesc objectLayout;
    bool isObjectLayoutSetUp;
};





#endif // MAINWINDOW_H
