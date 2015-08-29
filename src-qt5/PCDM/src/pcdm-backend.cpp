/* PCDM Login Manager:
*  Written by Ken Moore (ken@pcbsd.org) 2012/2013
*  Copyright(c) 2013 by the PC-BSD Project
*  Available under the 3-clause BSD license
*/

#include <QProcess>
#include <QProcessEnvironment>
#include <QTemporaryFile>
#include <QTextStream>
#include <QDebug>
#include <QStringList>

#include "pcdm-backend.h"
#include "pcdm-config.h"
#include "pcbsd-utils.h"

QStringList displaynameList,usernameList,homedirList,usershellList,instXNameList,instXBinList,instXCommentList,instXIconList,instXDEList,excludedUsers;
QString logFile;
QString saveX,saveUsername, lastUser, lastDE;
bool Over1K = true;

QStringList Backend::getAvailableDesktops(){  
  if(instXNameList.isEmpty()){ loadXSessionsData(); }
  QStringList out = instXNameList;
  return out;
}

QString Backend::getDesktopComment(QString xName){
  if(instXNameList.isEmpty()){ loadXSessionsData(); }
  int index = instXNameList.indexOf(xName);
  if(index == -1){ Backend::log("PCDM: Invalid Desktop Name: " + xName); return ""; }
  return instXCommentList[index];
}

QString Backend::getNLDesktopName(QString xName){
  //Get the non-localized desktop name from the localized version
  if(instXNameList.isEmpty()){ loadXSessionsData(); }
  int index = instXNameList.indexOf(xName);
  if(index == -1){ Backend::log("PCDM: Invalid Desktop Name: " +xName); return ""; }
  return instXDEList[index];	
}

QString Backend::getDesktopIcon(QString xName){
  if(instXNameList.isEmpty()){ loadXSessionsData(); }
  int index = instXNameList.indexOf(xName);
  if(index == -1){ Backend::log("PCDM: Invalid Desktop Name: " +xName); return ""; }
  return instXIconList[index];
}

QString Backend::getDesktopBinary(QString xName){
  if(instXNameList.isEmpty()){ loadXSessionsData(); }
  int index = instXNameList.indexOf(xName);
  if(index == -1){ Backend::log("PCDM: Invalid Desktop Name: " + xName); return ""; }
  return instXBinList[index];
}

void Backend::allowUidUnder1K(bool allow, QStringList excludes){
  Over1K = !allow;
  excludedUsers = excludes;
  //Make sure to re-load the user list if necessary
  readSystemUsers();
}


QStringList Backend::getSystemUsers(bool realnames){
  if(usernameList.isEmpty()){
    readSystemUsers();
  }
  if(realnames){
    return displaynameList;
  }else{
    return usernameList;
  }
}

QString Backend::getALUsername(){
  //Make sure the requested user is valid
  readSystemUsers(); //first read the available users on this system
  QString ruser = Config::autoLoginUsername();
  int index = usernameList.indexOf(ruser);
  if(index == -1){ //invalid username
    //Check if a display name was given instead
    index = displaynameList.indexOf(ruser);
    if(index == -1){ //invalid display name
      log("Invalid Auto-Login user requested - skipping....");
      ruser.clear();
    }else{
      //use the valid username for the given display name
      ruser = usernameList[index]; 
    }
  }
  return ruser;
}

QString Backend::getALPassword(){
  QString rpassword = Config::autoLoginPassword();
  return rpassword;
}

QString Backend::getUsernameFromDisplayname(QString dspname){
  if(dspname.isEmpty()){return "";}
  int i = displaynameList.indexOf(dspname);
  if(i == -1){ i = usernameList.indexOf(dspname); }
  
  if(i == -1){ return ""; }
  else{ return usernameList[i]; }
}

QString Backend::getDisplayNameFromUsername(QString username){
  if(username.isEmpty()){return "";}
  int i = usernameList.indexOf(username);
  if(i==-1){ i = displaynameList.indexOf(username); } //make sure it was not a display name passed in
  if(i==-1){ return ""; }
  else{
    return displaynameList[i];  
  }
}

QString Backend::getUserHomeDir(QString username){
  if(username.isEmpty()){ return ""; }
  int i = usernameList.indexOf(username);
  if( i == -1 ){ i = displaynameList.indexOf(username); }
  if( i < 0){ return ""; }
  return homedirList[i];
}

QString Backend::getUserShell(QString username){
  if(username.isEmpty()){ return ""; }
  int i = usernameList.indexOf(username);
  if( i == -1 ){ i = displaynameList.indexOf(username); }
  if( i < 0){ return ""; }
  return usershellList[i];	
}

QStringList Backend::keyModels()
{
    QStringList _models;
    QString code, desc, line;

    Process p(QStringList() << "xkeyboard-models");

    if (p.waitForFinished()) {
        while (p.canReadLine()) {
            line = p.readLine();
            code = line;
            code.truncate(line.indexOf(" "));
            desc = line.remove(0, line.indexOf(" "));
            _models.append(desc.simplified() + " - (" + code.simplified() + ")");
        }
    }
    return _models;
}

QStringList Backend::keyLayouts()
{
    QStringList _layouts;
    QString code, desc, line;

    Process p(QStringList() << "xkeyboard-layouts");

    if (p.waitForFinished()) {
        while (p.canReadLine()) {
            line = p.readLine();
            code = line;
            code.truncate(line.indexOf(" "));
            desc = line.remove(0, line.indexOf(" "));
            _layouts.append(desc.simplified() + " - (" + code.simplified() + ")");
        }
    }
    return _layouts;
}

// Function which gets the key Variants for the target layout
QStringList Backend::keyVariants(const QString &layout, QStringList &savedKeyVariants)
{
    QStringList _variants;
    QString code, desc, line;

    if ( savedKeyVariants.empty() )
    {
      Process p(QStringList() << "xkeyboard-variants");
      if (p.waitForFinished()) {
        while (p.canReadLine()) {
            line = p.readLine();
            savedKeyVariants << line;
        }
      }
    }

    for (int i = 0; i < savedKeyVariants.size(); ++i) {
       // Look for variants for this particular layout
       line = savedKeyVariants.at(i);
       if ( line.indexOf(" " + layout + ":") != -1 )
       {
         code = line.simplified();
         code.truncate(code.indexOf(" "));
         desc = line.remove(0, line.indexOf(": ") + 1);
         _variants.append(desc.simplified() + " - (" + code.simplified() + ")");
       }
    }

    return _variants;
}

// Function which lets us run setxkbmap
bool Backend::changeKbMap(QString model, QString layout, QString variant)
{
   QProcess kbp;
	kbp.setProcessChannelMode(QProcess::MergedChannels);
   QStringList args;
   QString prog;
   prog = "setxkbmap"; 
   if(!model.isEmpty()){ args << "-model" << model; }
   if(!layout.isEmpty()){ args << "-layout" << layout; }
   if(!variant.isEmpty()){ args << "-variant" << variant; }
   Backend::log("setxkbmap: " + args.join(" "));
   kbp.start(prog, args);
   kbp.waitForFinished();
   bool ok = (kbp.exitCode() == 0);
   if(!ok){
     Backend::log("setxkbmap Failed: "+QString(kbp.readAllStandardOutput()) );
   }
   return ok;
}

QStringList Backend::languages()
{
    QStringList _languages;
    _languages.append("Default - (en_US)"); //make sure this is always at the top of the list
    QString code, desc, line;

    QFile mFile;
    mFile.setFileName("/usr/share/pc-sysinstall/conf/avail-langs");
    if ( ! mFile.open(QIODevice::ReadOnly | QIODevice::Text))
       return QStringList();

    // Read in the meta-file for categories
    QTextStream in(&mFile);
    in.setCodec("UTF-8");
    while ( !in.atEnd() ) {
       line = in.readLine();
       code = line;
       code.truncate(line.indexOf(" "));
       desc = line.remove(0, line.indexOf(" "));
        _languages.append(desc.simplified() + " - (" + code.simplified() + ")");
    }
    mFile.close();
    return _languages;
}

void Backend::openLogFile(QString logFilePath){
  //If a log file exists, move it to *.old
  if(QFile::exists(logFilePath)){ 
    if(QFile::exists(logFilePath+".old")){ QFile::remove(logFilePath+".old"); }
    QFile::rename(logFilePath, logFilePath+".old"); 
  }
  //save the path to the logfile
  logFile = logFilePath;
}

void Backend::log(QString line){
  qDebug() << line; //quick replacement to verify the new logging system works
  /*QFile lFile(logFile);
  lFile.open(QIODevice::Append);
  QTextStream out(&lFile);
  out << line << "\n";
  lFile.close();*/
}

void Backend::checkLocalDirs(){
  //Check for directories first
  QString base = "/usr/local/share/PCDM";
  QDir mainDir(base);
  if(!mainDir.exists()){ mainDir.mkdir(base); }
  if(!mainDir.exists("themes")){ mainDir.mkdir("themes"); }
  //Check for sample files
  if(!mainDir.exists("pcdm.conf.sample")){ QFile::copy(":samples/pcdm.conf",base+"/pcdm.conf.sample"); } 
  //Check for the PCDM runtime directory
  mainDir.cd(DBDIR);
  if(!mainDir.exists()){ mainDir.mkpath(DBDIR); }
}

QString Backend::getLastUser(){
  //Load the file if necessary
  if(lastUser.isEmpty()){
    readSystemLastLogin();  
  }
  //return the value
  QString user = getDisplayNameFromUsername(lastUser);
  return user;
}

QString Backend::getLastDE(QString user){
  if(user.isEmpty()){ return ""; }
  if(lastDE.isEmpty()){
    readSystemLastLogin();
  }
  QString de = readUserLastDesktop(user);
  if(de.isEmpty()){ return lastDE; }
  else{ return de; }
  
}

void Backend::saveLoginInfo(QString user, QString desktop){
  writeSystemLastLogin(user,desktop); //save the system file (DBDIR/lastlogin)
  writeUserLastDesktop(user,desktop); //save the user file (~/.lastlogin)
}

void Backend::readDefaultSysEnvironment(QString &lang, QString &keymodel, QString &keylayout, QString &keyvariant){
  //Set the default values
    lang = "en_US";
    keymodel = "pc104";
    keylayout = "us";
    keyvariant = "";
  //Read the current inputs file and overwrite default values
  QFile file(DBDIR+"defaultInputs");
  bool goodFile=false;
  if(file.exists()){
    if(file.open(QIODevice::ReadOnly | QIODevice::Text) ){
      QTextStream in(&file);
      while(!in.atEnd()){
        QString line = in.readLine();
	QString var = line.section("=",0,0).simplified();
	QString val = line.section("=",1,1).simplified();
	if(var=="Lang"){ lang = val;}
	else if(var=="KeyModel"){ keymodel = val; }
	else if(var=="KeyLayout"){ keylayout = val; }
	else if(var=="KeyVariant"){ keyvariant = val; }
      }
      file.close();
    }
  }
  if(!goodFile){
    //Save our own defaults
    saveDefaultSysEnvironment(lang, keymodel, keylayout, keyvariant);
  }	
}

void Backend::saveDefaultSysEnvironment(QString lang, QString keymodel, QString keylayout, QString keyvariant){
  QFile file(DBDIR+"defaultInputs");
  //Make sure the containing directory exists
  if(!QFile::exists(DBDIR)){
    QDir dir;
    dir.mkpath(DBDIR);
  }
  //Now save the file
    if(file.open(QIODevice::WriteOnly | QIODevice::Text) ){
      QTextStream out(&file);
      out << "Lang=" + lang + "\n";
      out << "KeyModel=" + keymodel + " \n";
      out << "KeyLayout=" + keylayout + " \n";
      out << "KeyVariant=" + keyvariant + " \n";
      file.close();
    }
}

QStringList Backend::getRegisteredPersonaCryptUsers(){
  //This is just a quick check to see what users are personacrypt-enabled on this system
  //  needs to do any of the personacrypt stuff on this system
  if( !QFile::exists("/usr/local/bin/personacrypt") ){ return QStringList(); } //not installed
  //Make sure there is at least one profile available
  QDir dir("/var/db/personacrypt");
  QStringList users = dir.entryList(QStringList()<<"*.key", QDir::Files | QDir::NoDotAndDotDot, QDir::Name); 
  for(int i=0; i<users.length(); i++){
    users[i].chop(4); //chop the ".key" off the end to get the username
  }
  return users;
}

QStringList Backend::getAvailablePersonaCryptUsers(){
  QStringList info = pcbsd::Utils::runShellCommand("personacrypt list");
  QStringList users;
  for(int i=0; i<info.length(); i++){
    if(info[i].contains(" on ")){
      users << info[i].section(" on ",0,0);
    }
  }
  return users;
}

bool Backend::MountPersonaCryptUser(QString user, QString pass){
  //First, the password needs to be saved to a temporary file for input
  QTemporaryFile tmpfile("/tmp/.XXXXXXXXXXXXXXXXXXXX"); //just a long/hidden randomized name
    tmpfile.setAutoRemove(true);
  if( !tmpfile.open() ){ return false; } //could not open the temporary file
  QTextStream out(&tmpfile);
  out << pass;
  tmpfile.close();
  
  //Second, the mount command needs to be run (be careful about spaces in the names)
  return 0==QProcess::execute("personacrypt mount \""+user+"\" \""+tmpfile.fileName()+"\"");
  //Finally, delete the temporary input file (if personacrypt did not already)
    //QTemporaryFile automatically tries to delete the file when it goes out of scope
    // no need to manually remove it
}

bool Backend::UnmountPersonaCryptUser(QString user){
  return 0==QProcess::execute("personacrypt umount \""+user+"\"");
}


bool Backend::writeFile(QString fileName, QStringList contents){
  //Open the file with .tmp extension
  QFile file(fileName+".tmp");
  if( !file.open(QIODevice::WriteOnly | QIODevice::Text) ){
    qDebug() << fileName+".tmp: Failure -- Could not open file";
    return false;
  }
  //Write the file
  QTextStream ofile(&file); //start the output stream
  for(int i=0; i<contents.length(); i++){
    ofile << contents[i];
    ofile << "\n";
  }
  //Close the File
  file.close();
  //Remove any existing file with the final name/location
  if( QFile::exists(fileName) ){
    if( !QFile::remove(fileName) ){
      qDebug() << fileName+": Error -- Could not overwrite existing file";
      QFile::remove(fileName+".tmp");
      return false;
    }
  }
  //Move the temporary file into its final location
  if( !file.rename(fileName) ){
    qDebug() << fileName+": Error: Could not rename "+fileName+".tmp as "+fileName;
    return false;
  }
  //Return success
  QString extra = QDir::homePath(); //remove this from the filename display
  qDebug() << "Saved:" << fileName.replace(extra,"~");
  return true;;	
}


//****** PRIVATE FUNCTIONS ******

void Backend::loadXSessionsData(){
  //Clear the current variables
  instXNameList.clear(); instXBinList.clear(); 
  instXCommentList.clear(); instXIconList.clear();
  instXDEList.clear();
  //Load the default paths/locale
  QString xDir = Config::xSessionsDir();
  QStringList paths = QString(getenv("PATH")).split(":");
  if(paths.isEmpty()){ paths <<"/usr/local/bin" << "/usr/local/sbin" << "/usr/bin" << "/usr/sbin" << "/bin" << "/sbin"; }
  if(!xDir.endsWith("/")){ xDir.append("/"); }
  QString xIconDir = Config::xSessionsImageDir();
  if(!xIconDir.endsWith("/")){ xIconDir.append("/"); }
  QString localeCode = QLocale().name(); //gets the current locale code
  //Find all *.desktop files
  QDir dir(xDir);
  QStringList deFiles = dir.entryList(QStringList() << "*.desktop", QDir::Files, QDir::Name);
  //deFiles = deFiles.filter(".desktop"); //only get *.desktop files
  //Read each file to see if that desktop is installed
  for(int i=0; i<deFiles.length(); i++){
    QStringList tmp = readXSessionsFile(xDir+deFiles[i],localeCode);
    //tmp[exec, name, comment, icon, tryexec]
    if(!tmp.isEmpty()){
      //Complete file paths if necessary
      //if(!tmp[0].startsWith("/")){ tmp[0] = "/usr/local/bin/"+tmp[0]; }
      if(!tmp[3].startsWith("/")&&!tmp[3].startsWith(":")&&!tmp[3].isEmpty()){ tmp[3] = xIconDir+tmp[3]; }
      if(!tmp[4].startsWith("/") && !QFile::exists(tmp[4])){ 
	for(int p=0; p<paths.length(); p++){
	  if(QFile::exists(paths[p]+"/"+tmp[4])){
	    tmp[4] = paths[p]+"/"+tmp[4];
	  }
	}
      }
      //Check for valid DE using the "tryexec" line
        //this allows for special startup commands on the "exec" line
      if(QFile::exists(tmp[4])){
        //Add the DE to list of installed xsessions
	instXBinList << tmp[0];
	instXNameList << tmp[1];
	instXCommentList << tmp[2];
	instXDEList << tmp[5]; //Non-localized name of the DE
	//Check to make sure we have a valid icon
	if(!tmp[3].isEmpty() && !QFile::exists(tmp[3]) ){ tmp[3] = ""; }
	instXIconList << tmp[3];
	Backend::log( "PCDM: Found xsession: " + deFiles[i].section("/",-1)+": "+tmp.join(" - ") );
      }
    }
  }
  if(instXNameList.isEmpty()){
    //Create an entry so that we know this function has been run already
    instXNameList << "None";
    instXBinList << "none";
    instXCommentList << "No xSession Environments available in" << xDir;
    instXIconList << ":images/nodesktop.png";
  }
}

QStringList Backend::readXSessionsFile(QString filePath, QString locale){
//output: [Exec, Localized Name, Localized Comment, Icon, TryExec]
  QString name, lname, comm, lcomm, icon, exec, tryexec;
  QStringList output;
  QString lna = "Name["+locale+"]"; //variable to look at for localized name
  QString lco = "Comment["+locale+"]"; //variable to look at for localized comment
  QFile file(filePath);
  if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
    QTextStream in(&file);
    while (!in.atEnd()){
      QString line = in.readLine().simplified();
      QString var = line.section("=",0,0,QString::SectionSkipEmpty).simplified();
      QString val = line.section("=",1,50,QString::SectionSkipEmpty).simplified();
      if(var.toLower()=="exec"){ exec = val; }
      else if(var.toLower()=="tryexec"){ tryexec = val; }
      else if(var.toLower()=="icon"){ icon = val; }
      else if(var.toLower()=="name"){ name = val; }
      else if(var.toLower()=="comment"){ comm = val; }
      else if(var==lna){ lname = val; }
      else if(var==lco){ lcomm = val; }
      else{} //do nothing with other lines

    }
  }
  //Use the unlocalized name/comment if localized values not detected
  if(lname.isEmpty()){ lname = name; }
  if(lcomm.isEmpty()){ lcomm = comm; }
  //Make sure that we have a name/exec for the session, otherwise invalid file
  if(lname.isEmpty() || exec.isEmpty() ){ return output; }
  //If no tryexec given, check for the first binary given on the Exec line
  if(tryexec.isEmpty()){ tryexec = exec.section(" ",0,0,QString::SectionSkipEmpty).simplified(); }
  //Check that there is an icon given
  if(icon.isEmpty()){
    //Try to use a built in icon if a known DE
    if(name.toLower().contains("gnome")){icon = ":images/gnome.png"; }
    if(name.toLower().contains("kde")){icon = ":images/kde.png"; }
    if(name.toLower().contains("xfce")){icon = ":images/xfce.png"; }
    if(name.toLower().contains("lxde")){icon = ":images/lxde.png"; }
    if(name.toLower().contains("fluxbox")){icon = ":images/fluxbox.png"; }
    if(name.toLower().contains("openbox")){icon = ":images/openbox.png"; }
  }
  //Format the results into the output list
  output.clear();
  output << exec << lname << lcomm << icon << tryexec << name;
  return output;

}

void Backend::readSystemUsers(){
  //make sure the lists are empty
  usernameList.clear(); displaynameList.clear(); homedirList.clear();
  QStringList uList;	
    //Use "getent" to get all possible users
    QProcess p;
    p.setProcessChannelMode(QProcess::MergedChannels);
      QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
      //Make sure to set all the possible UTF-8 flags before reading users
      env.insert("LANG", "en_US.UTF-8");
      env.insert("LC_ALL", "en_US.UTF-8");
      env.insert("MM_CHARSET","UTF-8");
    p.setProcessEnvironment(env);
    p.start("getent passwd");
    while(p.state()==QProcess::Starting || p.state() == QProcess::Running){
      p.waitForFinished(200);
      QCoreApplication::processEvents();
    }
    uList = QString::fromUtf8( p.readAllStandardOutput() ).split("\n");
    
    //Remove all users that have:
   QStringList filter; filter << "server" << "daemon" << "database" << "system"<< "account"<<"pseudo";
   //List any shells which are still valid - if not installed fall back on csh
   QStringList validShells; validShells << "/usr/local/bin/zsh" << "/usr/local/bin/fish" << "/usr/local/bin/bash";
   for(int i=0; i<uList.length(); i++){
    bool bad = false;
    bool fixshell = false;
    QString dispcheck = uList[i].section(":",4,4).toLower();
    QString shell = uList[i].section(":",6,6);
    //First see if the listed shell is broken, but valid
    if(!QFile::exists(shell) && validShells.contains(shell)){ fixshell = true; }
    // Shell Checks
    if(shell.contains("nologin") || shell.isEmpty() ){bad=true;}
    else if( !QFile::exists(shell) && !fixshell ){ bad = true; }
    // User Home Dir
    else if(uList[i].section(":",5,5).contains("nonexistent") || uList[i].section(":",5,5).contains("/empty") || uList[i].section(":",5,5).isEmpty() ){bad=true;}
    // uid > 0
    else if(uList[i].section(":",2,2).toInt() < 1){bad=true;} //don't show the root user
    //Check that the name/description does not contain "server"
    else if(uList[i].section(":",2,2).toInt() < 1000){
	if(Over1K){ bad = true;} //ignore anything under UID 1000
	else{
	  //Apply the special <1000 filters
	  if(excludedUsers.contains(uList[i].section(":",0,0))){ bad = true; }
	  for(int f=0;f<filter.length() && !bad; f++){
	    if(dispcheck.contains(filter[f])){ bad = true; }
          }
        }
    }
    
    //See if it failed any checks
    if(bad){ uList.removeAt(i); i--; }
    else{
      //Add this user to the lists if it is good
      usernameList << uList[i].section(":",0,0).simplified();
      displaynameList << uList[i].section(":",4,4).simplified();
      homedirList << uList[i].section(":",5,5).simplified();
      if(fixshell){ usershellList << "/bin/csh"; }
      else{ usershellList << uList[i].section(":",6,6).simplified(); }
    }
   } //end loop over uList
  
}

void Backend::readSystemLastLogin(){
    if(!QFile::exists(DBDIR+"lastlogin")){
      lastUser.clear();
      Backend::log("PCDM: No previous login data found");
    }else{
      //Load the previous login data
      QFile file(DBDIR+"lastlogin");
      if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        Backend::log("PCDM: Unable to open previous login data file");    
      }else{
        QTextStream in(&file);
        lastUser= in.readLine();
	lastDE= in.readLine();
        file.close();
      }
    }  
}

void Backend::writeSystemLastLogin(QString user, QString desktop){
  QFile file1(DBDIR+"lastlogin");
  if(!file1.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text)){
    Backend::log("PCDM: Unable to save last login data to system directory");	  
  }else{
    QTextStream out(&file1);
    out << user << "\n" << desktop;
    file1.close();
  }

}

QString Backend::readUserLastDesktop(QString user){
  QString desktop;
  QString LLpath = Backend::getUserHomeDir(user) + "/.lastlogin";
  if(!QFile::exists(LLpath)){
    Backend::log("PCDM: No previous user login data found for user: "+user);
  }else{
    //Load the previous login data
    QFile file(LLpath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
      Backend::log("PCDM: Unable to open previous user login file: "+user);    
    }else{
      QTextStream in(&file);
      desktop = in.readLine();
      file.close();
    }
  }
  return desktop;
}

void Backend::writeUserLastDesktop(QString user, QString desktop){
  QFile file2( Backend::getUserHomeDir(user) + "/.lastlogin" );
  if(!file2.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text)){
    Backend::log("PCDM: Unable to save last login data for user:"+user);	  
  }else{
    QTextStream out(&file2);
    out << desktop;
    file2.close();
  }
}


