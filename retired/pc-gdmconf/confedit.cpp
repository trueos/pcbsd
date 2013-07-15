/**************************************************************************
*   Copyright (C) 2012 by Yuri Momotyuk                                   *
*   yurkis@gmail.com                                                      *
*                                                                         *
*   Permission is hereby granted, free of charge, to any person obtaining *
*   a copy of this software and associated documentation files (the       *
*   "Software"), to deal in the Software without restriction, including   *
*   without limitation the rights to use, copy, modify, merge, publish,   *
*   distribute, sublicense, and/or sell copies of the Software, and to    *
*   permit persons to whom the Software is furnished to do so, subject to *
*   the following conditions:                                             *
*                                                                         *
*   The above copyright notice and this permission notice shall be        *
*   included in all copies or substantial portions of the Software.       *
*                                                                         *
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
*   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
*   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*
*   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
*   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
*   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
*   OTHER DEALINGS IN THE SOFTWARE.                                       *
***************************************************************************/

#include "confedit.h"
#include <QTextStream>

bool CGDMConf::open(QString FileName)
{
    SGroup entry;
    mvGroups.clear();
    isDirty = false;
    mFileName="";

    QFile file(FileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QTextStream stream( &file );
    stream.setCodec("UTF-8");
    QString line;
    while(!stream.atEnd())
    {
        line = stream.readLine();        
        //test for comment
        if ( line.trimmed().indexOf('#', 0) == 0 )
        {
           entry.mvLines.push_back(line);
           continue;
        }//if comment

        //test for group begin
        if ( (line.trimmed().indexOf("[", 0) == 0 ) && (line.indexOf("]")>0))
        {
            mvGroups.push_back(entry);
            entry.mvLines.clear();
            int from = line.indexOf('[') + 1;
            int num = line.indexOf(']') - from;
            entry.mGroupName = line.mid(from, num);
            entry.mvLines.push_back(line);
            continue;
        }
        entry.mvLines.push_back(line);
    }//file read

    if (entry.mvLines.size())
        mvGroups.push_back(entry);

    mFileName = FileName;
    return true;
}

QString CGDMConf::getKey(const QString& Group, const QString& Key, QString Default )
{
    QString RetVal = Default;

    for (int i=0; i<mvGroups.size(); i++)
    {
        if ( 0 != QString::compare(mvGroups[i].mGroupName.trimmed(), Group.trimmed(), Qt::CaseInsensitive))
        {
            continue;
        }

        for(int j=0; j<mvGroups[i].mvLines.size(); j++)
        {
            QString line = mvGroups[i].mvLines[j];
            if ( line.trimmed().indexOf('#', 0) == 0 )
            {
                continue;
            }
            int eqpos = line.indexOf('=',0);
            if (eqpos<1)
                continue;
            QString KeyName = line.left(eqpos);
            if (0 == QString::compare(KeyName.trimmed(), Key.trimmed(), Qt::CaseInsensitive))
            {
                return line.mid(eqpos + 1);
            }
        }//for lines in group
    }//for all groups
    return RetVal;
}

bool CGDMConf::setKey(const QString& Group, const QString& Key, QString Val, bool isCreateKey, bool isCreateGroup)
{
    int GroupIDX=-1;
    for (int i=0; i<mvGroups.size(); i++)
    {
        if (0 == QString::compare(mvGroups[i].mGroupName.trimmed(), Group.trimmed(), Qt::CaseInsensitive))
        {
            GroupIDX = i;
            break;
        }
    }
    if (GroupIDX<0)
    {
        if (!isCreateGroup)
            return false;
        SGroup entry;
        entry.mGroupName = Group;
        entry.mvLines.push_back(QString("[")+Group+QString("]"));
        mvGroups.push_back(entry);
        GroupIDX = mvGroups.size()-1;
        isDirty = true;
    }

    for(int i=0; i<mvGroups[GroupIDX].mvLines.size(); i++)
    {
        QString line = mvGroups[GroupIDX].mvLines[i];
        if ( line.trimmed().indexOf('#', 0) == 0 )
        {
            continue;
        }
        int eqpos = line.indexOf('=',0);
        if (eqpos<1)
            continue;
        QString KeyName = line.left(eqpos);
        if (0 == QString::compare(KeyName.trimmed(), Key.trimmed(), Qt::CaseInsensitive))
        {
            //we found key!
            mvGroups[GroupIDX].mvLines[i]=Key.trimmed()+QString("=")+Val;
            isDirty = true;
            return true;
        }
    }//for all keys in group
    if (!isCreateKey)
        return false;

    mvGroups[GroupIDX].mvLines.push_back(Key.trimmed()+QString("=")+Val);
    isDirty = true;
    return true;
}

bool CGDMConf::save()
{
    if(!isDirty)
        return true;
    QFile file(mFileName);
    file.open(QIODevice::WriteOnly);

    QTextStream stream( &file );
    for(int i=0; i<mvGroups.size(); i++)
    {
        for(int j=0; j<mvGroups[i].mvLines.size(); j++)
        {
            stream<<mvGroups[i].mvLines[j]<<"\n";
        }
    }
    file.close();
    return true;
}
