#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QStringList>

#define _STRING_CONSTANT static const char* const



inline bool parseFetchOutput(QString line, long& size, long& downloaded, QString& speed)
{
    static const char* const SIZE_DL_MARKER = "SIZE:";
    static const char* const DOWNLOADED_DL_MARKER = "DOWNLOADED:";

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
