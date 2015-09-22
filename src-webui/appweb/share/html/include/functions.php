<?php
defined('DS') OR die('No direct access allowed.');

// Set the error string syscache returns if a particular request
// isn't available
$SCERROR="[ERROR] Information not available";

function hideurl($newurl = "")
{
   if (empty($newurl) )
     $p = "/?p=" . $_GET['p'];
   else
     $p = "$newurl";
   ?>
   <script>
	window.history.pushState('AppWeb', 'AppWeb', '<?php echo $p; ?>');
   </script>
   <?php
}

// Runs commands through the sudo dispatcher
function run_cmd($cmd)
{
   global $DISPATCHID;
   putenv("PHP_DISID=$DISPATCHID");
   exec("/usr/local/bin/sudo /usr/local/share/appcafe/dispatcher $cmd", $output);
   return $output;
}

function syscache_ins_pkg_list($jail="")
{
   $jail = "#system";
   exec("/usr/local/bin/syscache ".escapeshellarg("pkg $jail installedlist"), $output);
   return $output;
}

function syscache_ins_plugin_list()
{
   exec("/usr/local/bin/syscache ".escapeshellarg("jail stoppedcages"), $output);
   exec("/usr/local/bin/syscache ".escapeshellarg("jail runningcages"), $output1);
   if ( empty($output) )
     return $output1;
   if ( empty($output1) )
     return $output;
   $clist[] = $output[0]. ", " .$output1[0];
   return $clist;
}

function syscache_pbidb_list($flag="allapps")
{
   exec("/usr/local/bin/syscache ".escapeshellarg("pbi list $flag"), $output);
   return $output;
}

function send_sc_query($cmdarray)
{
   global $scclient;

   $jarray = array();

   // Build the json array
   $num = 0;
   foreach($cmdarray as $cmd)
   {
     // Make sure we don't have a duplicate in the cmd list
     if ( array_search($cmd, $jarray) === false ) {
       $jarray["request$num"] = "$cmd";
       $num++;
     }
   }
   $scclient->send(json_encode($jarray));
   $rjson = $scclient->receive();
   $rarray = json_decode($rjson, true);
   return $rarray;
}

function queueInstallApp()
{
   $jail = "#system";
   $jailUrl="__system__";

   $app = $_GET['installApp'];
   $type = $_GET['installAppCmd'];

   if ( ! empty($app) and ! empty($type) and ! empty($jail) )
      run_cmd("queue $type $app install $jailUrl");
 
   // Now we can remove those values from the URL
   $newUrl=http_build_query($_GET);
   $app=str_replace("/", "%2F", $app);
   $newUrl=str_replace("&installApp=$app", "", $newUrl);
   $newUrl=str_replace("installApp=$app", "", $newUrl);
   $newUrl=str_replace("&installAppCmd=$type", "", $newUrl);
   $newUrl=str_replace("installAppCmd=$type", "", $newUrl);
   hideurl("?".$newUrl);
}

function queueDeleteApp()
{
   $jail = "#system";
   $jailUrl="__system__";

   $app = $_GET['deleteApp'];
   $type = $_GET['deleteAppCmd'];

   if ( ! empty($app) and ! empty($type) and ! empty($jail) )
     run_cmd("queue $type $app delete $jailUrl");

   // Now we can remove those values from the URL
   $newUrl=http_build_query($_GET);
   $app=str_replace("/", "%2F", $app);
   $newUrl=str_replace("&deleteApp=$app", "", $newUrl);
   $newUrl=str_replace("deleteApp=$app", "", $newUrl);
   $newUrl=str_replace("&deleteAppCmd=$type", "", $newUrl);
   $newUrl=str_replace("deleteAppCmd=$type", "", $newUrl);
   hideurl("?".$newUrl);
}

function queueInstallPlugin()
{
   $origin = $_GET['installPlugin'];
   $ghurl = $_GET['installPluginGH'];

   if ( ! empty($origin) and ! empty($ghurl) )
     $output = run_cmd("queue iocage pull $origin $ghurl");

   // Now we can remove those values from the URL
   $newUrl=http_build_query($_GET);
   $origin=str_replace("/", "%2F", $origin);
   $newUrl=str_replace("&installPlugin=$origin", "", $newUrl);
   $newUrl=str_replace("installPlugin=$origin", "", $newUrl);
   hideurl("?".$newUrl);
}

function queueDeletePlugin()
{
   $ioid = $_GET['deletePlugin'];
   $app = $_GET['app'];

   if ( ! empty($ioid) )
     run_cmd("queue iocage destroy $app $ioid");

   // Now we can remove those values from the URL
   $newUrl=http_build_query($_GET);
   $newUrl=str_replace("&deletePlugin=$ioid", "", $newUrl);
   $newUrl=str_replace("deletePlugin=$ioid", "", $newUrl);
   hideurl("?".$newUrl);
}

function getDispatcherStatus()
{
   global $dispatcherstatus;
   if ( ! empty($dispatcherstatus) )
     return $dispatcherstatus;

   $dispatcherstatus = run_cmd("status");
   return $dispatcherstatus;
}

function get_installed_list($target = "#system")
{
  $sccmd = array("pkg ". $target . " installedlist");
  $response = send_sc_query($sccmd);
  $pbilist = $response["pkg" . $target . " installedlist"];
}

function parse_details($pbiorigin, $jail, $col, $showRemoval=false, $filter=true)
{
  global $totalCols;
  global $inslist;
  global $SCERROR;
  global $sysType;
  global $allPBI;

  $jail = "#system";
  $jailUrl="__system__";

  if ( empty($inslist) )
    $inslist = get_installed_list($jail);

  $sccmd = array("$jail app-summary $pbiorigin");
  $response = send_sc_query($sccmd);
  $pbiarray = $response["$jail app-summary $pbiorigin"];
  // Output format (4/7/15): [origin, name, version, iconpath, rating, type, comment, confdir, isInstalled, canRemove]
  $pbiname = $pbiarray[1];
  $pbiver = $pbiarray[2];
  $pbiicon = $pbiarray[3];
  $pbirating = $pbiarray[4];
  $pbitype = $pbiarray[5];
  $pbicomment = $pbiarray[6];
  $pbicdir = $pbiarray[7];
  $pbiinstalled = $pbiarray[8];
  $pbicanremove = $pbiarray[9];
  if ( empty($pbitype) ) {
    $isPBI=false;
    $pkgCmd="pkg";
  } else {
    $isPBI=true;
    $pkgCmd="pbi";
  }
  // If no match, return false
  if ( empty($pbiname) or $pbiname == "$SCERROR" )
     return 1;

  if ( $allPBI == "false" )
  {
 
    // Not on a desktop, filter out Graphical types
    if ( $sysType != "DESKTOP" and $filter ) {
       if ( $pbitype == "Graphical" )
   	  return 1;
       if ( $pbitype != "Server" and $viewType != "ALL" )
	  return 1;
    }

    // If on a desktop, only list graphical types for the main system
    if ( $jail == "#system" and $sysType == "DESKTOP" and $filter ) {
       if ( $pbitype != "Graphical" )
   	  return 1;
    }

    // In a jail, see what else to filter
    if ( $jail != "#system" and $filter ) {
       // In jails we only list Server types, unless user requested CLI also
       if ( $pbitype != "Server" and $viewType != "ALL" )
	  return 1;

       // In a jail, filter out Graphical types
       if ( $pbitype == "Graphical" )
  	  return 1;
    }

  }

  if ( $col == 1 )
    print ("<tr>\n");

  // Get our values from this line
  print("  <td>\n");

  if ( strlen($pbiname) > 14 )
    $pbiname = substr($pbiname, 0, 14) . "..";

  if ( strlen($pbiver) > 14 )
    $pbiver = substr($pbiver, 0, 14) . "..";

  $appbusy=false;
  $dStatus = getDispatcherStatus();
  foreach($dStatus as $curStatus) {
    if ( strpos($curStatus, "pbi $pbiorigin") !== false ) {
      $appbusy=true;
      break;
    }
    if ( strpos($curStatus, "pkg $pbiorigin") !== false ) {
      $appbusy=true;
      break;
    }
  }
  if ( $appbusy ) {
    print("<img style=\"background-color: Transparent;background-repeat:no-repeat;border: none;float:right;\" height=22 width=22 src=\"images/working.gif\" title=\"Working...\">");
    echo("<script>setTimeout(function () { location.reload(1); }, 8000);</script>");
  } else {


    // Is this app installed?
    //if ( array_search($pbiorigin, $inslist) !== false ) {
    if ( $pbiinstalled == "true" ){
      if ( $pbicanremove == "true" )
        print("    <button title=\"Delete $pbiname\" style=\"background-color: Transparent;background-repeat:no-repeat;border: none;float:right;\" onclick=\"delConfirm('" . $pbiname ."','".rawurlencode($pbiorigin)."','".$pkgCmd."','".$jailUrl."')\"><img src=\"/images/application-exit.png\" height=22 width=22></button>\n");
    } else {
     print("    <button title=\"Install $pbiname\" style=\"background-color: Transparent;background-repeat:no-repeat;border: none;float:right;\" onclick=\"addConfirm('" . $pbiname ."','".rawurlencode($pbiorigin)."','".$pkgCmd."','".$jailUrl."')\"><img src=\"/images/install.png\" height=22 width=22></button>\n");
    }
  }

  print("    <a href=\"/?p=appinfo&app=".rawurlencode($pbiorigin)."&allPBI=$allPBI\" title=\"$pbicomment\"><img border=0 align=\"center\" height=48 width=48 src=\"/images/pbiicon.php?i=$pbicdir/icon.png\" style=\"float:left;\"></a>\n");
  print("    <a href=\"/?p=appinfo&app=".rawurlencode($pbiorigin)."&allPBI=$allPBI\" style=\"margin-left:5px;\">$pbiname</a><br>\n");
  print("    <a href=\"/?p=appinfo&app=".rawurlencode($pbiorigin)."&allPBI=$allPBI\" style=\"margin-left:5px;\">$pbiver</a><br>\n");
  if ( ! empty($pbirating) and $pbirating != $SCERROR ) {
    if ( strpos($pbirating, "5") === 0 )
      print("<img src=\"/images/rating-5.png\" height=16 width=80 title=\"$pbirating\">");
    if ( strpos($pbirating, "4") === 0 )
      print("<img src=\"/images/rating-4.png\" height=16 width=80 title=\"$pbirating\">");
    if ( strpos($pbirating, "3") === 0 )
      print("<img src=\"/images/rating-3.png\" height=16 width=80 title=\"$pbirating\">");
    if ( strpos($pbirating, "2") === 0 )
      print("<img src=\"/images/rating-2.png\" height=16 width=80 title=\"$pbirating\">");
    if ( strpos($pbirating, "1") === 0 )
      print("<img src=\"/images/rating-1.png\" height=16 width=80 title=\"$pbirating\">");
  }
  print("  </td>\n");

  if ( $col == $totalCols )
    print ("</tr>\n");

  return 0;
}

function display_cats($iconsize = "32")
{
  global $SCERROR;
  global $sysType;
  global $allPBI;

  $jail = "#system";
  $jailUrl="__system__";

?>
<center>- <b>Categories</b> -&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp</center><br>
<div class="onoffswitch">
    <input type="checkbox" name="onoffswitch" class="onoffswitch-checkbox" id="pbiswitch" onclick="togglePBIMode()" <?php if ( $allPBI == "false" ) { echo "checked"; }?>>
    <label class="onoffswitch-label" for="pbiswitch">
        <span class="onoffswitch-inner"></span>
        <span class="onoffswitch-switch"></span>
    </label>
</div><br>
<?php

  if ( $allPBI == "true" )
     $listcmd="pbi list allcats";
  else if ( $jail == "#system" && $sysType == "DESKTOP" )
     $listcmd="pbi list graphicalcats";
  else
     $listcmd="pbi list servercats";


  $sccmd = array("$listcmd");
  $response = send_sc_query($sccmd);
  $catlist = $response["$listcmd"];

  foreach ( $catlist as $cat ) {
    if ( empty($cat) )
      continue;

    $sccmd = array("pbi cat $cat name", "pbi cat $cat icon", "pbi cat $cat comment");
    $response = send_sc_query($sccmd);
    $catname = $response["pbi cat $cat name"];
    $caticon = $response["pbi cat $cat icon"];
    $catcomment = $response["pbi cat $cat comment"];

    echo "<img height=$iconsize width=$iconsize src=\"/images/pbiicon.php?i=$caticon\"><a href=\"?p=appcafe&cat=$cat&allPBI=$allPBI\" title=\"$catcomment\">$catname</a><br>\n";
  }

}

function get_jail_list($force=false)
{
  global $sc;
  global $jail_list_array;

  // If this is set, we have the jail list already
  if ( ! empty( $jail_list_array) and ! $force )
     return $jail_list_array;

  unset($jail_list_array);

  // Query the system for the running jail list
  exec("$sc ". escapeshellarg("jail list")
       , $jail_list_array);

  // Get the UUID of the jails only
  $jarray = array();
  foreach($jail_list_array as $jline) {
    $jitem = explode(", ", $jline);
    foreach($jitem as $jia) {
      $jail = explode(" ", $jia);
      if ( empty($jrunning) )
        $jrunning =  $jail[0];
      else
        $jrunning = $jrunning . ", " . $jail[0];
    }
  }
  $jarray[] = $jrunning;

  // Query the system for the stopped jail list
  unset($jail_list_array);
  exec("$sc ". escapeshellarg("jail stoppedlist")
       , $jail_list_array);
  foreach($jail_list_array as $jline) {
    $jitem = explode(", ", $jline);
    foreach($jitem as $jia) {
      $jail = explode(" ", $jia);
      if ( empty($jstopped) )
        $jstopped =  $jail[0];
      else
        $jstopped = $jstopped . ", " . $jail[0];
    }
  }
  $jarray[] = $jstopped;

  return $jarray;

}

// Check if a particular iocage jail ID is running
function is_jail_running($jail)
{
  global $sc;
  exec("$sc ". escapeshellarg("jail list")
       , $jail_list_array);
 
  if ( array_search($jail, $jail_list_array) !== false )
    return true; 

  return false;
}

// Check if a particular iocage pbicage is running
function is_pbicage_running($jail)
{
  global $sc;
  exec("$sc ". escapeshellarg("jail runningcages")
       , $jail_list_array);

  if ( array_search_partial($jail, $jail_list_array) !== false )
    return true;

  return false;
}

function get_nics()
{
   exec("/sbin/ifconfig ".escapeshellarg("-l"), $output);
   $nics = explode( " ", $output[0]);
   $nicarray = array();
   foreach ( $nics as $nic )
   {
     if ( $nic == "lo0" )
        continue;
     $nicarray[] = $nic;
   }
   return $nicarray;
}

function check_update_reboot() {
  global $sysType;

  // Check if the system is waiting to reboot
  if ( ($sysType == "DESKTOP" or $sysType == "SERVER") and file_exists("/tmp/.rebootRequired") )
  {
     exec("who -b", $wout);
     exec("cat /tmp/.rebootRequired", $rout);
     if ( $wout == $rout ) {
       echo "<center>The system is waiting to reboot from updating, please reboot before installing packages!</center>";
       exit(0);
     }
  }

  // Check if the system is updating
  if ( file_exists("/tmp/.updateInProgress") )
  {
     exec("pgrep -qF /tmp/.updateInProgress ; echo $?", $rout);
     if ( $rout[0] == "0" ) {
       exec("tail -20 /var/log/pc-updatemanager.log", $logout);
       echo "<meta http-equiv=\"refresh\" content=\"4\">";
       echo "<center>The system is updating, please wait for this to finish before installing packages!</center><br>";
       echo "<hr>";
       echo "<pre>";
       foreach($logout as $line)
         echo "$line\n";
       echo "</pre>";
       exit(0);
     }
  }

  return 0;
}

function parse_plugin_details($origin, $col, $showRemoval=false, $filter=true)
{
  global $totalCols;
  global $SCERROR;
  global $sysType;
  global $cage_installed;

  $sccmd = array("cage-summary $origin");
  $response = send_sc_query($sccmd);
  $pbiarray = $response["cage-summary $origin"];
  // Output format (4/7/15): [origin, name, iconpath, arch, fbsdver]
  $pbiname = $pbiarray[1];
  $pbiicon = $pbiarray[2];
  $pbiarch = $pbiarray[3];
  $pbifbsdver = $pbiarray[4];

  // If no match, return false
  if ( empty($pbiname) or $pbiname == "$SCERROR" )
     return 1;

  // Set our $cage_installed only once
  if ( empty($cage_installed) ) {
    $sccmd = array("jail stoppedcages", "jail runningcages");
    $response = send_sc_query($sccmd);
    $cage_installed[] = $response["jail stoppedcages"];
    $cage_installed[] = $response["jail runningcages"];

  }

  $pbiinstalled=false;
  if ( array_search_partial($origin . " ", $cage_installed) !== false )
    $pbiinstalled=true;

  if ( $col == 1 )
    print ("<tr>\n");

  // Get our values from this line
  print("  <td>\n");

  if ( strlen($pbiname) > 25 )
    $pbiname = substr($pbiname, 0, 14) . "..";

  $appbusy=false;
  $dStatus = getDispatcherStatus();
  foreach($dStatus as $curStatus) {
    if ( strpos($curStatus, "iocage pull $origin") !== false ) {
      $appbusy=true;
      break;
     }
     if ( strpos($curStatus, "iocage destroy $origin") !== false ) {
       $appbusy=true;
       break;
     }
   }
   if ( $appbusy ) {
     print("<img style=\"background-color: Transparent;background-repeat:no-repeat;border: none;float:right;\" height=22 width=22 src=\"images/working.gif\" title=\"Working...\">");
     echo("<script>setTimeout(function () { location.reload(1); }, 8000);</script>");
   } else {
     // Is this app installed?
     if ( $pbiinstalled == true ){
       $ioid = get_iocage_id_from_origin($origin);
       print("    <button title=\"Delete $pbiname\" style=\"background-color: Transparent;background-repeat:no-repeat;border: none;float:right;\" onclick=\"delAppConfirm('" . $pbiname ."','".rawurlencode($origin)."','".$ioid."')\"><img src=\"/images/application-exit.png\" height=22 width=22></button>\n");
     } else {
       exec("$sc ".escapeshellarg("pbi cage " . $origin . " git"), $ghrepo);
       print("    <button title=\"Install $pbiname\" style=\"background-color: Transparent;background-repeat:no-repeat;border: none;float:right;\" onclick=\"addAppConfirm('" . $pbiname ."','".rawurlencode($origin)."','".rawurlencode($ghrepo[0])."')\"><img src=\"/images/install.png\" height=22 width=22></button>\n");
     }
  }

  print("    <a href=\"/?p=plugininfo&app=".rawurlencode($origin)."\" title=\"$pbicomment\"><img border=0 align=\"center\" height=48 width=48 src=\"/images/pbiicon.php?i=$pbiicon\" style=\"float:left;\"></a>\n");
  print("    <a href=\"/?p=plugininfo&app=".rawurlencode($origin)."\" style=\"margin-left:5px;text-width:100%\">$pbiname</a><br>\n");
  print("  </td>\n");

  if ( $col == $totalCols )
    print ("</tr>\n");

  return 0;
}


function array_search_partial($keyword, $arr) {
  foreach($arr as $index => $string) {
      if (strpos($string, $keyword) !== FALSE)
          return true;
  }
  return false;
}

function display_plugin_cats($iconsize = "32")
{
  global $SCERROR;
  global $sysType;
  global $allPBI;

?>
<center>- <b>Categories</b> -&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp</center><br>
<?php

  // Get a list of available plugins
  $sccmd = array("pbi list cages");
  $response = send_sc_query($sccmd);
  $plugarray = $response["pbi list cages"];

  // Get all the PBI categories
  $sccmd = array("pbi list allcats");
  $response = send_sc_query($sccmd);
  $catlist = $response["pbi list allcats"];

  foreach ( $catlist as $cat ) {
    if ( empty($cat) )
      continue;

    // Skip any categories which have no entries
    if ( array_search_partial($cat, $plugarray) == false )
      continue;

    $sccmd = array("pbi cat $cat name", "pbi cat $cat icon", "pbi cat $cat comment");
    $response = send_sc_query($sccmd);
    $catname = $response["pbi cat $cat name"];
    $caticon = $response["pbi cat $cat icon"];
    $catcomment = $response["pbi cat $cat comment"];

    echo "<img height=$iconsize width=$iconsize src=\"/images/pbiicon.php?i=$caticon\"><a href=\"?p=plugins&cat=$cat\" title=\"$catcomment\">$catname</a><br>\n";
  }

}

function get_iocage_id_from_origin($origin)
{
  $sccmd = array("jail stoppedcages", "jail runningcages");
  $response = send_sc_query($sccmd);
  $stopped = $response["jail stoppedcages"];
  $running = $response["jail runningcages"];
  $stoppedarray = explode(", ", $stopped);
  $runningarray = explode(", ", $running);

  // Check stopped cages first
  foreach ( $stoppedarray as $jail ) {
    if ( stripos($jail, $origin . " ") !== false ) {
       $jitem = explode(" ", $jail);
       return $jitem[1];
    }
  }

  foreach ( $runningarray as $jail ) {
    if ( stripos($jail, $origin . " ") !== false ) {
      $jitem = explode(" ", $jail);
      return $jitem[1];
    }
  }

  return "";
}

function get_iocage_pool()
{
   $output = run_cmd("iocage activate");
   $rtn = explode( " ", $output[0]);
   return $rtn[1];
}

function get_zpools()
{
   exec("/sbin/zpool list ".escapeshellarg("-H"), $output);
   $zpools = array();
   foreach ( $output as $pline )
   {
     $pline=str_replace("\t", " ", $pline);
     $zarray = explode(" ", $pline);
     $zpools[] = $zarray[0];
   }
   return $zpools;
}

?>
