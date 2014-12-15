#include <QCoreApplication>
#include <QDebug>
#include <unistd.h>
#include <sys/types.h>

#include "syscache-client.h"
//#include "../config.h"

#ifndef PREFIX
#define PREFIX QString("/usr/local/")
#endif

void printJailUsage(){
  qDebug() << "syscache Jail database access:\n\
-------------------------------------------------------------------------------\n\
\"jail list\": List all running jails by name\n\
\"jail stoppedlist\": List all stopped jails by name\n\
\"jail <jailname> <info>\": Get information about a particular jail\n\
  Possible Info requests:\n\
  \"id\": 	[RUNNING ONLY] Get the current jail ID # (JID)\n\
  \"ip\": 	[RUNNING ONLY] Get the current jail IP address\n\
  \"path\":	[RUNNING ONLY] Get the jail directory path on host system\n\
  \"WID\":	Get the Warden jail ID #\n\
  \"ipv4\":	Get the jail ipv4 address setting\n\
  \"alias-ipv4\":	Get the jail ipv4 alias setting\n\
  \"bridge-ipv4\":	Get the jail ipv4 bridge setting\n\
  \"alias-bridge-ipv4\":	Get the jail ipv4 bridge setting for the alias\n\
  \"defaultrouter-ipv4\":	The ipv4 address setting for the default gateway\n\
  \"ipv6\":	Get the jail ipv6 address setting\n\
  \"alias-ipv6\":	Get the jail ipv6 alias setting\n\
  \"bridge-ipv6\":	Get the jail ipv6 bridge setting\n\
  \"alias-bridge-ipv6\":	Get the jail ipv6 bridge setting for the alias\n\
  \"defaultrouter-ipv6\":	The ipv6 address setting for the default gateway\n\
  \"autostart\":	[true/false] Jail is set to start automatically on boot\n\
  \"vnet\":	[Disabled/Enabled]\n\
  \"type\":	Get the type of jail [portjail, traditional, linux]\n\
\n\
\n\
Example:\n\
\"jail myjail ip\": Returns the current IP address for the running jail \"myjail\" \n\
";
  exit(1);
}

void printPkgUsage(){
  qDebug() << "syscache PKG database access:\n\
-------------------------------------------------------------------------------\n\
NOTE: <jail> = \"#system\" or name of a running jail\n\
-------------------------------------------------------------------------------\n\
\"pkg <jail> installedlist\": List all installed packages by origin\n\
\"pkg <jail> hasupdates\": (true/false) Package updates are available\n\
\"pkg <jail> updatemessage\": Full log message from the check for updates\n\
\"pkg <jail> <local or remote> <pkg origin> <info>\":\n\
  Possible Info requests:\n\
  \"origin\": 	Package/port origin\n\
  \"name\": 	Package name\n\
  \"version\":	Package version\n\
  \"maintainer\":	Maintainer email address\n\
  \"comment\":	Short package summary\n\
  \"description\":	Full package description\n\
  \"website\":	Application website URL\n\
  \"size\":	Human-readable package size (installed or to download)\n\
  \"arch\":	System architecture the package was built for\n\
  \"timestamp\":	[local only] Date/Time the package was installed (epoch time?)\n\
  \"message\":	Special message included with the package\n\
  \"isOrphan\":	[local only] Is the package orphaned? (true/false)\n\
  \"isLocked\":	[local only] Is the package version locked? (true/false)\n\
  \"dependencies\":	List of dependencies by pkg origin\n\
  \"rdependencies\":	List of reverse dependencies by pkg origin\n\
  \"categories\":	List of categories where this package belongs\n\
  \"files\":	[local only] List of files installed by this package\n\
  \"options\":	List of compile-time options used to build the package\n\
  \"license\":	List of licences the application is available under\n\
  \"users\":	[local-only] List of users that were created for this package\n\
  \"groups\":	[local-only] List of groups that were created for this package\n\
\n\
\"pkg search <search term> [<jail>] [minimum results]\":\n\
  NOTE: If no <jail> is specified, it will assume \"#system\"\n\
For additional information about the search algorithm, run \"syscache help search\"\n\
\n\
Example:\n\
\"pkg #system local games/angband version\": Returns the system-installed version of the game \"angband\"\n\
";
  exit(1);
}

void printPbiUsage(){
  qDebug() << "syscache PBI database access:\n\
-------------------------------------------------------------------------------\n\
\"pbi list <infolist>\":\n\
  Possible Info Lists:\n\
  \"allapps\":	List all applications by pkg origin\n\
  \"serverapps\":	List all server applications by pkg origin\n\
  \"textapps\":	List all text applications by pkg origin\n\
  \"graphicalapps\":	List all graphical applications by pkg origin\n\
  \"allcats\":	List all categories\n\
  \"servercats\":	List all categories that contain a server application\n\
  \"textcats\":	List all categories that contain a text application\n\
  \"graphicalcats\":	List all categories that contain a graphical application\n\
\n\
\"pbi app <pkg origin> <info>\":\n\
  Possible Application Information:\n\
  \"author\": 	Package author\n\
  \"category\":	Primary category where this package belongs\n\
  \"confdir\":	Local path to configuration directory\n\
  \"dependencies\":	List of *additional* dependencies by pkg origin\n\
  \"origin\": 	Package/port origin\n\
  \"plugins\":	List of optional plugins by pkg origin\n\
  \"rating\":	Current rating (0.00 to 5.00)\n\
  \"relatedapps\":	List of similar applications by pkg origin\n\
  \"screenshots\":	List of screenshot URLs\n\
  \"type\":	Primary category where this package belongs\n\
  \"tags\":	List of search tags for application\n\
  \"comment\":	(pkg override) Short package summary\n\
  \"description\":	(pkg override) Full package description\n\
  \"license\":	(pkg override) List of licences for the application\n\
  \"maintainer\":	(pkg override) Maintainer email address\n\
  \"name\": 	(pkg override) Package name\n\
  \"options\":	(pkg override) List of compile-time options used\n\
  \"website\":	(pkg override) Application website URL\n\
\n\
\"pbi cat <pkg category> <info>\":\n\
  Possible Category Information:\n\
  \"comment\": 	Short description of the category contents\n\
  \"icon\":	Icon file path for the category\n\
  \"name\":	Display name to use for the category (Ex: Desktop Utilities)\n\
  \"origin\":	pkg name of the category (Ex: deskutils)\n\
\n\
\"pbi search <search term> [filter] [minimum results]\":\n\
  Possible Filters:\n\
  \"all\" (default): No filtering\n\
  \"graphical\": Only search for graphical applications\n\
  \"server\": Only search for server applications\n\
  \"text\": Only search for text applications\n\
  \"notgraphical\": Do not search graphical applications\n\
  \"notserver\": Do not search server applications\n\
  \"nottext\": Do not search text applications\n\
For additional information about the search algorithm, run \"syscache help search\"\n\
\n\
Example:\n\
\"pbi app games/angband author\": Returns the author of the game \"angband\"\n\
";
  exit(1);
}

void printSearchUsage(){
  qDebug() << "syscache Search algorithm information:\n\
-------------------------------------------------------------------------------\n\
\"<pkg | pbi> search <search term> [<pkg jail>/<pbi filter>] [result minimum]\"\n\
  This allows the user to retrieve a list of pkg origins corresponding to the given search term.\n\
\n\
Default Values for optional inputs:\n\
  <pkg jail> -> \"#system\"\n\
  <pbi filter> -> \"all\"\n\
  <result minimum> -> 10\n\
\n\
Notes:\n\
  1) Each search is performed case-insensitive, with the next highest search priority group added to the end of the list as long as the number of matches is less than the requested minimum.\n\
  2) Each search priority/group is arranged alphabetically by name independently of the other groups.\n\
  3) Each package origin will always appear in the highest priority group possible with no duplicates later in the output.\n\
\n\
Search matching groups/priority is:\n\
    1) Exact Name match\n\
    2) Partial Name match (name begins with search term)\n\
    3) Partial Name match (search term anywhere in name)\n\
    4) Comment match (search term in comment for pkg)\n\
    5) Description match (search term in description for pkg)\n\
\n\
Initial Filtering:\n\
  For packages, it always searches the entire list of available/remote packages for that particular jail\n\
  For PBI's the possible filters are:\n\
  \"all\"\n\
  \"graphical\"\n\
  \"server\"\n\
  \"text\"\n\
  \"notgraphical\" (I.E. Show both server and text apps)\n\
  \"notserver\" (I.E. Show both graphical and text apps)\n\
  \"nottext\" (I.E. Show both graphical and server apps)\n\
";
  exit(1);	
}
void printHelp(){
  qDebug() << "\
syscache: Interface to retrieve system information from the syscache daemon\n\
Note:  Syscache uses the \"<br>\" string for line breaks in all information output.\n\
\n\
Usage:\n\
  syscache startsync 	-> Manually start a system information sync (usually unnecessary) \n\
  syscache needsreboot 	-> [true/false] See whether the system needs to reboot to finish updates \n\
  syscache isupdating	-> [true/false] See whether the system is currently performing updates \n\
  syscache hasupdates 	-> [true/false] See whether any system updates are available \n\
  syscache updatelog	-> Raw text output from the check for system updates \n\
  syscache hasmajorupdates 	-> [true/false] See whether major FreeBSD system updates are available \n\
  syscache majorupdatelog	-> Details about the major update(s)\n\
  syscache hassecurityupdates 	-> [true/false] See whether FreeBSD security updates are available \n\
  syscache securityupdatelog	-> Details about any security update(s)\n\
  syscache haspcbsdupdates 	-> [true/false] See whether any special PC-BSD hotfixes are available \n\
  syscache pcbsdupdatelog	-> Details about any PC-BSD hotfixes\n\
\n\
  syscache \"<DB request 1>\" \"<DB request 2>\" [etc] \n\
  syscache help [jail | pkg | pbi | search] -> Information about DB requests for that subsystem \n\
";
  exit(1);
}

int main( int argc, char ** argv )
{
    //Turn the inputs into a list
    QStringList inputs;
    for(int i=1; i<argc; i++){
      inputs << QString( argv[i] );
    }
    //Check for help inputs and print usage info
    if(inputs.contains("help") && inputs.contains("pkg") ){ printPkgUsage(); }
    else if(inputs.contains("help") && inputs.contains("jail") ){ printJailUsage(); }
    else if(inputs.contains("help") && inputs.contains("pbi") ){ printPbiUsage(); }
    else if(inputs.contains("help") && inputs.contains("search") ){ printSearchUsage(); }
    else if(inputs.length()<1 || inputs.contains("help")){ printHelp(); }
    //Check whether running as root (if shutting down the daemon)
    if( getuid() != 0 && inputs.join("").simplified().contains("shutdowndaemon") ){
      qDebug() << "[ERROR] The syscache daemon may only be stopped by root!";
      return 1;
    }
    
    QCoreApplication a(argc, argv);
    //Create the client and send requests
    SysCacheClient *w = new SysCacheClient(&a);
    w->parseInputs(inputs);
    return a.exec();
}
