/**************************************************************************
*   Copyright (C) 2013- by Yuri Momotyuk                                   *
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

#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QStringList>

#define __string_constant static const char* const



inline bool parseFetchOutput(QString line, long& size, long& downloaded, QString& speed)
{
    __string_constant SIZE_DL_MARKER = "SIZE:";
    __string_constant DOWNLOADED_DL_MARKER = "DOWNLOADED:";

    //Example:
    // SIZE: 215710 DOWNLOADED: 3973 SPEED: 233 KB/s
    // ^0    ^1     ^2          ^3   ^4     ^5  ^6
    if (line.contains(SIZE_DL_MARKER) && line.contains(DOWNLOADED_DL_MARKER))
    {
        QStringList dl_list = line.split(" ");
        speed= dl_list[5] + QString(" ") + dl_list[6];
        size= dl_list[1].toInt() * 1024;
        downloaded= dl_list[3].toInt() * 1024;
        return true;
    }
    return false;
}


#endif // UTILS_H
