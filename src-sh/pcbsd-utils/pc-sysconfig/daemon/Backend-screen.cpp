#include "Backend.h"
#include <QDebug>

/*
This is the screen control subsystems backend
*/
QString Backend::getScreenBrightness(){
  //Get the current LCD brightness controls/values
  QStringList sysctl = runShellCommand("sysctl -ae").filter(".lcd");
  //See if there are any active screens
  QStringList active = sysctl.filter(".active=");
  for(int i=0; i<active.length(); i++){
    if(active[i].endsWith("=0")){
      //inactive - remove it
      active.removeAt(i);
      i--;
    }
  }
  if(active.isEmpty()){ return "[ERROR]"; } //no screens available to be managed
  //Now go through and get the brightness on the first available screen
  for(int i=0; i<active.length(); i++){
    //First get the variable
    QString var = active[i];
      var.replace(".active=", ".brightness="); //change to the brightness controller
      var = var.section("=",0,0); //only need the variable, not the value
    //Now get the value for the new variable
    QString out = sysctl.filter(var).first().section("=",1,1);
    return out;
  }
  //If it gets this far, there was an error
  return "[ERROR]";
}

QString Backend::setScreenBrightness(QString percent){
  //Quick checks for possible symbols in the input
  percent.remove("%");
  percent = percent.section(".",0,0).section(",",0,0); //only whole numbers
  bool mode = -1;
  if(percent.startsWith("+")){ percent.remove("+"); mode = 0; } //increase by percent - not full number given
  else if(percent.startsWith("-")){ percent.remove("-"); mode = 1; } //decrease by percent
  //Now get the current LCD brightness controls/values
  QStringList sysctl = runShellCommand("sysctl -ae").filter(".lcd");
  //See if there are any active screens
  QStringList active = sysctl.filter(".active=");
  for(int i=0; i<active.length(); i++){
    if(active[i].endsWith("=0")){
      //inactive - remove it
      active.removeAt(i);
      i--;
    }
  }
  if(active.isEmpty()){ return "[ERROR]"; } //no screens available to be managed
  //Now go through and set the brightness on each available screen
  for(int i=0; i<active.length(); i++){
    //First get the variable
    QString var = active[i];
      var.replace(".active=", ".brightness="); //change to the brightness controller
      var = var.section("=",0,0); //only need the variable, not the value
    //Now determine what the new value should be
    QString newval = percent;
    if(mode>=0){
      //Need the current value to adjust it
      int diff = percent.toInt();
      int oval = sysctl.filter(var).first().section("=",1,1).toInt();
      if(mode==0){ //increase value
	int nval = oval+diff;
	if(nval>100){ nval = 100; }//only need to check upper limit in this case
        newval = QString::number(nval); 
      }else if(mode==1){ //decrease value
	int nval = oval-diff;
	if(nval<0){ nval = 0; }//only need to check lower limit in this case
        newval = QString::number(nval); 	
      }
    }else{
      //input percentage is the requested value, just check the limits for validity
      int nval = newval.toInt();
      if(nval<0){ newval="0"; }
      else if(nval>100){ newval="100"; }
    }
     //Now set the new value
    runShellCommand("sysctl "+var+"="+newval);   
  }//go to the next active screen

  return "[SUCCESS]";
  
}