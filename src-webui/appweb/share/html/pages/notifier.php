<?php
  // Get the client IP address
  $CLIENTIP = $_SERVER['REMOTE_ADDR'];
  session_start();
  $DISPATCHID = $_SESSION['dispatchid'];

  if ( (!USERNAME or isset($_GET['logout'])) ) {
    // Bypass if called from localhost
    if ( $CLIENTIP != "127.0.0.1" and $CLIENTIP != "::1" ) {
      include('include/login.php');
      exit(0);
    }
  }

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

  session_start();

  define('DS',  TRUE); // used to protect includes
  define('USERNAME', $_SESSION['username']);
  define('SELF',  $_SERVER['PHP_SELF'] );

  // Get the page this was requested from
  $page = $_GET['p'];

  require("../include/globals.php");
  require("../include/functions.php");
  $bgcolor="white";
  $status="UP2DATE";

  // Command to prod dispatcher for current status
  // Eventually we will pep this up with extra parsing about
  // the status
  $narray = run_cmd("status");
  if ( $narray[0] == "Idle" or empty($narray) ) {
    $rarray = run_cmd("results");
    $lastElement=count($rarray);
    $lastElement--;
    if ( ! empty($rarray[$lastElement]) ) {
      $results = explode(" ", $rarray[$lastElement]);
      if ( $results[2] == "pkgupdate" ) {
        $target=$results[3];
	if ( $results[3] == "__system__" )
            $target="Local System";
      } elseif ( $results[2] == "iocage" ) {
      } elseif ( $results[4] == "install" ) {
      } else {
      }

      if ( $results[0] == "SUCCESS" )
	$status = "SUCCESS";
      else
	$status = "FAILED";
    }
    $output="$result";
  } else {
    $status = "WORKING";
  }

  $pkgUpdates=false;
  // Check if we can prod local system for updates
  $larray = array();
  if ( $sysType != "APPLIANCE" )
     $larray[] = "#system";

  // Build list of jails
  $jailoutput = get_jail_list();
  $running=$jailoutput[0];
  $rarray = explode( ", ", $running);

  // Combine the arrays
  $carray = array_merge($larray, $rarray);

  foreach ($carray as $jname) {
    if ( empty($jname) )
       continue;

    unset($jarray);
    exec("$sc ". escapeshellarg("pkg ". $jname . " hasupdates"), $jarray);
    $hasupdates=$jarray[0];
 
    if ( $hasupdates == "true" ) {
       $pkgUpdates=true;
       break;
    }
  }

  // We have updates! Show the notification icon
  if ( $pkgUpdates and $status != "WORKING" )
    $status = "UPDATES";

  $output = "<img align=absmiddle height=32 width=32 src=\"../images/dialog-ok.png\">";
  if ( $status == "WORKING" )
    $output = "<img align=absmiddle height=32 width=32 src=\"../images/working.gif\" title=\"Working...\"> Working";
  elseif ( $status == "UPDATES" )
     $output="<img src=\"/images/warning.png\" align=\"absmiddle\" height=32 width=32 title=\"Updates are available!\"> Update";
  elseif ( $status == "SUCCESS" )
     $output = "<img align=absmiddle height=32 width=32 src=\"../images/dialog-ok.png\"> Status";
  elseif ( $status == "FAILED" ) 
     $output = "<img align=absmiddle height=32 width=32 src=\"../images/application-exit.png\"> Failure";
  
  if ( $page == "plugins" or $page == "plugininfo" or $page == "sysplugins" )
    echo "<a href=\"?p=dispatcher-plugins\">$output</a>";
  else
    echo "<a href=\"?p=dispatcher\">$output</a>";

?>
