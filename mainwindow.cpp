#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "callfunctiondialog.h"
#include "newanimate.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();
    QTextEdit * txt = this->findChild<QTextEdit*>("textEdit");
    hl = new MapSyntaxHighlighter(txt);
    txt->setFocus();
    rehighlightTimer = new QTimer(this);
    connect(rehighlightTimer, SIGNAL(timeout()),SLOT(rehighlightText()));
    rehighlightTimer->start(10000);
    txt->installEventFilter(this);
    setUpShortCats(0);

}

/*
 *  my change.
 */
bool MainWindow::eventFilter(QObject *target, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QTabWidget * tab = this->findChild<QTabWidget*>("tabWidget");
        int key = ((QKeyEvent*)event)->key();
        int isCTRL = (QApplication::keyboardModifiers() & Qt::ControlModifier);
        if ((key == Qt::Key_Space) && isCTRL)
        {
            hl->rehighlight();
            return true;
        }
        else if ((key == Qt::Key_1) && isCTRL)
        {
            tab->setCurrentIndex(0);
            return true;
        }
        else if ((key == Qt::Key_2) && isCTRL)
        {
            tab->setCurrentIndex(1);
            return true;
        }
        else if ((key == Qt::Key_3) && isCTRL)
        {
            tab->setCurrentIndex(2);
            return true;
        }
        else if ((key == Qt::Key_4) && isCTRL)
        {
            tab->setCurrentIndex(3);
            return true;

        }
        else if ((key == Qt::Key_5) && isCTRL)
        {
            tab->setCurrentIndex(4);
            return true;
        }
    }
    return false;
}

QByteArray MainWindow::compileMap(QStringList scriptMap)
{
    QByteArray wholeMap;
    QByteArray header;
    QList<QByteArray> cFunctions;
    QHash<QString,char> funcMap;
    QHash<QString, QString> vars;
    QHash<QString, int> spriteMap;
    QHash<QString, int> objectMap;
    int cSpriteId = 0;
    int cObjectId = 0;
    /*
     * 0 = find Defines;
     * 1 = in functionList;
     * 2 = in function
     */
    Defines d;
    d.LoadFromString(scriptMap.join("\n"));
    QStringList functions = d.GetList(".");
    foreach(const QString& fName,functions)
    {
        QStringList commands = d.GetList("@"+fName);
        foreach(const QString& str, commands)
        {
            QString trStr = hl->translateString(str,vars);
            QStringList params = trStr.split(" ");
            QByteArray cFunc;
            if (params.at(0).toLower() == "addsprite")
            {
                cFunc.append(char(0x01));
                cFunc.append(char(params.at(1).length()));
                cFunc.append(params.at(1).toStdString().c_str(),params.at(1).length());
                spriteMap[params.at(2)] = cSpriteId++;
                char id[sizeof(cSpriteId)];
                memcpy(id,&cSpriteId,sizeof(cSpriteId));
                cFunc.append(id,sizeof(cSpriteId));
            }
            else if (params.at(0).toLower() == "movesprite")
            {
                cFunc.append(char(0x02));
                char id[sizeof(int)];
                memcpy(id,&spriteMap[params.at(1)],sizeof(int));
                cFunc.append(id,sizeof(int));

                char leftN[sizeof(int)];
                int nLeft = params.at(1).toInt();
                memcpy(leftN, &nLeft,sizeof(int));

                char topN[sizeof(int)];
                int nTop = params.at(2).toInt();
                memcpy(topN, &nTop,sizeof(int));

                cFunc.append(leftN,sizeof(int));
                cFunc.append(topN, sizeof(int));
            }
            else if (params.at(0).toLower() == "setanimation")
            {
                cFunc.append(char(0x03));
                char id[sizeof(int)];
                memcpy(id,&spriteMap[params.at(1)],sizeof(int));
                cFunc.append(id,sizeof(int));
                if (params.at(2) == "!start")
                    cFunc.append(char(0xFF));
                else if (params.at(2) == "!stop")
                    cFunc.append(char(0x00));
                else
                {
                    cFunc.append(char(params.at(2).length()));
                    char anName [255];
                    memcpy(anName,params.at(2).toStdString().c_str(),params.at(2).length());
                    cFunc.append(anName,params.at(2).length());
                }
            }
            else if (params.at(0).indexOf(":") == 0)
            {
                cFunc.append(char(0x04));
                QString fName = params.at(0).mid(1,params.at(0).length()-1);
                char cfName [255];
                memcpy(cfName,fName.toStdString().c_str(),fName.length());
                cFunc.append(cfName,fName.length());
            }
            else if (params.at(0).toLower().indexOf("animation.") == 0)
            {
                QStringList spParams = params.at(0).split(".");
                int sprId = spriteMap[spParams.at(1)];
                char c_sprId[sizeof(int)];
                memcpy(c_sprId, &sprId, sizeof(int));
                char operation = (spParams.at(2).toLower() == "onstart") ?
                                  0x00 : 0x01;
                QString fName = params.at(1).mid(1,params.at(0).length()-1);
                char cfName [255];
                memcpy(cfName,fName.toStdString().c_str(),fName.length());
                cFunc.append(char(0x07));
                cFunc.append(c_sprId, sizeof(int));
                cFunc.append(operation);
                cFunc.append(cfName,fName.length());
            }
            else if (params.at(0).toLower() == "waitcall")
            {
                cFunc.append(char(0x08));
                char c_t[sizeof(int)];
                int n_c_t = params.at(1).toInt();
                memcpy(c_t,&n_c_t,sizeof(int));
                cFunc.append(c_t,sizeof(int));
                QString fName = params.at(2).mid(1,params.at(0).length()-1);
                char cfName [255];
                memcpy(cfName,fName.toStdString().c_str(),fName.length());
                cFunc.append(cfName,fName.length());
            }
            else if (params.at(0).toLower() == "removesprite")
            {
                cFunc.append(char(0x09));
                int sprId = spriteMap[params.at(1)];
                char c_sprId[sizeof(int)];
                memcpy(c_sprId, &sprId, sizeof(int));
                spriteMap.remove(params.at(1));
                cFunc.append(c_sprId,sizeof(int));
            }
            else if (params.at(0).toLower() == "playsound" ||
                     params.at(0).toLower() == "playsoundand")
            {
                cFunc.append(char(0x09));
                char namesize = params.at(1).length();
                cFunc.append(namesize);
                char name[255];
                memset(name,0,255);
                memcpy(name,params.at(1).toStdString().c_str(),params.at(1).length());
                cFunc.append(name,params.at(1).length());
                if (params.count() == 3)
                {
                    QString thirdParam = params.at(2);
                    thirdParam = thirdParam.replace(":","");
                    QString funcToCall = thirdParam;
                    namesize = funcToCall.length();
                    memset(name,0,255);
                    cFunc.append(namesize);
                    memcpy(name,funcToCall.toStdString().c_str(),funcToCall.length());
                    cFunc.append(name,funcToCall.length());
                }
                else
                {
                    namesize = 0;
                    cFunc.append(namesize);
                }
            }
            else if (params.at(0).toLower() == "addobject")
            {
                cFunc.append(char(0x51));
                objectMap[params.at(1)] = cObjectId++;
                int objId = objectMap[params.at(1)];
                char c_objId[sizeof(int)];
                memcpy(c_objId, &objId, sizeof(int));
                cFunc.append(c_objId,sizeof(int));
                QString builtString = "";
                for (int i=2; i<params.count(); i++)
                {
                    builtString +=params.at(i);
                    if (i!=params.count()-1)
                        builtString += " ";
                }
                char namesize = builtString.length();
                char name[256];
                memset(name,0,256);
                memcpy(name,builtString.toStdString().c_str(),builtString.length());
                cFunc.append(name,builtString.length());
            }
            else if (params.at(0).toLower() == "moveobject")
            {
                cFunc.append(char(0x52));
                char id[sizeof(int)];
                memcpy(id,&objectMap[params.at(1)],sizeof(int));
                cFunc.append(id,sizeof(int));

                char leftN[sizeof(int)];
                int nLeft = params.at(1).toInt();
                memcpy(leftN, &nLeft,sizeof(int));

                char topN[sizeof(int)];
                int nTop = params.at(2).toInt();
                memcpy(topN, &nTop,sizeof(int));

                cFunc.append(leftN,sizeof(int));
                cFunc.append(topN, sizeof(int));
            }
            else if (params.at(0).toLower() == "setobjectanimation")
            {
                cFunc.append(char(0x53));
                char id[sizeof(int)];
                memcpy(id,&objectMap[params.at(1)],sizeof(int));
                cFunc.append(id,sizeof(int));
                if (params.at(2) == "!start")
                    cFunc.append(char(0xFF));
                else if (params.at(2) == "!stop")
                    cFunc.append(char(0x00));
                else
                {
                    cFunc.append(char(params.at(2).length()));
                    char anName [255];
                    memcpy(anName,params.at(2).toStdString().c_str(),params.at(2).length());
                    cFunc.append(anName,params.at(2).length());
                }
            }
            else if (params.at(0).toLower() == "animate" ||
                     params.at(0).toLower() == "animateobject")
            {
                bool spriteAnim = params.at(0).toLower() == "animate";
                cFunc.append(char(spriteAnim?0x0B:0x54));
                char id[sizeof(int)];
                memcpy(id,&((spriteAnim?spriteMap:objectMap)[params.at(1)]),sizeof(int));
                cFunc.append(id,sizeof(int));

                char leftN[sizeof(int)];
                int nLeft = params.at(1).toInt();
                memcpy(leftN, &nLeft,sizeof(int));

                char topN[sizeof(int)];
                int nTop = params.at(2).toInt();
                memcpy(topN, &nTop,sizeof(int));

            }



        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QTextEdit * txt = this->findChild<QTextEdit*>("textEdit");
    newFormDialog nf;
    nf.setModal(true);
    nf.exec();

    if (nf.result()==true)
    {

        txt->setText("");
        txt->append("#defines");
        txt->append("#name");
        txt->append(nf.getName());

        txt->append("");

        txt->append("@.");
        txt->append("start");
        txt->append("/.");

        txt->append("");
        txt->append("@start");
        txt->append("/start");
    }
    QTextCursor tc = txt->textCursor();
    tc.movePosition(QTextCursor::PreviousBlock,QTextCursor::MoveAnchor);
    tc.movePosition(QTextCursor::EndOfLine,QTextCursor::MoveAnchor);
    tc.insertText("\n");
    txt->setTextCursor(tc);
    txt->setFocus();
}



QPushButton *MainWindow::findButton(QObject *parent, QString name)
{
    return parent->findChild<QPushButton*>(name);
}


void MainWindow::setUpShortCats(int groupIndex)
{
    QTabWidget * tab = this->findChild<QTabWidget*>("tabWidget");

    switch (groupIndex)
    {
    case 0:
        findButton(tab, "pushButton")->setShortcut(tr("Ctrl+N"));
        findButton(tab, "pushButton_4")->setShortcut(tr("Ctrl+F"));
        findButton(tab, "pushButton_3")->setShortcut(tr("Ctrl+D"));
        findButton(tab, "pushButton_2")->setShortcut(tr("Ctrl+R"));
        findButton(tab, "pushButton_ssp")->setShortcut(tr("Ctrl+S"));
        break;
    case 1:
        findButton(tab, "pushButton_11")->setShortcut(tr("Ctrl+N"));
        findButton(tab, "pushButton_15")->setShortcut(QKeySequence(Qt::ALT + Qt::Key_Right));
        findButton(tab, "pushButton_16")->setShortcut(QKeySequence(Qt::ALT + Qt::Key_Left));
        findButton(tab, "pushButton_14")->setShortcut(QKeySequence(Qt::ALT + Qt::Key_Up));
        findButton(tab, "pushButton_13")->setShortcut(QKeySequence(Qt::ALT + Qt::Key_Down));
        findButton(tab, "pushButton_12")->setShortcut(tr("Ctrl+M"));
        findButton(tab, "pushButton_10")->setShortcut(tr("Ctrl+F"));
        findButton(tab, "pushButton_9")->setShortcut(tr("Ctrl+E"));
        break;
    case 2:
        findButton(tab, "pushButton_27")->setShortcut(tr("Ctrl+N"));
        findButton(tab, "pushButton_26")->setShortcut(tr("Ctrl+D"));
        findButton(tab, "pushButton_25")->setShortcut(tr("Ctrl+A"));
        findButton(tab, "pushButton_24")->setShortcut(tr("Ctrl+W"));
        findButton(tab, "pushButton_23")->setShortcut(tr("Ctrl+S"));
        findButton(tab, "pushButton_21")->setShortcut(tr("Ctrl+M"));
        findButton(tab, "pushButton_19")->setShortcut(tr("Ctrl+E"));
        findButton(tab, "pushButton_17")->setShortcut(tr("Ctrl+O"));
        break;
    case 3:
        findButton(tab, "pushButton_38")->setShortcut(tr("Ctrl+N"));
        findButton(tab, "pushButton_37")->setShortcut(tr("Ctrl+L"));
        findButton(tab, "pushButton_36")->setShortcut(tr("Ctrl+P"));
        findButton(tab, "pushButton_35")->setShortcut(tr("Ctrl+S"));
        findButton(tab, "pushButton_34")->setShortcut(tr("Ctrl+B"));
        findButton(tab, "pushButton_33")->setShortcut(tr("Ctrl+V"));
        findButton(tab, "pushButton_32")->setShortcut(tr("Ctrl+F"));
        findButton(tab, "pushButton_31")->setShortcut(tr("Ctrl+G"));
        findButton(tab, "pushButton_30")->setShortcut(tr("Ctrl+U"));
        findButton(tab, "pushButton_29")->setShortcut(tr("Ctrl+I"));
        break;
    case 4:
        findButton(tab, "pushButton_46")->setShortcut(tr("Ctrl+N"));
        findButton(tab, "pushButton_45")->setShortcut(tr("Ctrl+P"));
        findButton(tab, "pushButton_44")->setShortcut(tr("Ctrl+S"));
        findButton(tab, "pushButton_43")->setShortcut(tr("Ctrl+M"));
        findButton(tab, "pushButton_42")->setShortcut(tr("Ctrl+D"));
        findButton(tab, "pushButton_41")->setShortcut(tr("Ctrl+C"));
        findButton(tab, "pushButton_40")->setShortcut(tr("Ctrl+G"));
        findButton(tab, "pushButton_39")->setShortcut(tr("Ctrl+S"));
        break;
    default:
        break;
    }
    for (int i =0; i<6; i++)
        if (i != groupIndex)
            this->removeShortCats(i);
}

void MainWindow::removeShortCats(int groupIndex)
{
    QTabWidget * tab = this->findChild<QTabWidget*>("tabWidget");

    switch (groupIndex)
    {
    case 0:
        findButton(tab, "pushButton")->setShortcut(0);
        findButton(tab, "pushButton_4")->setShortcut(0);
        findButton(tab, "pushButton_3")->setShortcut(0);
        findButton(tab, "pushButton_2")->setShortcut(0);
        findButton(tab, "pushButton_ssp")->setShortcut(0);
        break;
    case 1:
        findButton(tab, "pushButton_11")->setShortcut(0);
        findButton(tab, "pushButton_15")->setShortcut(0);
        findButton(tab, "pushButton_16")->setShortcut(0);
        findButton(tab, "pushButton_14")->setShortcut(0);
        findButton(tab, "pushButton_13")->setShortcut(0);
        findButton(tab, "pushButton_12")->setShortcut(0);
        findButton(tab, "pushButton_10")->setShortcut(0);
        findButton(tab, "pushButton_9")->setShortcut(0);
        break;
    case 2:
        findButton(tab, "pushButton_27")->setShortcut(0);
        findButton(tab, "pushButton_26")->setShortcut(0);
        findButton(tab, "pushButton_25")->setShortcut(0);
        findButton(tab, "pushButton_24")->setShortcut(0);
        findButton(tab, "pushButton_23")->setShortcut(0);
        findButton(tab, "pushButton_21")->setShortcut(0);
        findButton(tab, "pushButton_19")->setShortcut(0);
        findButton(tab, "pushButton_17")->setShortcut(0);
        break;
    case 3:
        findButton(tab, "pushButton_38")->setShortcut(0);
        findButton(tab, "pushButton_37")->setShortcut(0);
        findButton(tab, "pushButton_36")->setShortcut(0);
        findButton(tab, "pushButton_35")->setShortcut(0);
        findButton(tab, "pushButton_34")->setShortcut(0);
        findButton(tab, "pushButton_33")->setShortcut(0);
        findButton(tab, "pushButton_32")->setShortcut(0);
        findButton(tab, "pushButton_31")->setShortcut(0);
        findButton(tab, "pushButton_30")->setShortcut(0);
        findButton(tab, "pushButton_29")->setShortcut(0);
        break;
    case 4:
        findButton(tab, "pushButton_46")->setShortcut(0);
        findButton(tab, "pushButton_45")->setShortcut(0);
        findButton(tab, "pushButton_44")->setShortcut(0);
        findButton(tab, "pushButton_43")->setShortcut(0);
        findButton(tab, "pushButton_42")->setShortcut(0);
        findButton(tab, "pushButton_41")->setShortcut(0);
        findButton(tab, "pushButton_40")->setShortcut(0);
        findButton(tab, "pushButton_39")->setShortcut(0);
        break;
    default:
        break;
    }
}


void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    setUpShortCats(index);

}

void MainWindow::on_textEdit_textChanged()
{

}

void MainWindow::rehighlightText()
{
    hl->rehighlight();
}

void MainWindow::on_pushButton_ssp_clicked()
{
    //here goes code;
    QTextEdit * txt = this->findChild<QTextEdit*>("textEdit");
    Defines d;
    d.LoadFromString(txt->toPlainText());
    SetupScrollParamsDialog dialog;
    dialog.init(!d.IsDefined("holdscrollingleft"), !d.IsDefined("holdscrolling"),
                d.IsDefined("smoothscrollingv") && d.IsDefined("smoothscrollingh"),
                d.IsDefined("startscrollleft") || d.IsDefined("startscrolltop"),
                d.Get("leftleftbound").toInt(), d.Get("toptopbound").toInt(),
                d.Get("leftrightbound").toInt(), d.Get("topbottombound").toInt(),
                d.Get("minscrollleft").toInt(), d.Get("minscrolltop").toInt(),
                d.Get("maxscrollleft").toInt(), d.Get("maxscrolltop").toInt(),
                d.Get("smoothscrollingh").toInt(), d.Get("smoothscrollingv").toInt(),
                d.Get("startscrollleft").toInt(), d.Get("startscrolltop").toInt()
                );
    dialog.exec();
    if (dialog.result() == true)
    {

        if (!dialog.isHScrollEnabled())
            d.Define("holdscrollingleft");
        else
            d.Undef("holdscrollingleft");
        if (!dialog.isVScrollEnabled())
            d.Define("holdscrollingtop");
        else
            d.Undef("holdscrollingtop");
        if (dialog.isHScrollEnabled() || dialog.isVScrollEnabled())
        {
            QRect bounds = dialog.getInternalRect();
            d.Set("leftleftbound",  QString::number(bounds.left()));
            d.Set("leftrightbound", QString::number(bounds.right()));
            d.Set("toptopbound",    QString::number(bounds.top()));
            d.Set("topbottombound", QString::number(bounds.bottom()));
            QRect exBounds = dialog.getExternalRect();
            d.Set("minscrollleft",  QString::number(exBounds.left()));
            d.Set("maxscrollleft",  QString::number(exBounds.right()));
            d.Set("minscrolltop",   QString::number(exBounds.top()));
            d.Set("maxscrolltop",   QString::number(exBounds.bottom()));
        }
        else
        {
            d.Undef("leftleftbound");
            d.Undef("leftrightbound");
            d.Undef("toptopbound");
            d.Undef("topbottombound");
            d.Undef("minscrollleft");
            d.Undef("maxscrollleft");
            d.Undef("minscrolltop");
            d.Undef("maxscrolltop");
        }
        if (dialog.isSmoothScrollingEnabled())
        {
            d.Set("smoothscrollingh",   QString::number(dialog.getSmoothH()));
            d.Set("smoothscrollingv",   QString::number(dialog.getSmoothV()));
        }
        else
        {
            d.Undef("smoothscrollingh");
            d.Undef("smoothscrollingv");
        }
        if (dialog.isStartScrollingEnabled())
        {
            d.Set("startscrollleft",    QString::number(dialog.getStartH()));
            d.Set("startscrolltop",     QString::number(dialog.getStartV()));
        }
        else
        {
            d.Undef("startscrollleft");
            d.Undef("startscrolltop");
        }
        txt->setPlainText(d.toString());
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    QMessageBox::StandardButton reply;
    QTextEdit * txt = this->findChild<QTextEdit*>("textEdit");
    reply = QMessageBox::question(this, "Removing Function", "Do you really want to remove Current Function?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        Defines d;
        QString plainTxt = txt->toPlainText();
        QStringList ptLines = plainTxt.split("\n");
        d.LoadFromString(txt->toPlainText());
        QTextCursor tc = txt->textCursor();
        QString funName;
        bool funFound = false;
        QString prevFun;
        for (int i = tc.blockNumber(); i>=0; i--)
        {
            QString curLine = ptLines.at(i);
            if (curLine != "" && curLine.at(0) == '@' && (!funFound))
            {
                funName = curLine.replace("@","");
                funFound = true;
            }
            else if (curLine != "" &&(curLine.at(0) == '@' || (i == 0)))
            {
                prevFun = curLine;
                break;
            }
        }
        d.RemoveList(funName);
        d.RemoveFromList(".",funName);
        txt->setPlainText(d.toString());
        tc = txt->textCursor();
        while (ptLines.at(tc.blockNumber()) != prevFun)
            tc.movePosition(QTextCursor::NextBlock,QTextCursor::MoveAnchor);
        txt->setTextCursor(tc);
    }
}

void MainWindow::on_pushButton_11_clicked()
{
    newSpriteDialog nsd;
    QTextEdit * txt = this->findChild<QTextEdit*>("textEdit");
    QStringList strings = txt->toPlainText().split("\n");
    QStringList vars;
    int strIndex=txt->textCursor().blockNumber()-1, curStrIndex=0;
    foreach (const QString &str, strings)
    {
        if (str == "")
            continue;
        if (curStrIndex>strIndex-1)
            break;
        QStringList inWords = str.split(" ");
        if (inWords.empty())
            break;
        QString command = inWords.at(0).toLower();
        if (command == "_set" || command == "set" || command == "mapset")
        {
            if (inWords.count() > 2 && inWords.at(2).length()!=0)
                vars.append(inWords.at(1));
        }
        curStrIndex++;
    }
    nsd.setValueList(vars);
    nsd.exec();
    if (nsd.result() == true)
    {
        QTextCursor tc = txt->textCursor();
        if (nsd.checkLayout())
        {
            spriteLayout = nsd.getLayout();
            this->isSpriteLayoutSetUp = true;
            tc.insertText("set "+spriteLayout.indexValue +" 0");
            tc.insertText("\nset "+spriteLayout.leftValue+" "+nsd.getLeftPos());
            tc.insertText("\nset "+spriteLayout.topValue +" "+nsd.getTopPos());
            tc.insertText("\naddSprite "+spriteLayout.type+" "+spriteLayout.namePrefix+"$"+spriteLayout.indexValue);

        }
        else
        {
            isSpriteLayoutSetUp = false;
            tc.insertText("\naddsprite "+nsd.getType()+" "+nsd.getName());

        }
        if (nsd.checkMoveSprite())
        {
            tc.insertText("\nmoveSprite "+spriteLayout.namePrefix+"$"+spriteLayout.indexValue+
                          +" $"+spriteLayout.leftValue+" $"+spriteLayout.topValue);
        }
        if (nsd.checkSetAnimation())
        {
            tc.insertText("\nsetAnimation "+spriteLayout.namePrefix+"$"+spriteLayout.indexValue
                          +" "+nsd.getAnimationName());
        }
        txt->setTextCursor(tc);
    }

}

void MainWindow::on_pushButton_4_clicked()
{
    QTextEdit * txt = this->findChild<QTextEdit*>("textEdit");
    newFormDialog nf;
    nf.setLabel("New Function Name");
    nf.setModal(true);
    nf.exec();
    if (nf.result() == true)
    {
        Defines d;
        QString funcName = nf.getName();
        QTextCursor tc = txt->textCursor();
        tc.insertText("\n@"+funcName+"\n\n/"+funcName+"\n");
        txt->setTextCursor(tc);
        d.LoadFromString(txt->toPlainText());
        d.AddToList(".",funcName);
        txt->setPlainText(d.toString());
        txt->setTextCursor(tc);
        txt->moveCursor(QTextCursor::PreviousBlock);
        txt->moveCursor(QTextCursor::PreviousBlock);
        txt->moveCursor(QTextCursor::PreviousBlock);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    callFunctionDialog dlg;
    QTextEdit * txt = this->findChild<QTextEdit*>("textEdit");
    dlg.setFunctions(txt->toPlainText());
    dlg.setModal(true);
    dlg.exec();
    if (dlg.result() == true)
    {
        QTextCursor tc = txt->textCursor();
        if (dlg.checkDelay())
        {
            QString commandText = "\nwaitcall "+dlg.getDelay()+" :"+dlg.getFunction();
            tc.insertText(commandText);
            tc.movePosition(QTextCursor::Right,QTextCursor::MoveAnchor,commandText.length());
        }
        else
        {
            tc.insertText(":"+dlg.getFunction());
            tc.movePosition(QTextCursor::Right,QTextCursor::MoveAnchor,dlg.getFunction().length());
        }
        txt->setTextCursor(tc);
    }
}

void MainWindow::on_pushButton_15_clicked()
{
    if (isSpriteLayoutSetUp)
    {
        QTextEdit * txt = this->findChild<QTextEdit*>("textEdit");
        QTextCursor tc = txt->textCursor();
        tc.insertText("\nadd "+spriteLayout.indexValue+" 1");
        tc.insertText("\nadd "+spriteLayout.leftValue+" "+spriteLayout.leftShift);
        tc.insertText("\naddSprite "+spriteLayout.type+" "+spriteLayout.namePrefix+"$"+spriteLayout.indexValue);
        tc.insertText("\nmoveSprite "+spriteLayout.namePrefix+"$"+spriteLayout.indexValue
                      +" $"+spriteLayout.leftValue+" $"+spriteLayout.topValue);
        txt->setTextCursor(tc);
    }
}

void MainWindow::on_pushButton_16_clicked()
{
    if (isSpriteLayoutSetUp)
    {
        QTextEdit * txt = this->findChild<QTextEdit*>("textEdit");
        QTextCursor tc = txt->textCursor();
        tc.insertText("\nadd "+spriteLayout.indexValue+" 1");
        tc.insertText("\nsub "+spriteLayout.leftValue+" "+spriteLayout.leftShift);
        tc.insertText("\naddSprite "+spriteLayout.type+" "+spriteLayout.namePrefix+"$"+spriteLayout.indexValue);
        tc.insertText("\nmoveSprite "+spriteLayout.namePrefix+"$"+spriteLayout.indexValue
                      +" $"+spriteLayout.leftValue+" $"+spriteLayout.topValue);
        txt->setTextCursor(tc);
    }
}

void MainWindow::on_pushButton_14_clicked()
{
    if (isSpriteLayoutSetUp)
    {
        QTextEdit * txt = this->findChild<QTextEdit*>("textEdit");
        QTextCursor tc = txt->textCursor();
        tc.insertText("\nadd "+spriteLayout.indexValue+" 1");
        tc.insertText("\nsub "+spriteLayout.topValue+" "+spriteLayout.topShift);
        tc.insertText("\naddSprite "+spriteLayout.type+" "+spriteLayout.namePrefix+"$"+spriteLayout.indexValue);
        tc.insertText("\nmoveSprite "+spriteLayout.namePrefix+"$"+spriteLayout.indexValue
                      +" $"+spriteLayout.leftValue+" $"+spriteLayout.topValue);
        txt->setTextCursor(tc);
    }
}

void MainWindow::on_pushButton_13_clicked()
{
    if (isSpriteLayoutSetUp)
    {
        QTextEdit * txt = this->findChild<QTextEdit*>("textEdit");
        QTextCursor tc = txt->textCursor();
        tc.insertText("\nadd "+spriteLayout.indexValue+" 1");
        tc.insertText("\nadd "+spriteLayout.topValue+" "+spriteLayout.topShift);
        tc.insertText("\naddSprite "+spriteLayout.type+" "+spriteLayout.namePrefix+"$"+spriteLayout.indexValue);
        tc.insertText("\nmoveSprite "+spriteLayout.namePrefix+"$"+spriteLayout.indexValue
                      +" $"+spriteLayout.leftValue+" $"+spriteLayout.topValue);
        txt->setTextCursor(tc);
    }
}

void MainWindow::on_pushButton_12_clicked()
{
    QTextEdit * txt = this->findChild<QTextEdit*>("textEdit");
    QTextCursor tc = txt->textCursor();
    hl->updateMap(tc.blockNumber());
    ChooseForm dlg;
    QStringList emptyList;
    dlg.setLabels("Sprite Name","");
    dlg.setEditable(true,false);
    dlg.setLinesEnabled(true, true);
    dlg.setupComboBoxes(hl->spriteList,emptyList);
    dlg.exec();
    if (dlg.result())
    {
        tc.insertText("moveSprite "+dlg.getText(0) + " " + dlg.getText(2)+ " " +dlg.getText(3)+"\n");
    }
    txt->setTextCursor(tc);
}

void MainWindow::on_pushButton_10_clicked()
{
    QTextEdit * txt = this->findChild<QTextEdit*>("textEdit");
    QTextCursor tc = txt->textCursor();
    ChooseForm dlg;
    hl->updateMap(tc.blockNumber());

    QStringList spriteList = hl->spriteList;
    QStringList emptyList;
    for (int i=0; i<spriteList.count(); i++)
    {
        QString sprType = hl->spriteTypes.at(i);
        spriteDesc spd = newSpriteDialog::getSpriteDesc(sprType);
        spd.animations << "!start"<<"!stop";
        dlg.addCustomItems(spd.animations);
    }


    dlg.setLabels("Sprite Name","");
    dlg.setEditable(true,true);
    dlg.setLinesEnabled(false, false);
    dlg.setCustomItems(true);
    dlg.setupComboBoxes(hl->spriteList,emptyList);
    dlg.exec();
    if (dlg.result())
    {
        tc.insertText("setAnimation "+dlg.getText(0) + " " + dlg.getText(1)+"\n");
    }
    txt->setTextCursor(tc);
}

void MainWindow::on_pushButton_9_clicked()
{
    QTextEdit * txt = this->findChild<QTextEdit*>("textEdit");
    QTextCursor tc = txt->textCursor();
    newAnimate dlg;
    hl->updateMap(tc.blockNumber());
    dlg.setAnimateSprite(true);
    dlg.setLists(hl->spriteList,hl->objectList);
    dlg.setFunctionsList(hl->functionList);
    dlg.exec();
    if (dlg.result())
    {
        tc.insertText("animate "+dlg.getName()+" "+dlg.getObjectName()+ " "+
                      QString::number(dlg.getDL()) +" "+QString::number(dlg.getDT())+" "+
                      QString::number(dlg.getSpeed())+" linear\n");
        if (dlg.isOnStartCallback())
            tc.insertText("animation."+dlg.getName()+".onStart :"+dlg.getOnStartCallback()+"\n");
        if (dlg.isOnEndCallback())
            tc.insertText("animation."+dlg.getName()+".onEnd :"+dlg.getOnEndCallback()+"\n");
        txt->setTextCursor(tc);
    }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    setUpShortCats(index);
}

void MainWindow::on_pushButton_27_clicked()
{
    QTextEdit * txt = this->findChild<QTextEdit*>("textEdit");
    QStringList objectTypes;
    objectTypes << "kappa" << "customkappa" << "boombox"
                << "box 0 128" << "box 0 256" << "box 0 512"<< "box 0 1024"
                << "box 1 128" << "box 0 256";
    newSpriteDialog nsd;
    nsd.setLabelsForAddObject();
    nsd.clearContent();
    for (int i=0; i<objectTypes.count(); i++)
    {
        if (i<2)
        {
            spriteDesc sd = nsd.getSpriteDesc("kappa");
            nsd.addNewObject(objectTypes.at(i),sd.w,sd.h,sd.animations);
            continue;
        }
        else if (i == 2)
        {
            spriteDesc sd = nsd.getSpriteDesc("boombox");
            nsd.addNewObject(objectTypes.at(i),sd.w,sd.h,sd.animations);
            continue;

        }
        else
        {
            spriteDesc sd;
            sd.animations << "still";
            sd.h = sd.w = objectTypes.at(i).split(" ").at(2).toInt();
            nsd.addNewObject(objectTypes.at(i),sd.w,sd.h,sd.animations);
        }
    }
    nsd.setValueList(hl->vars);
    nsd.exec();
    if (nsd.result())
    {
        QTextCursor tc = txt->textCursor();
        if (nsd.checkLayout())
        {
            objectLayout = nsd.getLayout();
            this->isObjectLayoutSetUp = true;
            tc.insertText("mapset "+objectLayout.indexValue +" 0");
            tc.insertText("\nmapset "+objectLayout.leftValue+" "+nsd.getLeftPos());
            tc.insertText("\nmapset "+objectLayout.topValue +" "+nsd.getTopPos());
            tc.insertText("\naddObject "+objectLayout.namePrefix+"$"+objectLayout.indexValue+
                                        + " "+ objectLayout.type);

        }
        else
        {
            isObjectLayoutSetUp = false;
            tc.insertText("\naddObject "+nsd.getName()+" "+nsd.getType());

        }
        if (nsd.checkMoveSprite())
        {
            tc.insertText("\nmoveObject "+objectLayout.namePrefix+"$"+objectLayout.indexValue+
                          +" $"+objectLayout.leftValue+" $"+objectLayout.topValue);
        }
        if (nsd.checkSetAnimation())
        {
            tc.insertText("\nsetObjectAnimation "+objectLayout.namePrefix+"$"+objectLayout.indexValue
                          +" "+nsd.getAnimationName());
        }
        txt->setTextCursor(tc);
    }

}

void MainWindow::on_pushButton_26_clicked()
{
    if (isObjectLayoutSetUp)
    {
        QTextEdit * txt = this->findChild<QTextEdit*>("textEdit");
        QTextCursor tc = txt->textCursor();
        tc.insertText("\nmapadd "+objectLayout.indexValue+" 1");
        tc.insertText("\nmapadd "+objectLayout.leftValue+" "+objectLayout.leftShift);
        tc.insertText("\naddObject "+objectLayout.namePrefix+"$"+objectLayout.indexValue+" "+objectLayout.type);
        tc.insertText("\nmoveObject "+objectLayout.namePrefix+"$"+objectLayout.indexValue
                      +" $"+objectLayout.leftValue+" $"+objectLayout.topValue);
        txt->setTextCursor(tc);
    }
}

void MainWindow::on_pushButton_25_clicked()
{
    if (isObjectLayoutSetUp)
    {
        QTextEdit * txt = this->findChild<QTextEdit*>("textEdit");
        QTextCursor tc = txt->textCursor();
        tc.insertText("\nmapadd "+objectLayout.indexValue+" 1");
        tc.insertText("\nmapsub "+objectLayout.leftValue+" "+objectLayout.leftShift);
        tc.insertText("\naddObject "+objectLayout.namePrefix+"$"+objectLayout.indexValue+" "+objectLayout.type);
        tc.insertText("\nmoveObject "+objectLayout.namePrefix+"$"+objectLayout.indexValue
                      +" $"+objectLayout.leftValue+" $"+objectLayout.topValue);
        txt->setTextCursor(tc);
    }
}

void MainWindow::on_pushButton_24_clicked()
{
    if (isObjectLayoutSetUp)
    {
        QTextEdit * txt = this->findChild<QTextEdit*>("textEdit");
        QTextCursor tc = txt->textCursor();
        tc.insertText("\nmapadd "+objectLayout.indexValue+" 1");
        tc.insertText("\nmapsub "+objectLayout.topValue+" "+objectLayout.topShift);
        tc.insertText("\naddObject "+objectLayout.namePrefix+"$"+objectLayout.indexValue+" "+objectLayout.type);
        tc.insertText("\nmoveObject "+objectLayout.namePrefix+"$"+objectLayout.indexValue
                      +" $"+objectLayout.leftValue+" $"+objectLayout.topValue);
        txt->setTextCursor(tc);
    }
}

void MainWindow::on_pushButton_23_clicked()
{
    if (isObjectLayoutSetUp)
    {
        QTextEdit * txt = this->findChild<QTextEdit*>("textEdit");
        QTextCursor tc = txt->textCursor();
        tc.insertText("\nmapadd "+objectLayout.indexValue+" 1");
        tc.insertText("\nmapadd "+objectLayout.topValue+" "+objectLayout.topShift);
        tc.insertText("\naddObject "+objectLayout.namePrefix+"$"+objectLayout.indexValue+" "+objectLayout.type);
        tc.insertText("\nmoveObject "+objectLayout.namePrefix+"$"+objectLayout.indexValue
                      +" $"+objectLayout.leftValue+" $"+objectLayout.topValue);
        txt->setTextCursor(tc);
    }
}

void MainWindow::on_pushButton_21_clicked()
{
    QTextEdit * txt = this->findChild<QTextEdit*>("textEdit");
    QTextCursor tc = txt->textCursor();
    hl->updateMap(tc.blockNumber());
    ChooseForm dlg;
    QStringList emptyList;
    dlg.setLabels("Object Name","");
    dlg.setEditable(true,false);
    dlg.setLinesEnabled(true, true);
    dlg.setupComboBoxes(hl->objectList,emptyList);
    dlg.exec();
    if (dlg.result())
    {
        tc.insertText("moveObject "+dlg.getText(0) + " " + dlg.getText(2)+ " " +dlg.getText(3)+"\n");
    }
    txt->setTextCursor(tc);
}

void MainWindow::on_pushButton_20_clicked()
{

}

void MainWindow::on_pushButton_19_clicked()
{
    QTextEdit * txt = this->findChild<QTextEdit*>("textEdit");
    QTextCursor tc = txt->textCursor();
    newAnimate dlg;
    hl->updateMap(tc.blockNumber());
    dlg.setAnimateSprite(true);
    dlg.setLists(hl->objectList,hl->objectList);
    dlg.setFunctionsList(hl->functionList);
    dlg.exec();
    if (dlg.result())
    {
        tc.insertText("animateObject "+dlg.getName()+" "+dlg.getObjectName()+ " "+
                      QString::number(dlg.getDL()) +" "+QString::number(dlg.getDT())+" "+
                      QString::number(dlg.getSpeed())+"\n");
        if (dlg.isOnStartCallback())
            tc.insertText("animation."+dlg.getName()+".onStart :"+dlg.getOnStartCallback()+"\n");
        if (dlg.isOnEndCallback())
            tc.insertText("animation."+dlg.getName()+".onEnd :"+dlg.getOnEndCallback()+"\n");
        txt->setTextCursor(tc);
    }
}

void MainWindow::on_pushButton_17_clicked()
{
    QTextEdit * txt = this->findChild<QTextEdit*>("textEdit");
    QTextCursor tc = txt->textCursor();
    hl->updateMap(tc.blockNumber());
    ChooseForm dlg;
    dlg.setLinesEnabled(true,true,true,true);
    dlg.setEditable(false,false);
    dlg.setupComboBoxes(hl->objectList,hl->functionList);
    dlg.setLabels("Object Name: ","Function to Call: ");
    dlg.exec();
    if (dlg.result())
    {
        tc.insertText("onObjectRect "+dlg.getText(0)
                      + " "+dlg.getText(2)+ " "+dlg.getText(3)+ " "+dlg.getText(4)+ " "+dlg.getText(5)
                      + " :"+dlg.getText(1));
        txt->setTextCursor(tc);
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    QTextEdit * txt = this->findChild<QTextEdit*>("textEdit");
    QString str = txt->toPlainText();
    QByteArray ba(str.toStdString().c_str());
    QByteArray cba = qCompress(ba,9);
    QFile file("../map.dat");
    file.open(QIODevice::WriteOnly);
    file.write(cba);
    file.close();
}
