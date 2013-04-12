/***************************************************************************
 *   Copyright (C) 2011 by iXsystems					   *
 *   kris@pcbsd.org							   *
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
#include <QDebug>

#include "classes.h"

// Repo class data
Repo::Repo() { }
Repo::~Repo() { }

void Repo::addRepo(QString id, QString desc, QString indexURL, QString Mirrors, QString index, QString meta, QString md5)
{
	repoID.append(id);
	repoDesc.append(desc);
	repoIndexURL.append(indexURL);
	repoMirrors.append(Mirrors);
	repoIndex.append(index);
	repoMeta.append(meta);
	repoMd5.append(md5);
	//qDebug() << "Added repo:" << id << desc << indexURL << Mirrors << index << meta;
}

QString Repo::getID(int rnum)
{
	return repoID.at(rnum);
}

QString Repo::getDesc(int rnum)
{
	return repoDesc.at(rnum);
}

QString Repo::getIndexURL(int rnum)
{
	return repoIndexURL.at(rnum);
}

QString Repo::getIndex(int rnum)
{
	return repoIndex.at(rnum);
}

QString Repo::getMeta(int rnum)
{
	return repoMeta.at(rnum);
}

QString Repo::getMd5(int rnum)
{
	return repoMd5.at(rnum);
}

QString Repo::getMirrors(int rnum)
{
	return repoMirrors.at(rnum);
}

int Repo::count()
{
	return repoID.count();
}

// Category Class Data
Category::Category() { }
Category::~Category() { }

void Category::addCat(QString name, QString desc, QString icon, QString repo)
{
	catName.append(name);
	catDesc.append(desc);
	catIcon.append(icon);
	catRepo.append(repo);
	//qDebug() << "Added Category:" << name << desc << icon << repo;
}

QString Category::getName(int cnum)
{
	return catName.at(cnum);
}

QString Category::getDesc(int cnum)
{
	return catDesc.at(cnum);
}

QString Category::getIcon(int cnum)
{
	return catIcon.at(cnum);
}

QString Category::getRepo(int cnum)
{
	return catRepo.at(cnum);
}

int Category::count()
{
	return catName.count();
}

// AvailPBI Class Data
AvailPBI::AvailPBI() { }
AvailPBI::~AvailPBI() { }

void AvailPBI::addPBI(QString name, QString ver, QString date, QString arch, QString auth, QString url, QString lic, QString type, QString keys, QString desc, QString icon, QString repo, QString cat, bool root, QString size)
{
	pbiName.append(name);
	pbiVer.append(ver);
	pbiDate.append(date);
	pbiArch.append(arch);
	pbiAuth.append(auth);
	pbiURL.append(url);
	pbiLic.append(lic);
	pbiType.append(type);
	pbiKeys.append(keys);
	pbiDesc.append(desc);
	pbiIcon.append(icon);
	pbiRepo.append(repo);
	pbiCat.append(cat);
	pbiSize.append(size);
	if ( root )
  	  pbiRoot.append("YES");
	else
  	  pbiRoot.append("NO");
	//qDebug() << "Added PBI:" << name << desc << icon << repo << cat;
}

QString AvailPBI::getCat(int pnum)
{
	return pbiCat.at(pnum);
}

QString AvailPBI::getSize(int pnum)
{
	return pbiSize.at(pnum);
}

QString AvailPBI::getRepo(int pnum)
{
	return pbiRepo.at(pnum);
}

QString AvailPBI::getIcon(int pnum)
{
	return pbiIcon.at(pnum);
}

QString AvailPBI::getDesc(int pnum)
{
	return pbiDesc.at(pnum);
}

QString AvailPBI::getKeys(int pnum)
{
	return pbiKeys.at(pnum);
}

QString AvailPBI::getType(int pnum)
{
	return pbiType.at(pnum);
}

QString AvailPBI::getLic(int pnum)
{
	return pbiLic.at(pnum);
}

QString AvailPBI::getURL(int pnum)
{
	return pbiURL.at(pnum);
}

QString AvailPBI::getAuth(int pnum)
{
	return pbiAuth.at(pnum);
}

QString AvailPBI::getArch(int pnum)
{
	return pbiArch.at(pnum);
}

QString AvailPBI::getDate(int pnum)
{
	return pbiDate.at(pnum);
}

QString AvailPBI::getVer(int pnum)
{
	return pbiVer.at(pnum);
}

QString AvailPBI::getName(int pnum)
{
	return pbiName.at(pnum);
}

bool AvailPBI::getRoot(int pnum)
{
	if ( pbiRoot.at(pnum) == "YES" )
		return true;
	return false;
}

int AvailPBI::count()
{
	return pbiName.count();
}
