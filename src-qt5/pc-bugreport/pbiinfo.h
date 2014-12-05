/**************************************************************************
*   Copyright (C) 2011 by Yuri Momotyuk                                   *
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
#ifndef PBIINFO_H
#define PBIINFO_H

#include <QString>
#include <QVector>

typedef struct _SPBIInfo
{
	QString mGenericName;
	QString mName;
	QString mVersion;
	QString mPrefix;
	QString mAuthor;
	QString mWebsite;
	QString mIcon;
	QString mFreeBSDVersion;
	QString mArch;
	QString mRepo;
	QString mCreatorVersion;
	QString mBuildTimeString;
	bool misDesktopIconsInstalled;
	bool misMenuIconsInstalled;
	bool misSignatureVerified;

}SPBIInfo;


class CInstalledPBI:public QVector<SPBIInfo>
{
 public:
	/**
	  Fill conteiner by info for installed pbis
	*/
	int refresh();

	/**
	  Find pbi item by name

	  \param What - Name to find
	  \param [out] - Reference to instance where return value will be placed
	  \param cs - search case sensativity

	  \return TRUE if found. Item will be placed in RetVal
	*/
	bool findByName(const QString What, SPBIInfo& RetVal,
					Qt::CaseSensitivity cs = Qt::CaseSensitive);

protected:
};

//! Singletone object with installed pbi
class CSingleInstalledPBI: public CInstalledPBI
{
public:
    static CSingleInstalledPBI& getRef();
};

//! singletone instance with installed pbi list
static CSingleInstalledPBI& InstalledPBIs = CSingleInstalledPBI::getRef();
#endif // PBIINFO_H
