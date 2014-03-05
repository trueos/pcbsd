/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/
#include <QProcess>
#include "snddialog.h"
#include <pcbsd-utils.h>


void sndDialog::programInit()
{
  // Connect our pushbuttons
  connect(pushTest, SIGNAL( clicked() ), this, SLOT( slotDiagnostic() ) );
  connect(pushClose, SIGNAL( clicked() ), this, SLOT( slotClose() ) );
  connect(pushApply, SIGNAL( clicked() ), this, SLOT( slotSave() ) );
  connect(comboSound, SIGNAL( currentIndexChanged(int) ), this, SLOT( slotEnableApply() ) );

  refreshDevices();
}

void sndDialog::slotEnableApply()
{
  pushApply->setEnabled(true);
}

void sndDialog::refreshDevices()
{
  defaultUnit = -1 ;
  comboSound->clear();

  // List the sound devices
  QString tmp, snd;
  QProcess d;
  d.start(QString("cat"), QStringList() << "/dev/sndstat");
  while(d.state() == QProcess::Starting || d.state() == QProcess::Running) {
     d.waitForFinished(200);
     QCoreApplication::processEvents();
  }
    
  int i = 0;
  while (d.canReadLine()) {
    tmp = d.readLine().simplified();
    if ( tmp.indexOf("pcm") == 0 ) {
      comboSound->addItem(tmp);
      if ( tmp.indexOf("default") != -1 ) {
        defaultUnit=i;
        comboSound->setCurrentIndex(i);
      }
      i++;
    }
  } 

  if ( defaultUnit == -1 ) {
     labelDesc->setText(tr("No sound devices detected!"));
     pushTest->setEnabled(false);
     pushApply->setEnabled(false);
     comboSound->setEnabled(false);
  }
     
  pushApply->setEnabled(false);
}

// Close the dialog and exit
void sndDialog::slotClose()
{
  close();
}

void sndDialog::slotSave()
{
  QString tmp;
  tmp.setNum(comboSound->currentIndex());
  system("sysctl hw.snd.default_unit=" + tmp.toLatin1());

  // Try to set the default pulseaudio sink as well
  QString username = QString::fromLocal8Bit(getenv("LOGNAME"));
  system("su " + username.toLatin1() + " -c \"pactl set-default-sink " + tmp.toLatin1() + "\"");

  pcbsd::Utils::setConfFileValue( "/etc/sysctl.conf", "hw.snd.default_unit=", "hw.snd.default_unit=" + tmp );
  refreshDevices();
}

// Play some sound for the user to detect this new device
void sndDialog::slotDiagnostic()
{
  QString tmp;
  tmp.setNum(comboSound->currentIndex());

  // Change the sysctl, and play a sound
  system("sysctl hw.snd.default_unit=" + tmp.toLatin1());
  system("mixer vol 100:100");
  system("mixer pcm 100:100");
  system("mixer speaker 100:100");

  QProcess d;
  d.start(QString("mplayer"), QStringList() << "/usr/local/share/sounds/testsound.ogg");
  while(d.state() == QProcess::Starting || d.state() == QProcess::Running) {
     d.waitForFinished(200);
     QCoreApplication::processEvents();
  }

  tmp.setNum(defaultUnit);
  system("sysctl hw.snd.default_unit=" + tmp.toLatin1());
}
