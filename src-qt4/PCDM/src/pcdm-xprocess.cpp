/* PCDM Login Manager:
*  Written by Ken Moore (ken@pcbsd.org) 2012/2013
*  Copyright(c) 2013 by the PC-BSD Project
*  Available under the 3-clause BSD license
*/

/*
Sub-classed QProcess for starting an XSession Process
*/

#include "pcdm-xprocess.h"

XProcess::XProcess() : QProcess(0) {
  //initialize the variables
  xuser.clear();
  xcmd.clear();
  xhome.clear();
  xpwd.clear();
  pam_started = FALSE;
  pam_session_open = FALSE;
  //Setup the finished signal/slot
  connect( this, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotCleanup(int, QProcess::ExitStatus)) );
}

XProcess::~XProcess(){
  if( isRunning() ){
    this->terminate();
  }
  this->close();
}

void XProcess::loginToXSession(QString username, QString password, QString homedir, QString cmd){
  //Setup the variables
  xuser = username;
  xpwd = password;
  xhome = homedir;
  xcmd = cmd;
  //Now start the login process
  startXSession();
}

bool XProcess::isRunning(){
  if(this->state() != QProcess::NotRunning){ return TRUE; }
  else{ return FALSE; }
}

void XProcess::waitForSessionClosed(){
  // CAUTION!! 
  // This function will pause the calling program to wait for the session to end!
  if( isRunning() ){
    this->waitForFinished(-1);
  }
}

/*
 ========== SESSION STARTUP ==========
*/

bool XProcess::startXSession(){
  //Check that the necessary info to start the session is available
  if( xuser.isEmpty() || xcmd.isEmpty() || xhome.isEmpty() ){
    emit InvalidLogin();  //Make sure the GUI knows that it was a failure
    return FALSE;
  }
  //Backend::log("Starting up Desktop environment ("+xcmd+") as user ("+xuser+")");
  
  //Check for PAM username/password validity
  if( !pam_checkPW() ){ emit InvalidLogin(); pam_shutdown(); return FALSE; }
  //Startup the PAM session
  if( !pam_startSession() ){ pam_shutdown(); return FALSE; }
  pam_session_open = TRUE; //flag that pam has an open session
  
  // Configure the DE startup command
  QString cmd = "su "+xuser+" -c \""; //switch user command to start process properly
  //  - Setup to run the user's <home-dir>/.xprofile startup script
  if(QFile::exists(xhome+"/.xprofile")){
    cmd.append("(/bin/sh "+xhome+"/.xprofile) &; ");  //make sure to start it in parallel
  }
  //  - Add the DE startup command to the end
  cmd.append(xcmd);
  //  - Finish up the command formatting
  cmd.append("\"");
  
  //Backend::log("Startup command: "+cmd);
  // Get the current locale code
  QLocale mylocale;
  QString langCode = mylocale.name();
  // Setup the process environment
  QProcessEnvironment environ = QProcessEnvironment::systemEnvironment(); //current environment

  // Setup any specialized environment variables
  // USER, HOME, and SHELL are set by the "su" login
  environ.insert("LOGNAME",xuser); //Login name
  environ.insert("USERNAME",xuser); // Username
  environ.insert("PATH",environ.value("PATH")+":"+xhome+"/bin"); // Append the user's home dir to the path
  if( langCode.toLower() == "c" ){} // do nothing extra to it
  else if(!environ.value("MM_CHARSET").isEmpty() ){ langCode.append( "."+environ.value("MM_CHARSET") ); }
  else{ langCode.append(".UTF-8"); }
  environ.insert("LANG",langCode); //Set the proper localized language
  environ.insert("MAIL","/var/mail/"+xuser); //Set the mail variable
  environ.insert("GROUP",xuser); //Set the proper group id
  environ.insert("SHLVL","1"); //Set the proper shell level
  this->setProcessEnvironment(environ);
  this->setWorkingDirectory(xhome); //set the current directory to the user's home directory
  //Log the DE startup outputs as well
  this->setStandardOutputFile(xhome+"/.pcdm-startup.log",QIODevice::Truncate);
  // Startup the process
  this->start(cmd);
  return TRUE;
}

void XProcess::slotCleanup(int exitCode, QProcess::ExitStatus status){
  pam_shutdown(); //make sure that PAM shuts down properly	
}

/*
 ========== PAM FUNCTIONS ==========
*/
static struct pam_conv pamc = { openpam_nullconv, NULL };

bool XProcess::pam_checkPW(){
 //Requires internal "xuser" and "xpwd" variables to be set
	
//Convert the inputs to C character arrays for use in PAM
  QByteArray tmp = xuser.toUtf8();
  char* cUser = tmp.data();
  QByteArray tmp2 = xpwd.toUtf8();
  char* cPassword = tmp2.data();
  //initialize variables
  bool result = FALSE;
  int ret;
  //Initialize PAM
  ret = pam_start("login", cUser, &pamc, &pamh);
  if( ret == PAM_SUCCESS ){
    pam_started = TRUE; //flag that pam is started
    //Place the user-supplied password into the structure 
    ret = pam_set_item(pamh, PAM_AUTHTOK, cPassword);
    //Set the TTY 
    //ret = pam_set_item(pamh, PAM_TTY, "pcdm-terminal");
    //Authenticate with PAM
    ret = pam_authenticate(pamh,0);
    if( ret == PAM_SUCCESS ){
      //Check for valid, unexpired account and verify access restrictions
      ret = pam_acct_mgmt(pamh,0);
      if( ret == PAM_SUCCESS ){ result = TRUE; }
    
    }else{
      pam_logFailure(ret);
    }
  }
  //return verification result
  return result;	
}

bool XProcess::pam_startSession(){
  //This should only be run if pam_checkPW was successful
  int ret = pam_open_session(pamh,0);
  bool ok = FALSE;
  if(ret == PAM_SUCCESS){ ok = TRUE; }
  else{ pam_logFailure(ret); }
  
  return ok;
}

bool XProcess::pam_stopSession(){
  //This should only be run if pam_startSession was successful
  int ret = pam_close_session(pamh,0);
  bool ok = FALSE;
  if(ret == PAM_SUCCESS){ ok = TRUE; }
  else{ pam_logFailure(ret); }
  
  return ok;
}

void XProcess::pam_logFailure(int ret){
  //Interpret a PAM error message and log it
  Backend::log("PAM Error: " + QString::number(ret));
  switch( ret ){
  case PAM_ABORT:
    Backend::log(" - PAM abort error");
    break;
  case PAM_AUTHINFO_UNAVAIL:
    Backend::log(" - Authentication info unavailable");
    break;
  case PAM_AUTH_ERR:
    Backend::log(" - Authentication error");
    break;
  case PAM_BUF_ERR:
    Backend::log(" - Buffer error");
    break;
  case PAM_CONV_ERR:
    Backend::log(" - Conversion error");
    break;
  case PAM_CRED_INSUFFICIENT:
    Backend::log(" - Credentials insufficient");
    break;
  case PAM_MAXTRIES:
    Backend::log(" - Maximum number of tries exceeded");
    break;
  case PAM_PERM_DENIED:
    Backend::log(" - Permission denied");
    break;
  case PAM_SERVICE_ERR:
    Backend::log(" - Service error");
    break;
  case PAM_SYMBOL_ERR:
    Backend::log(" - Symbol error");
    break;
  case PAM_SYSTEM_ERR:
    Backend::log(" - System error");
    break;
  case PAM_USER_UNKNOWN:
    Backend::log(" - Unknown user");
    break;
  default:
    Backend::log(" - Unrecognized authentication error");
  }
	
}

void XProcess::pam_shutdown(){
  if(pam_session_open){
    pam_stopSession();
    pam_session_open = FALSE;
  }
  if(pam_started){
    pam_end(pamh,0);
    pam_started = FALSE;
  }
}
