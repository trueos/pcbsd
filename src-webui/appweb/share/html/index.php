<?

  // Few defaults
  $remoteAccess = false;
  if ( file_exists("/usr/local/etc/appcafe.conf") )
  {
     $ini = parse_ini_file("/usr/local/etc/appcafe.conf");
     if ( $ini['remote'] == true )
        $remoteAccess = true;
  }

  // Check if we are talking to remote
  $CLIENTIP = $_SERVER['REMOTE_ADDR'];
  if ( $CLIENTIP != "127.0.0.1" and $CLIENTIP != "::1" and ! $remoteAccess )
     die("No remote access enabled!");

  // Start PHP session
  session_start(); 

  require("include/Mobile_Detect.php");

  define('DS',  TRUE); // used to protect includes
  define('USERNAME', $_SESSION['username']);
  define('SELF',  $_SERVER['PHP_SELF'] );

  if ( (!USERNAME or isset($_GET['logout'])) ) {
    // Bypass if called from localhost
    if ( $CLIENTIP != "127.0.0.1" and $CLIENTIP != "::1" ) {
      include('include/login.php');
      exit(0);
    }
  }

  require("include/globals.php");
  require("include/functions.php");


  // Placeholder for detection to determine type of system we are
  // $sysType can be set to {DESKTOP|SERVER|APPLIANCE}
  //
  // Desktop will display all packages for the local system
  // Server will display everything EXCEPT graphical (xorg) packages for local system
  // Appliance will not allow modification of the local system, only jails
  if ( file_exists("/usr/local/bin/startx") )
     $sysType="DESKTOP"; 
  else
     $sysType="SERVER"; 


  // Figure out what page is being requested
  $jail = "";
  if ( ! empty($_GET['jail'])) {
     if ( $_GET['jail'] == "__system__") {
        $jail = "#system";
        $jailUrl = "__system__";
     } else {
        $jail = $_GET['jail'];
        $jailUrl = $_GET['jail'];
     }

  }

  // Do any install / delete requests
  if ( ! empty($_GET["deleteApp"]) )
     queueDeleteApp();
  if ( ! empty($_GET["installApp"]) )
     queueInstallApp();

  // Figure out what page is being requested
  if ( empty($_GET["p"]))
     $page = "appcafe";
  else
     $page = $_GET["p"];

  // Set some globals for mobile detection
  $detect = new Mobile_Detect;
  $deviceType = ($detect->isMobile() ? ($detect->isTablet() ? 'tablet' : 'phone') : 'computer');
  $scriptVersion = $detect->getScriptVersion();

  require("include/header.php");

  if ( $deviceType == "computer" )
    require("include/nav-computer.php");
  else
    require("include/nav-phone.php");

  // Does the page exist?
  if ( file_exists("pages/$page.php") === false ) {
    require("pages/error.php");
  } else {
    require("pages/$page.php");
  }


  require("include/footer.php");
?>
