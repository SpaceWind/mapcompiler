#ifndef MAPSYNTAXHIGHLIGHTER_H
#define MAPSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextEdit>

class MapSyntaxHighlighter : public QSyntaxHighlighter
{
    friend class MainWindow;
    Q_OBJECT
public:
    explicit MapSyntaxHighlighter(QObject *parent = 0);
    MapSyntaxHighlighter(QTextEdit * parent);
    void updateMap(int lastStringIndex);
protected:
    void highlightBlock(const QString &str);


signals:

public slots:

private:

    enum {normalState = 0, inDefineState = 1, inFunctionState = 2, inRootFunctionState = 3};
    enum {operationAdd = 0, operationSub = 1, operationMul = 2, operationDiv = 3};


    bool highlightCommand(QString str);
    bool spellCheckCommand(QString str);
    bool spellCheckDynCommand(QString str);
    void varOperation(QString varName, QString value, int operation);
    QString translateString(QString varRef);
    static QString translateString(QString varRef, QHash<QString,QString> varMap);


    struct HighlightingRule
    {
         QRegExp pattern;
         QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp defineExp;
    QRegExp animateExp;
    QRegExp funcStartExp;
    QRegExp funcEndExp;

    QTextCharFormat defineFormat;
    QTextCharFormat defineValueFormat;
    QTextCharFormat sceneCommandFormat;
    QTextCharFormat mapCommandFormat;
    QTextCharFormat soundCommandFormat;
    QTextCharFormat digitFormat;
    QTextCharFormat objectFormat;
    QTextCharFormat wrongObjectFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat functionListFormat;
    QTextCharFormat wrongFormat;
    QTextCharFormat varFormat;
    QTextCharFormat varRefFormat;
    QTextCharFormat commentFormat;
    QTextCharFormat inactiveFormat;



    QStringList sceneCommands;
    QStringList soundCommands;
    QStringList mapCommands;
    QStringList varCommands;

    QStringList functionList;
    QStringList spriteList;
    QStringList spriteTypes;
    QStringList objectList;
    QStringList soundList;

    QStringList sceneAnimations;
    QStringList objectAnimations;
    QHash<QString, QString> vars;
    QString prevStr;

    QHash<QString,QString> mapCommandSyntaxRules;
    QTextEdit * txt;
    QString currentFunctionName;

};

#endif // MAPSYNTAXHIGHLIGHTER_H
