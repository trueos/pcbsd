#include "syscontroller.h"

#include <QDebug>

const QString UPDATE_MANAGER = "pc-updatemanager";
#define UM_CHECK_PARAMS "check"

CSysController::CSysController()
{
    mUpdProcess.setProcessChannelMode(QProcess::MergedChannels);
    connect(&mUpdProcess, SIGNAL(readyReadStandardOutput()),
            this, SLOT(slotProcessRead()));
}

void CSysController::parseProcessLine(QString line)
{
    switch(curentState())
    {
        case eCHECKING:
            checkReadLine(line);
            break;

        case eUPDATING:
            updateReadLine(line);
            break;

        default: //fuckup here
        qDebug()<<"Unknown state of update manager";
    }
}

void CSysController::onCheckUpdates()
{

    mUpdProcess.start(UPDATE_MANAGER, QStringList() << UM_CHECK_PARAMS);

}

void CSysController::onUpdateAll()
{

}

void CSysController::slotProcessRead()
{
    while (mUpdProcess.canReadLine())
        parseProcessLine(mUpdProcess.readLine().simplified());

}

void CSysController::checkReadLine(QString line)
{
    if (line == QString("Your system is up to date!"))
    {
        setCurrentState(eFULLY_UPDATED);
    }
    //-----------------------------------------------------
    // Code is form original update manager my Kris or Ken:
    //-----------------------------------------------------

     /*static QString line, tmp, name, type, version, date, tag, url, size, sa, rr;

     if ( line.indexOf("NAME: ") == 0) {
            name = line.replace("NAME: ", "");
            return;
         }
         if ( line.indexOf("TYPE: ") == 0) {
            type = line.replace("TYPE: ", "");
            return;
         }

         if ( type == "SYSUPDATE" ) {
           if ( line.indexOf("VERSION: ") == 0) {
              version = line.replace("VERSION: ", "");
              return;
           }
           if ( line.indexOf("DATE: ") == 0) {
              date = line.replace("DATE: ", "");
              return;
           }
           if ( line.indexOf("TAG: ") == 0) {
              tag = line.replace("TAG: ", "");
              return;
           }
           if ( line.indexOf("DETAILS: ") == 0) {
              url = line.replace("DETAILS: ", "");
              return;
           }

           if ( line.indexOf("To install:") == 0) {
              up.clear();
          up << name << type << version << date << tag << url;
          listUpdates.append(up);
              type=""; name="", version="", date="", tag="", url="";
              return;
           }

         }
         if ( type == "PATCH" ) {
           if ( line.indexOf("DATE: ") == 0) {
              date = line.replace("DATE: ", "");
              return;
           }
           if ( line.indexOf("TAG: ") == 0) {
              tag = line.replace("TAG: ", "");
              return;
           }
           if ( line.indexOf("SIZE: ") == 0) {
              size = line.replace("SIZE: ", "");
              return;
           }
           if ( line.indexOf("STANDALONE: ") == 0) {
              sa = line.replace("STANDALONE: ", "");
              return;
           }
           if ( line.indexOf("REQUIRESREBOOT: ") == 0) {
              rr = line.replace("REQUIRESREBOOT: ", "");
              return;
           }
           if ( line.indexOf("DETAILS: ") == 0) {
              url = line.replace("DETAILS: ", "");
              return;
           }
           if ( line.indexOf("To install:") == 0) {
          // TODO add this update to list
         //     up.clear();
         // up << name << type << date << tag << size << sa << rr << url;
         // listUpdates.append(up);
              type=""; name="", date="", tag="", size="", sa="", rr="", url="";
              continue;
         //  }
          }
         }
         */
    qDebug()<<line;

}

void CSysController::updateReadLine(QString line)
{

}
