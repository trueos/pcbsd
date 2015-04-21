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
    if(active[i].endsWith("=0") || sysctl.filter(active[i].section(".active=",0,0)+".brightness=").isEmpty() ){
      //inactive or no corresponding brightness control - remove it
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
  int mode = -1;
  if(percent.startsWith("+")){ percent.remove("+"); mode = 0; } //increase by percent - not full number given
  else if(percent.startsWith("-")){ percent.remove("-"); mode = 1; } //decrease by percent
  //Now get the current LCD brightness controls/values
  QStringList sysctl = runShellCommand("sysctl -ae").filter(".lcd");
  //See if there are any active screens
  QStringList active = sysctl.filter(".active=");
  for(int i=0; i<active.length(); i++){
    if(active[i].endsWith("=0") || sysctl.filter(active[i].section(".active=",0,0)+".brightness=").isEmpty() ){
      //inactive or no corresponding brightness control - remove it
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
    //Ensure that the value is one of the ones listed as valid ("levels of brightness")
    QString levvar = active[i].section(".active=",0,0)+".levels=";
    if(!sysctl.filter(levvar).isEmpty()){
      //This screen has levels defined - adjust to match the closest one
      QStringList levels = sysctl.filter(levvar).first().section("=",1,1).split(" ");
      if(levels.contains(newval)){} //matches already - just continue on
      else if(levels.contains(QString::number(newval.toInt()+1)) ){ newval = QString::number(newval.toInt()+1); } //just bump it up by 1 (some screens operate on even/odd values)
      else{
	//Need to be a bit smarter about this and check each level to find which is closer
	// "wobble" around the initial value until the closest valid level is found (less computations for potentially large lists)
	// This is also a lot faster than trying to sort/convert the list to numbers before checking values
	int div=levels.length();
	if(div>10){ div = (levels.length()/10)*10; } //always rounds down to the nearest multiple of 10 (integer chop)
	div = 100/div; //This defines the range of values to search around the initial value
	//Example so far: for 20->30 levels, "div"=5, so we only need to check the +/- 5 values before we will hit something
	int perc = newval.toInt();
	for(int i=1; i<div+1; i++){ //remember the +1 offset in counting numbers
          //Check positive and negative changes of this value
	  if(levels.contains( QString::number(perc+i) )){ newval = QString::number(perc+i); break; }
	  else if(levels.contains( QString::number(perc-i) )){ newval = QString::number(perc-i); break; }
	}
      }
    }
     //Now set the new value
    runShellCommand("sysctl "+var+"="+newval);   
  }//go to the next active screen

  return "[SUCCESS]";
  
}