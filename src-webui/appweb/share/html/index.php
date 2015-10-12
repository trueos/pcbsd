<?php
  // Few defaults
  $remoteAccess = false;
  if ( file_exists("/usr/local/etc/appcafe.conf") )
  {
     $ini = parse_ini_file("/usr/local/etc/appcafe.conf");
     if ( $ini['remote'] == true )
        $remoteAccess = true;
  }

  // Check the mode to run in
  if ( ! empty($ini['mode'] ) )
  {
     if ( $ini['mode'] == "desktop" )
        $sysType="DESKTOP";
     if ( $ini['mode'] == "server" )
        $sysType="SERVER";
     if ( $ini['mode'] == "appliance" )
        $sysType="APPLIANCE";
  } else {
    // No config setting, let's pick one
    if ( file_exists("/usr/local/bin/startx") )
      $sysType="DESKTOP";
    else
      $sysType="SERVER";
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
  $DISPATCHID = $_SESSION['dispatchid'];

  // Check if calling from a remote host
  if ( (!USERNAME or isset($_GET['logout'])) ) {
    $timeout = false;
    include('include/login.php');
    exit(0);
  }

  // See if the timeout has been met (60 minutes)
  if ( $_SESSION['timeout'] + 60 * 60 < time() ) {
    $timeout = true;
    include('include/login.php');
    exit(0);
  } else {
    $_SESSION['timeout'] = time();
  }

  $login_on_fail = true;
  // Load our websocket library
  require('vendor/autoload.php');
  require("include/globals.php");
  require("include/functions.php");
  require("include/functions-config.php");

  // Auth this WS connection
  $sccmd = array("username" => "root");
  $response = send_sc_query($sccmd, "auth");


  // Check if we have updates to display
  check_update_reboot();

  // Do any install / delete requests
  if ( ! empty($_GET["deleteApp"]) )
     queueDeleteApp();
  if ( ! empty($_GET["installApp"]) )
     queueInstallApp();
  if ( ! empty($_GET["deletePlugin"]) )
     queueDeletePlugin();
  if ( ! empty($_GET["installPlugin"]) )
     queueInstallPlugin();

  // Figure out what page is being requested
  if ( empty($_GET["p"])) {
     $page = "appcafe";
  } else {
     $page = $_GET["p"];
  }

  // If we are running in appliance mode flip to the plugins page
  if ( $sysType == "APPLIANCE" and empty($page))
    $page = "plugins";
  if ( $sysType == "APPLIANCE" and $page == "appcafe" )
    $page = "plugins";
 
  // Don't echo headers / nav info if we are saving PBI list
  if ( "$page" == "exportpbis" ) {
    require("pages/exportpbis.php");
    exit(0);
  }

  // Check if we are on the dispatcher plugin page
  $pluginDispatcher = false;
  if ( $page == "dispatcher-plugins" ) {
    $page = "dispatcher";
    $pluginDispatcher = true;
  }
  $jailDispatcher = false;
  if ( $page == "dispatcher-jails" ) {
    $page = "dispatcher";
    $jailDispatcher = true;
  }

  // If we are on plugins section, make sure we have a start-end range
  if ( stripos($page, "plugin") !== false ) {
    // Check for VIMAGE support
    $vimage = exec("/sbin/sysctl -qn kern.features.vimage");
    if ( $vimage == 1 )
    {
      // Get the iocage pool
      $curpool = get_iocage_pool();
      if ( empty($curpool) )
      {
        $firstrun=true;
        $page="pluginconfig";
      }
    } else {
      $dccmd = array("iocage get ip4_autostart default", "iocage get ip4_autoend default");
      $output = send_dc_cmd($dccmd);
      $ip4start = $output["iocage get ip4_autostart default"][0];
      $ip4end = $output["iocage get ip4_autoend default"][0];
      if ( ( empty($ip4start) or empty($ip4end) ) or ( $ip4start == "none" or $ip4end == "none" ) )
      {
        $firstrun=true;
        $page="pluginconfig";
      }
    }
  }

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
