#include "defines.h"

Defines::Defines()
{
        encode = false;
}
Defines::~Defines()
{
}
Defines::Defines(QString filename)
{
    QFile textFile(filename);
    if(!textFile.open(QIODevice::ReadOnly))
    {
        qDebug("Error");
        return;
    }

    QTextStream textStream(&textFile);
    while (true)
    {
        QString line = textStream.readLine();
        if (line.isNull())
            break;
        else
            l.append(line);
    }
    textFile.close();
}
void Defines::Load(QString filename)
{
    l.clear();
    QFile textFile(filename);
    if(!textFile.open(QIODevice::ReadOnly))
    {
        qDebug("Error");
        return;
    }

    QTextStream textStream(&textFile);
    while (true)
    {
        QString line = textStream.readLine();
        if (line.isNull())
            break;
        else
            l.append(line);
    }
    textFile.close();
}

void Defines::LoadFromString(QString txt)
{
    l.clear();
    l.append(txt.split('\n'));
}
void Defines::Save(QString filename)
{
    QFile textFile(filename);
    if(!textFile.open(QIODevice::ReadWrite))
    {
        qDebug("Error");
        return;
    }

    QTextStream textStream(&textFile);
    for (QStringList::const_iterator constIterator = l.constBegin(); constIterator != l.constEnd(); ++constIterator)
          textStream << (*constIterator).toLocal8Bit().constData() << endl;
    textFile.close();
}

QString Defines::toString()
{
    QString str;
    for (QStringList::const_iterator it = l.constBegin(); it!=l.constEnd(); ++it)
        str = str + (*it) + "\n";
    return str;
}
QString Defines::Get(QString key, QString prefix, QString postfix)
{
        int index = -1;
        for (int i=0; i<l.size(); i++)
                if (l.at(i)==QString("#")+key)
                {
                        index = i+1;
                        break;
                }
        if (index == -1)
                return "";
        return prefix + l.at(index) + postfix;
}
void Defines::Set(QString key, QString value)
{
        int index = -1;
        for (int i=0; i<l.size(); i++)
                if (l.at(i) == QString("#")+key)
                {
                        index = i+1;
                        break;
                }
        if (index==-1)
        {
                l.append("#"+key);
                l.append(value);
        }else
                l[index]=value;
}
bool Defines::IsDefined(QString key)
{
        return (Get(key)!=QString(""));
}
void Defines::Define(QString key)
{
        if (!IsDefined(key))
                Set(key,"!");
}
void Defines::Undef(QString key)
{
    if (IsDefined(key))
        Set(key,"");
}


void Defines::SetList(QString key, QStringList & data)
{
    ClearList(key);
    AddToList(key,"");
    int index = l.indexOf("@"+key)+1;
    foreach (const QString& str, data)
    {
        l.insert(index, str);
        index++;
    }
}
QStringList Defines::GetList(QString key)
{
        int indexS = -1, indexF = -1;
        for (int i=0; i<l.size(); i++)
        {
                switch(indexS)
                {
                case -1:
                        if (l.at(i)==QString("@")+key)
                                indexS = i;
                        break;
                default:
                        if (l.at(i)==QString("/")+key)
                        {
                                indexF = i;
                                i = l.size();
                        }
                        break;
                }
        }
        QStringList out;
        if (indexS !=-1 && indexF!=-1)
        {
                for (int i=0; i<(indexF-indexS-1); i++)
                        out.append(l.at(indexS+1+i));
                return out;
        }
        else
                return out;
}
void Defines::RemoveList(QString key)
{
        int iStart=l.indexOf("@"+key);
        int iStop=l.indexOf("/"+key);

        for (int i=iStart; i<=iStop; i++)
            l.removeAt(iStart);
}

void Defines::ClearList(QString key)
{
    int iStart=l.indexOf("@"+key)+1;
    int iStop=l.indexOf("/"+key)-1;

    for (int i=iStart; i<=iStop; i++)
        l.removeAt(iStart);
}
bool Defines::AddToList(QString key, QString data)
{
        QStringList s = GetList(key);
        if (s.isEmpty())
                return 0;
        s.append(data);
        SetList(key,s);
        return true;
}
void Defines::RemoveFromList(QString keyList, QString data)
{
        QStringList s = GetList(keyList);
        if (s.isEmpty())
                return;
        int i=s.indexOf(data);
        if (i>=0)
                s.removeAt(i);
        SetList(keyList,s);
}

//---------------------------------------------------------------------------


