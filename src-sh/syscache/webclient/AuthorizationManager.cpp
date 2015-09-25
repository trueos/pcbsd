// ===============================
//  PC-BSD REST/JSON API Server
// Available under the 3-clause BSD License
// Written by: Ken Moore <ken@pcbsd.org> July 2015
// =================================
#include "AuthorizationManager.h"
#include <QDebug>
//#include <QProcess>

// Stuff for PAM to work
#include <sys/types.h>
#include <security/pam_appl.h>
#include <security/openpam.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pwd.h>
#include <login_cap.h>

//Internal defines
#define TIMEOUTSECS 300 // (5 minutes) time before a token becomes invalid
#define AUTHCHARS QString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789")
#define TOKENLENGTH 20

AuthorizationManager::AuthorizationManager(){
  HASH.clear();
  //initialize the random number generator (need to generate auth tokens)
  qsrand(QDateTime::currentMSecsSinceEpoch());
}
AuthorizationManager::~AuthorizationManager(){
	
}

// == Token Interaction functions ==
void AuthorizationManager::clearAuth(QString token){
  //clear an authorization token
  if(HASH.contains(token)){ HASH.remove(token); }
}

bool AuthorizationManager::checkAuth(QString token){
	//see if the given token is valid
  bool ok = false;
  if(HASH.contains(token)){
    //Also verify that the token has not timed out
    ok = (HASH[token] > QDateTime::currentDateTime());
    if(ok){ HASH.insert(token, QDateTime::currentDateTime().addSecs(TIMEOUTSECS)); } //valid - bump the timestamp
  }
  return ok;
}

int AuthorizationManager::checkAuthTimeoutSecs(QString token){
	//Return the number of seconds that a token is valid for
  if(!HASH.contains(token)){ return 0; } //invalid token
  return QDateTime::currentDateTime().secsTo( HASH[token] );
}


// == Token Generation functions
QString AuthorizationManager::LoginUP(bool localhost, QString user, QString pass){
	//Login w/ username & password
  bool ok = false;
  if(localhost){
    //only need to check that the username is valid
    ok = system( QString("id \""+user+"\" > /dev/null").toLocal8Bit() )==0;
  }else{
    //Need to run the full username/password through PAM
    ok = pam_checkPW(user,pass);
  }
  qDebug() << "User Login Attempt:" << user << " Success:" << ok;
  if(!ok){ return ""; } //invalid login
  else{ return generateNewToken(); } //valid login - generate a new token for it
}

QString AuthorizationManager::LoginService(bool localhost, QString service){
  //Login a particular automated service
  qDebug() << "Service Login Attempt:" << service << " Success:" << localhost;
  if(!localhost){ return ""; } //invalid - services must be local for access
  //Check that the service is valid on the system
  // -- TO-DO
  
  //Now generate a new token and send it back
  return generateNewToken();
}

// =========================
//               PRIVATE
// =========================
QString AuthorizationManager::generateNewToken(){
  QString tok;
  for(int i=0; i<TOKENLENGTH; i++){
    tok.append( AUTHCHARS.at( qrand() % AUTHCHARS.length() ) );
  }
  if(HASH.contains(tok)){ 
    //Just in case the randomizer came up with something identical - re-run it
    tok = generateNewToken();
  }else{ 
    //unique token created - add it to the hash with the current time (+timeout)
    HASH.insert(tok, QDateTime::currentDateTime().addSecs(TIMEOUTSECS) );
  }
  return tok;
}

/*
 ========== PAM FUNCTIONS ==========
*/
static struct pam_conv pamc = { openpam_nullconv, NULL };
pam_handle_t *pamh;

bool AuthorizationManager::pam_checkPW(QString user, QString pass){
  //Convert the inputs to C character arrays for use in PAM
  QByteArray tmp = user.toUtf8();
  char* cUser = tmp.data();
  QByteArray tmp2 = pass.toUtf8();
  char* cPassword = tmp2.data();
  //initialize variables
  bool result = false;
  int ret;
  //Initialize PAM
  ret = pam_start("login", cUser, &pamc, &pamh);
  if( ret == PAM_SUCCESS ){
    //Place the user-supplied password into the structure 
    ret = pam_set_item(pamh, PAM_AUTHTOK, cPassword);
    //Set the TTY 
    //ret = pam_set_item(pamh, PAM_TTY, "pcdm-terminal");
    //Authenticate with PAM
    ret = pam_authenticate(pamh,0);
    if( ret == PAM_SUCCESS ){
      //Check for valid, unexpired account and verify access restrictions
      ret = pam_acct_mgmt(pamh,0);
      if( ret == PAM_SUCCESS ){ result = true; }
    
    }else{
      pam_logFailure(ret);
    }
  }
  //return verification result
  return result;	
}

void AuthorizationManager::pam_logFailure(int ret){
  //Interpret a PAM error message and log it
  qWarning() << "PAM Error: " << ret;
  switch( ret ){
  case PAM_ABORT:
    qWarning() << " - PAM abort error";
    break;
  case PAM_AUTHINFO_UNAVAIL:
    qWarning() << " - Authentication info unavailable";
    break;
  case PAM_AUTH_ERR:
    qWarning() << " - Authentication error";
    break;
  case PAM_BUF_ERR:
    qWarning() << " - Buffer error";
    break;
  case PAM_CONV_ERR:
    qWarning() << " - Conversion error";
    break;
  case PAM_CRED_INSUFFICIENT:
    qWarning() << " - Credentials insufficient";
    break;
  case PAM_MAXTRIES:
    qWarning() << " - Maximum number of tries exceeded";
    break;
  case PAM_PERM_DENIED:
    qWarning() << " - Permission denied";
    break;
  case PAM_SERVICE_ERR:
    qWarning() << " - Service error";
    break;
  case PAM_SYMBOL_ERR:
    qWarning() << " - Symbol error";
    break;
  case PAM_SYSTEM_ERR:
    qWarning() << " - System error";
    break;
  case PAM_USER_UNKNOWN:
    qWarning() << " - Unknown user";
    break;
  default:
    qWarning() << " - Unrecognized authentication error";
  }
	
}
