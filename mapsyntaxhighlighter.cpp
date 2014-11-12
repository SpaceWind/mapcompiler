#include "mapsyntaxhighlighter.h"

MapSyntaxHighlighter::MapSyntaxHighlighter(QObject *parent) :
    QSyntaxHighlighter(parent)
{
}

MapSyntaxHighlighter::MapSyntaxHighlighter(QTextEdit *parent) :
    QSyntaxHighlighter(parent->document())
{
    txt = parent;
    HighlightingRule rule;
    prevStr = "";

    defineFormat.setFontWeight(QFont::Bold);
    defineFormat.setForeground(QBrush(QColor(0,190,0)));
    defineValueFormat.setForeground(QBrush(QColor(0,120,0)));

    digitFormat.setForeground(QBrush(QColor(30,40,190)));

    objectFormat.setFontItalic(true);
    objectFormat.setForeground(QBrush(QColor(160,160,20)));

    wrongObjectFormat.setFontUnderline(true);
    wrongObjectFormat.setForeground(QBrush(QColor(255,150,0)));

    wrongFormat.setFontUnderline(true);
    wrongFormat.setForeground(QBrush(QColor(220,20,0)));

    functionFormat.setFontWeight(QFont::Bold);
    functionFormat.setForeground(QBrush(QColor(255,220,175)));

    functionListFormat.setForeground(QBrush(QColor(255,220,175)));

    objectFormat.setFontWeight(QFont::Bold);
    objectFormat.setForeground(QBrush(QColor(200,200,30)));

    sceneCommandFormat.setForeground(QBrush(QColor(255,180,76)));
    sceneCommandFormat.setFontItalic(true);

    mapCommandFormat.setForeground(QBrush(QColor(240,130,170)));
    mapCommandFormat.setFontItalic(true);

    soundCommandFormat.setForeground(QBrush(QColor(60,130,250)));
    soundCommandFormat.setFontItalic(true);

    varFormat.setForeground(QBrush(QColor(60,120,130)));
    varFormat.setFontItalic(true);

    varRefFormat.setForeground(QBrush(QColor(50,140,155)));
    varRefFormat.setFontItalic(true);
    varRefFormat.setFontWeight(QFont::Bold);

    commentFormat.setForeground(QBrush(QColor(140,140,160)));
    commentFormat.setFontItalic(true);

    inactiveFormat.setForeground(QBrush(QColor(140,100,100)));


    sceneCommands    << "addsprite"         << "movesprite"         << "removesprite"
                     << "addstaticsprite"   << "removestaticsprite" << "setanimation"
                     << "animate"           << "waitcall"           << "scenedone";
    mapCommandSyntaxRules["addsprite"] = "spritetype string";
    mapCommandSyntaxRules["movesprite"] = "sprite int int";
    mapCommandSyntaxRules["removesprite"] = "sprite";
    mapCommandSyntaxRules["addstaticsprite"] = "spritetype string";
    mapCommandSyntaxRules["removestaticsprite"] = "sprite";
    mapCommandSyntaxRules["setanimation"] = "sprite string";
    mapCommandSyntaxRules["animate"] = "string sprite int int int string";
    mapCommandSyntaxRules["waitcall"] = "int function";
    mapCommandSyntaxRules["scenedone"] = "";
    foreach (const QString &pattern, sceneCommands)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = sceneCommandFormat;
        highlightingRules.append(rule);
    }

    soundCommands    << "playsound"         << "playsoundand"   << "_loadsound"
                     << "_setsoundvolume"   << "_playsound"     << "_stopsound"
                     << "_unbindsound"      << "_freesound"     << "_freeall"
                     << "_unbind"           << "_bindsound";
    mapCommandSyntaxRules["playsound"] = "string";
    mapCommandSyntaxRules["playsoundand"] = "string function";
    mapCommandSyntaxRules["_loadsound"] = "string string bool";
    mapCommandSyntaxRules["_setsoundvolume"] = "sound int int";
    mapCommandSyntaxRules["_playsound"] = "sound";
    mapCommandSyntaxRules["_stopsound"] = "sound";
    mapCommandSyntaxRules["_unbindsound"] = "sound";
    mapCommandSyntaxRules["_freesound"] = "sound";
    mapCommandSyntaxRules["_freeall"] = "";
    mapCommandSyntaxRules["_unbind"] = "";
    mapCommandSyntaxRules["_bindsound"] = "sound object object int int int int";
    foreach (const QString &pattern, soundCommands)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = soundCommandFormat;
        highlightingRules.append(rule);
    }

    mapCommands << "addobject"      << "moveobject"         << "setobjectanimation"
                << "animateobject"  << "objectanimation"    << "removeobject"
                << "removeevents"   << "onobjectrect"       << "onmapscroll"
                << "done"           << "bindscrollingto"    << "unbindscrolling";
    foreach (const QString &pattern, mapCommands)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = mapCommandFormat;
        highlightingRules.append(rule);
    }
    varCommands << "set " << "_set " << "mapset " << "add " << "_add " << "mapadd "
                << "sub " << "_sub " << "mapsub " << "mul " << "_mul " << "mapmul "
                << "div " << "_div " << "mapdiv " << "cat " << "_cat " << "mapcat ";
    foreach (const QString &pattern, varCommands)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = varFormat;
        highlightingRules.append(rule);
    }


    mapCommandSyntaxRules["addobject"] = "string string";
    mapCommandSyntaxRules["moveobject"] = "object int int";
    mapCommandSyntaxRules["setobjectanimation"] = "string";
    mapCommandSyntaxRules["animateobject"] = "string object int int int";
    mapCommandSyntaxRules["removeobject"] = "object";
    mapCommandSyntaxRules["removeevents"] = "";
    mapCommandSyntaxRules["onobjectrect"] = "object int int int int function";
    mapCommandSyntaxRules["done"] = "";
    mapCommandSyntaxRules["bindscrollingto"] = "object";
    mapCommandSyntaxRules["unbindscrolling"] = "";

    mapCommandSyntaxRules["set"] = mapCommandSyntaxRules["_set"] = mapCommandSyntaxRules["mapset"] = "string string";
    mapCommandSyntaxRules["add"] = mapCommandSyntaxRules["_add"] = mapCommandSyntaxRules["mapadd"] = "value int";
    mapCommandSyntaxRules["sub"] = mapCommandSyntaxRules["_sub"] = mapCommandSyntaxRules["mapsub"] = "value int";
    mapCommandSyntaxRules["mul"] = mapCommandSyntaxRules["_mul"] = mapCommandSyntaxRules["mapmul"] = "value int";
    mapCommandSyntaxRules["div"] = mapCommandSyntaxRules["_div"] = mapCommandSyntaxRules["mapdiv"] = "value int";
    mapCommandSyntaxRules["cat"] = mapCommandSyntaxRules["_cat"] = mapCommandSyntaxRules["mapcat"] = "value string";

}



void MapSyntaxHighlighter::highlightBlock(const QString &str)
{
    int state = previousBlockState();
    QTextBlock tb = this->currentBlock();

    updateMap(tb.firstLineNumber()+tb.lineCount()-1);
    if (vars.contains("val"))
    {
        int a = 5;
        a+=5;
    }

        if (state == inDefineState)
        {
            if (str.mid(0,1)=="#")
                setFormat(0,str.length(),defineFormat);
            else
            {
                setFormat(0,str.length(),defineValueFormat);
                state = normalState;
            }
        }
        else if (state == inRootFunctionState)
        {
            if (translateString(str).indexOf("/.") == 0)
            {
                setFormat(0,str.length(),functionFormat);
                state = normalState;
            }
            else
                setFormat(0,str.length(),functionListFormat);
        }
        else if (state == inFunctionState)
        {
            if (translateString(str).mid(0, 2) == "//")
            {
                setFormat(0, str.length(), commentFormat);
            }
            else if (translateString(str).trimmed() == "/"+currentFunctionName)
            {
                if (functionList.contains(currentFunctionName))
                    setFormat(0,str.length(),functionFormat);
                else
                    setFormat(0,str.length(),wrongObjectFormat);
                state = normalState;
                currentFunctionName = "_::systemFunctionNameSH";
            }
            else if (translateString(str).mid(0,1) == ":" &&
                     functionList.contains(translateString(str)))
            {
                setFormat(0,str.length(),functionFormat);
            }
            else
            {
                highlightCommand(str);
                if (!spellCheckCommand(str))
                {
                    if (!spellCheckDynCommand(str))
                        setFormat(0,str.length(),wrongFormat);
                }
            }
        }
        else
        {
            QString normalStateTranslatedString = translateString(str);
            if (normalStateTranslatedString.mid(0,2) == "@.")
            {
                setFormat(0,str.length(),functionFormat);
                state = inRootFunctionState;
            }

            else if (normalStateTranslatedString.mid(0, 2) == "//")
            {
                setFormat(0, str.length(), commentFormat);
            }
            else if (normalStateTranslatedString.mid(0,1)=="#")
            {
                setFormat(0,str.length(),defineFormat);
                state = inDefineState;
            }
            else if ((normalStateTranslatedString.mid(0,1)=="@"))
            {
                QString funcName = normalStateTranslatedString.replace("@","");
                if (functionList.contains(funcName))
                    setFormat(0,str.length(),functionFormat);
                else
                    setFormat(0,str.length(),wrongObjectFormat);
                currentFunctionName = funcName;
                state = inFunctionState;

            }
            else
                setFormat(0,str.length(),inactiveFormat);

        }

    setCurrentBlockState(state);
}

bool MapSyntaxHighlighter::highlightCommand(QString str)
{
    bool found = false;
    QString trStr = translateString(str).toLower();
    foreach (const HighlightingRule &rule, highlightingRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(trStr);

        if (index == 0)
        {
            setFormat(index, str.split(" ").at(0).length(), rule.format);
            found = true;
            break;
        }
    }
    return found;
}

bool MapSyntaxHighlighter::spellCheckCommand(QString str)
{
    QString translatedString = translateString(str);
    QStringList inWords = str.split(" ");
    QStringList translatedWords = translatedString.split(" ");
    QStringList commandSyntax;
    QString command = translatedWords.at(0).toLower();
    if (!mapCommandSyntaxRules.contains(command))
        return false;
    if (mapCommandSyntaxRules[command]!="")
        commandSyntax = mapCommandSyntaxRules[command].split(" ");
    if ((inWords.count()-1 < commandSyntax.count()) && (!commandSyntax.isEmpty()))
    {
        return false;
    }
    int pos = inWords.at(0).length()+1;

    for (int i = 0; i<commandSyntax.count(); i++)
    {

        QString s = commandSyntax.at(i);
        QString word = inWords.at(i+1);
        QString translatedWord = translatedWords.at(i+1);
        if (s == "string")
        {
            pos+=word.length()+1;
            continue;
        }
        else if (s == "int")
        {
            bool isNumber;
            translatedWord.toInt(&isNumber);
            if (!isNumber)
                setFormat(str.mid(pos,str.length()-pos).indexOf(word)+pos,word.length(),wrongObjectFormat);
            else
                setFormat(str.mid(pos,str.length()-pos).indexOf(word)+pos,word.length(),digitFormat);
            pos+=word.length()+1;
        }
        else if (s == "sprite")
        {
            if (!spriteList.contains(translatedWord))
                setFormat(str.mid(pos,str.length()-pos).indexOf(word)+pos,word.length(),wrongObjectFormat);
            else
                setFormat(str.mid(pos,str.length()-pos).indexOf(word)+pos,word.length(),objectFormat);
            pos+=word.length()+1;
        }
        else if (s == "function")
        {
            if (!functionList.contains(translatedWord))
                setFormat(str.mid(pos,str.length()-pos).indexOf(word)+pos,word.length(),wrongObjectFormat);
            else
                setFormat(str.mid(pos,str.length()-pos).indexOf(word)+pos,word.length(),functionFormat);
            pos+=word.length()+1;
        }
        else if (s == "sound")
        {
            if (!soundList.contains(translatedWord))
                setFormat(str.mid(pos,str.length()-pos).indexOf(word)+pos,word.length(),wrongObjectFormat);
            else
                setFormat(str.mid(pos,str.length()-pos).indexOf(word)+pos,word.length(),objectFormat);
            pos+=word.length()+1;
        }
        else if (s == "object")
        {
            if (!objectList.contains(translatedWord))
                setFormat(str.mid(pos,str.length()-pos).indexOf(word)+pos,word.length(),wrongObjectFormat);
            else
                setFormat(str.mid(pos,str.length()-pos).indexOf(word)+pos,word.length(),objectFormat);
            pos+=word.length();
        }
        else if (s == "bool")
        {
            if (translatedWord.toLower() == "true" || translatedWord.toLower() == "false")
                setFormat(str.mid(pos,str.length()-pos).indexOf(word)+pos,word.length(),objectFormat);
            else
                setFormat(str.mid(pos,str.length()-pos).indexOf(word)+pos,word.length(),wrongObjectFormat);
            pos+=word.length()+1;
        }
        else if (s == "value")
        {
            if (vars.contains(translatedWord))
                setFormat(str.mid(pos,str.length()-pos).indexOf(word)+pos,word.length(),varRefFormat);
            else
                setFormat(str.mid(pos,str.length()-pos).indexOf(word)+pos,word.length(),wrongObjectFormat);
            pos+=word.length()+1;
        }

    }
    return true;
}

bool MapSyntaxHighlighter::spellCheckDynCommand(QString str)
{
    QString translatedString = translateString(str);
    QStringList inWords = str.split(" ");
    QStringList translatedWords = translatedString.split(" ");
    QStringList splitCommand = translatedWords.at(0).split(".");
    bool checkedWell = false, commandRight = false, splitType=false;
    if (splitCommand.count() == 3 && translatedWords.count()>=2)
        if (splitCommand.at(0).toLower()=="animation")
        {
            commandRight = true;
            splitType = true;
            if (sceneAnimations.contains(splitCommand.at(1)) &&
                ((splitCommand.at(2).toLower() == "onend")|| (splitCommand.at(2).toLower() == "onstart")))
                checkedWell = true;
        }
        else if (splitCommand.at(0).toLower()=="objectanimation")
        {
            commandRight = true;
            if (objectAnimations.contains(splitCommand.at(1)) &&
                ((splitCommand.at(2).toLower() == "onend")|| (splitCommand.at(2).toLower() == "onstart")))
                checkedWell = true;
        }
    if (commandRight && checkedWell)
    {
        if (splitType)
            setFormat(0,inWords.at(0).length(), sceneCommandFormat);
        else
            setFormat(0,inWords.at(0).length(), mapCommandFormat);
        if (translatedString == str)
            setFormat(str.indexOf(splitCommand.at(1)),splitCommand.at(1).length(), objectFormat);

        if (functionList.contains(translatedWords.at(1)))
            setFormat(str.indexOf(inWords.at(1)),inWords.at(1).length(),functionFormat);
        else
            setFormat(str.indexOf(inWords.at(1)),inWords.at(1).length(),wrongObjectFormat);
        return true;
    }
    return false;

}

void MapSyntaxHighlighter::varOperation(QString varName, QString value, int operation)
{
    if (vars.contains(varName))
    {
        if ((value.at(0)=='$') && (vars.contains(value.replace("$",""))))
            value = vars[value.replace("$","")];
        bool isValueInt = false;
        int valueInt = value.toInt(&isValueInt);
        bool isVarInt = false;
        int varInt = vars[varName].toInt(&isVarInt);
        if (isValueInt && isVarInt)
            switch (operation)
            {
            case operationAdd:
                vars[varName] = QString::number(varInt + valueInt);
                break;
            case operationSub:
                vars[varName] = QString::number(varInt - valueInt);
                break;
            case operationMul:
                vars[varName] = QString::number(varInt * valueInt);
                break;
            case operationDiv:
                vars[varName] = QString::number(varInt / valueInt);
                break;
            default:
                break;
            }
    }
}

QString MapSyntaxHighlighter::translateString(QString varRef)
{
    QHash<QString, QString>::const_iterator i = vars.constBegin();
    while (i != vars.constEnd())
    {
        varRef = varRef.replace("$"+i.key(),i.value());
        ++i;
    }
    return varRef;
}

QString MapSyntaxHighlighter::translateString(QString varRef, QHash<QString, QString> varMap)
{
    QHash<QString, QString>::const_iterator i = varMap.constBegin();
    while (i != varMap.constEnd())
    {
        varRef = varRef.replace("$"+i.key(),i.value());
        ++i;
    }
    return varRef;
}

void MapSyntaxHighlighter::updateMap(int lastStringIndex)
{
    QStringList strings = txt->toPlainText().split('\n');


    spriteList.clear();
    spriteTypes.clear();
    objectList.clear();
    soundList.clear();
    functionList.clear();
    vars.clear();
    int strIndex=lastStringIndex-1, curStrIndex=0;

    int rootTableStart = strings.indexOf("@.");
    int rootTableEnd   = strings.indexOf("/.");
    QStringList rootFunctions;
    for (int i=rootTableStart+1; i<rootTableEnd; ++i)
    {
        rootFunctions.append(strings.at(i));
        rootFunctions.removeDuplicates();
    }
    foreach (const QString &str, strings)
    {
        if (str == "")
            continue;
        if (str.at(0) == '@')
        {
            QString fname = str.mid(1,str.length()-1);
            if (rootFunctions.contains(fname))
            {
                functionList.append(fname);
                functionList.append(":"+fname);
                functionList.removeDuplicates();
            }
        }
    }


    foreach (const QString &str, strings)
    {
        if (str == "")
            continue;
        if (curStrIndex>strIndex-1)
            break;
        QStringList inWords = translateString(str).split(" ");
        if (inWords.empty())
            return;
        QString command = inWords.at(0).toLower();
        if (command == "addsprite" || command == "addstaticsprite")
        {
            if (inWords.count() > 2)
            {
                if (!spriteList.contains(inWords.at(2)))
                {
                    spriteList.append(inWords.at(2));
                    spriteTypes.append(inWords.at(1));
                }
            }
            spriteList.removeDuplicates();

        }
        if (command == "animate")
        {
            if (inWords.count() > 2)
            {
                sceneAnimations.append(inWords.at(1));
                sceneAnimations.removeDuplicates();
            }
        }
        else if (command == "animateobject")
        {
            if (inWords.count() > 2)
            {
                objectAnimations.append(inWords.at(1));
                objectAnimations.removeDuplicates();
            }
        }
        else if (command == "removesprite" || command == "removestaticsprite")
        {
            if (inWords.count()>1)
            if (spriteList.contains(inWords.at(1)))
            {
                int index = spriteList.indexOf(inWords.at(1));
                spriteList.removeAt(index);
                spriteTypes.removeAt(index);
            }
        }
        else if (command == "addobject")
        {
            if (inWords.count() > 1)
                objectList.append(inWords.at(1));
            objectList.removeDuplicates();
        }
        else if (command == "removeobject")
        {
            if (objectList.contains(inWords.at(1)))
                objectList.removeAt(objectList.indexOf(inWords.at(1)));
        }
        else if (command == "_freesound")
        {
            if (soundList.contains(inWords.at(1)))
                soundList.removeAt(soundList.indexOf(inWords.at(1)));
        }
        else if (command == "_loadsound")
        {
            if (inWords.count() > 1)
                soundList.append(inWords.at(1));
            soundList.removeDuplicates();
        }
        else if (command == "_set" || command == "set" || command == "mapset")
        {
            if (inWords.count() > 2 && inWords.at(2).length()!=0)
            {
                QString varName = inWords.at(1);
                QString value;
                for (int i = 2; i<inWords.count(); i++)
                {
                    value += inWords.at(i);
                    if (i != inWords.count()-1)
                        value += " ";
                }
                if ((value.at(0)=='$') && (vars.contains(value.replace("$",""))))
                    vars[varName] = vars[value.replace("$","")];
                else
                    vars[varName] = value;
            }
        }
        else if (command == "_cat" || command == "cat" || command == "mapcat")
        {
            if (inWords.count() > 2 && inWords.at(2).length()!=0)
            {
                QString varName = inWords.at(1);
                if (vars.contains(varName))
                {
                    QString value = inWords.at(2);
                    if ((value.at(0)=='$') && (vars.contains(value.replace("$",""))))
                        vars[varName] = vars[varName] + vars[value.replace("$","")];
                    else
                        vars[varName] = vars[varName] + value;
                }
            }
        }
        else if (command == "_add" || command == "add" || command == "mapadd")
        {
            if (inWords.count() > 2&& inWords.at(2).length()!=0)
                varOperation(inWords.at(1),inWords.at(2),operationAdd);
        }
        else if (command == "_sub" || command == "sub" || command == "mapsub")
        {
            if (inWords.count() > 2&& inWords.at(2).length()!=0)
                varOperation(inWords.at(1),inWords.at(2),operationSub);
        }
        else if (command == "_mul" || command == "mul" || command == "mapmul")
        {
            if (inWords.count() > 2&& inWords.at(2).length()!=0)
                varOperation(inWords.at(1),inWords.at(2),operationMul);
        }
        else if (command == "_div" || command == "div" || command == "mapdiv")
        {

            if (inWords.count() > 2&& inWords.at(2).length()!=0)
                varOperation(inWords.at(1),inWords.at(2),operationDiv);
        }
        curStrIndex++;
    }
}
