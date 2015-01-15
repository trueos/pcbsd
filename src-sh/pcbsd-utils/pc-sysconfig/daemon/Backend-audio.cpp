#include "Backend.h"
#include <QDebug>

/*
This is the audio subsystems control backend
*/

QString Backend::setDefaultAudioDevice(QString pcmdevice){
  QString deviceNumber = pcmdevice.section("pcm",-1);
  QProcess::execute("sysctl hw.snd.default_unit=" + deviceNumber);
  // Try to set the default pulseaudio sink as well
  system("su " + CUSER.toLatin1() + " -c \"pactl set-default-sink " + deviceNumber.toLatin1() + "\"");

  //Now save this setting into sysctl.conf
  QStringList fcontents = readFile("/etc/sysctl.conf");
  bool found = false;
  //Try to replace the line if it exists already
  for(int i=0; i<fcontents.length() && !found; i++){
    if(fcontents[i].startsWith("hw.snd.default_unit=")){
      fcontents[i] = "hw.snd.default_unit=" + deviceNumber;
      found = true;
    }
  }
  if(!found){ fcontents << "hw.snd.default_unit=" + deviceNumber; } //append to the end of the file
  writeFile("/etc/sysctl.conf", fcontents, true);
  return "[SUCCESS]";	
}

QStringList Backend::ListAudioDevices(){
  QStringList contents = runShellCommand("cat /dev/sndstat"); //not a normal text file
  QStringList out;
  //qDebug() << "/dev/sndstat:" << contents;
  for(int i=0; i<contents.length(); i++){
    if(contents[i].startsWith("pcm")){
      out << contents[i];
    }
  }
  return out;
}