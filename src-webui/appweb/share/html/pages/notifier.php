<?php
  // Get the client IP address
  $CLIENTIP = $_SERVER['REMOTE_ADDR'];

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


  require("../include/globals.php");
  require("../include/functions.php");
  $bgcolor="white";

  // Command to prod dispatcher for current status
  // Eventually we will pep this up with extra parsing about
  // the status
  $narray = run_cmd("status");
  if ( $narray[0] == "Idle" or empty($narray[0]) ) {
    $rarray = run_cmd("results");
    $lastElement=count($rarray);
    $lastElement--;
    if ( ! empty($rarray[$lastElement]) ) {
      $results = explode(" ", $rarray[$lastElement]);
      if ( $results[2] == "pkgupdate" ) {
        $target=$results[3];
	if ( $results[3] == "__system__" )
            $target="Local System";
        $result = "Updated packages on: ". $target;
      } elseif ( $results[2] == "warden" ) {
        if ( $results[3] == "create" ) {
          $result = "Create jail: ". $results[4];
        } else {
          $result = "Delete jail: ". $results[4];
        }
      } elseif ( $results[4] == "install" ) {
        $result = "Installed ". $results[3];
      } else {
        $result = "Removed ". $results[3];
      }

      if ( $results[0] == "SUCCESS" )
	$result = "<img align=absmiddle height=35 width=35 src=\"../images/dialog-ok.png\">".$result;
      else
	$result = "<img align=absmiddle height=35 width=35 src=\"../images/application-exit.png\">".$result;
    }
    $output="$result";
  } else {
    $carray = explode(" ", $narray[0]);
    // Doing pkg / pbi ops
    if ( $carray[0] == "pkg" or $carray[0] == "pbi" ) {
      if ( $carray[2] == "install" )
         $result = "Installing $carray[1] to";
      else
         $result = "Removing $carray[1] from";

      $target=$carray[3];
      if ( $carray[3] == "__system__" )
         $target = "Local system";

      $output = $result . " $target";
    }
    if ( $carray[0] == "pkgupdate" ) {
      $target=$carray[2];
      if ( $carray[2] == "__system__" )
         $target = "Local system";
      $output = "Updating $target";
    }
    if ( $carray[0] == "warden" ) {
       if ( $carray[1] == "create" )
          $output = "Creating jail: ". $carray[2];
       if ( $carray[1] == "delete" )
          $output = "Removing jail: ". $carray[2];
    }

    $output = "<img align=absmiddle height=40 width=40 src=\"../images/working.gif\"> " . $output;
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
  if ( $pkgUpdates )
    $output="<img src=\"/images/warning.png\" align=\"absmiddle\" height=35 width=35 title=\"Updates are available!\"> <class id=updatesavail style=\"text-decoration: underline;\">Updates available </class>" . $output;

?>
<a href="?p=dispatcher"><?php echo "$output"; ?></a>
