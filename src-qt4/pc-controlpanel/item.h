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

#ifndef ITEM_H
#define ITEM_H

#include <QListWidget>
#include <QSettings>
#include <QStringList>
#include "pbiinfo.h"
#include "deinfo.h"
class QCPItem : public QListWidgetItem
{
public:
	QCPItem();

	/**
	  * Read .desktop file.
	  *
	  * Set item text, icon from file, check item visibility, and store
	  * execute command and executable path
	  *
	  * @param FileName - name of .desktop file to read
	  *
	  * @return TRUE if success and item is visible
	  */
        bool read(QString FileName, const QVector<QString>& vEnabledDE);

	/**
	  * Check for valid data
	  *
	  * @return TRUE if we got right data from file and item must be visible
	  */
	bool isValid();

	/**
	  * Launch control panel item executable
	  *
	  * @return TRUE if success
	  */
	bool launch();

        /**
          Set displaying DE name in item text

          @param isDisplay - True if DE name is displayed
        */
        void setDisplayDEName(bool isDisplay);

        /**
          Returnes item name that was readed from .desktop file
         */
        QString itemName() {return mName; }

        /**
          Returnes original (untranslated) item name
          */
        QString itemOriginalName() {return mOriginalName;};

        QStringList tags() {return mTags;}

        bool isSudoRequired() {return isSudo;}

        bool isRootRequired() {return isRequireRoot;}

protected:
        QString mName;
        QString mOriginalName;
        QString mFileName;
        QString mExecCommand;
        QString mPath;
        QString mPBIPath;
        bool    misValid;
        QString mIconFile;
        bool    misMsgBox;
        QString mMsgBoxText;
        bool    misUseXDG;
        bool    misXDGLaunch;
        QString mRequiredDE;
        bool    misSubstFound;
        QVector<QString> mvEnabledDE;
        bool isRequireRoot;
        bool isSudo;
        QStringList mTags;

        bool getLocalizedField(const QSettings& Reader,
                               const QString& FieldName,
                               QString& RetVal);
        bool launchOwn();
        bool launchXDG();
};

#endif // ITEM_H
