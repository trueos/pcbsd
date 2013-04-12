/***************************************************************************
 *   Copyright (C) 2011 by iXsystems   					   *
 *   kris@pcbsd.org 							   *
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
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. *
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/

#include <QAbstractButton>
#include <QMouseEvent>
#include <QStringList>
#include <QWidget>

#ifndef INC_CLASSES_H
#define INC_CLASSES_H

class Repo {

public:
    //Constructors
    Repo();
    ~Repo();
    
    //Getters
    virtual QString getID(int rnum);
    virtual QString getDesc(int rnum);
    virtual QString getIndexURL(int rnum);
    virtual QString getMd5(int rnum);
    virtual QString getMeta(int rnum);
    virtual QString getMirrors(int rnum);
    virtual QString getIndex(int rnum);
    virtual int count();
    
    //Setters
    virtual void addRepo(QString id, QString desc, QString indexURL, QString mirrors, QString index, QString meta, QString md5);

private:
    QStringList repoID, repoDesc;
    QStringList repoIndexURL, repoMirrors, repoIndex, repoMeta, repoMd5;
};

class Category {

public:
    //Constructors
    Category();
    ~Category();
    
    //Getters
    virtual QString getName(int cnum);
    virtual QString getDesc(int cnum);
    virtual QString getIcon(int cnum);
    virtual QString getRepo(int cnum);
    virtual int count();
    
    //Setters
    virtual void addCat(QString name, QString desc, QString icon, QString repo);

private:
    QStringList catName, catDesc, catIcon, catRepo;
};


class AvailPBI {

public:
    //Constructors
    AvailPBI();
    ~AvailPBI();
    
    //Getters
    virtual QString getName(int pnum);
    virtual QString getVer(int pnum);
    virtual QString getDate(int pnum);
    virtual QString getArch(int pnum);
    virtual QString getAuth(int pnum);
    virtual QString getURL(int pnum);
    virtual QString getLic(int pnum);
    virtual QString getType(int pnum);
    virtual QString getKeys(int pnum);
    virtual QString getDesc(int pnum);
    virtual QString getIcon(int pnum);
    virtual QString getRepo(int pnum);
    virtual QString getCat(int pnum);
    virtual QString getSize(int pnum);
    virtual bool getRoot(int pnum);
    virtual int count();
    
    // Setters
    virtual void addPBI(QString name, QString ver, QString date, QString arch, QString auth, QString url, QString lic, QString type, QString keys, QString desc, QString icon, QString repo, QString cat, bool root, QString size);

private:
    QStringList pbiName, pbiVer, pbiDate, pbiArch, pbiAuth, pbiURL, pbiLic, pbiType, pbiKeys, pbiCat, pbiDesc, pbiIcon, pbiRepo, pbiRoot, pbiSize;
    
};

#endif
