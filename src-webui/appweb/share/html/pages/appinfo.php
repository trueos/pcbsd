<?php
defined('DS') OR die('No direct access allowed.');

function do_service_action()
{
  global $pbiorigin;
  global $sc;
  global $jail;
  global $jailUrl;

  $sname=$_GET['service'];
  $sscript=$_GET['servicerc'];
  $action=$_GET['action'];
  if ( empty($sname) or empty($sscript) or empty($action) )
    return;

  if ( $jail == "#system" )
     $output = run_cmd("service $action $sname $sscript $jailUrl");
  else {
     // Get jail ID
     exec("$sc ". escapeshellarg("jail ". $jail . " id"), $jarray);
     $jid=$jarray[0];
     $output = run_cmd("service $action $sname $sscript $jid");
  }

  if ( $action == "start" )
     echo "Started $sname on $jail<br>";  
  if ( $action == "stop" )
     echo "Stopped $sname on $jail<br>";  
  if ( $action == "restart" )
     echo "Restarted $sname on $jail<br>";  

   $newUrl=http_build_query($_GET);
   $app=str_replace("/", "%2F", $app);
   $newUrl=str_replace("&service=$sname", "", $newUrl);
   $newUrl=str_replace("service=$sname", "", $newUrl);
   $newUrl=str_replace("&servicerc=$sscript", "", $newUrl);
   $newUrl=str_replace("servicerc=$sscript", "", $newUrl);
   $newUrl=str_replace("&action=$action", "", $newUrl);
   $newUrl=str_replace("action=$action", "", $newUrl);
   hideurl("?".$newUrl);
}

function parse_service_start()
{
  global $pbicdir;
  global $pbiorigin;
  global $pbiindexdir;
  global $jail;
  global $jailUrl;
  global $sc;

  $lines = file($pbicdir . "/service-start");
  foreach($lines as $line_num => $line)
  {
    $cline = trim($line);
    if ( empty($cline) )
       continue;
    if ( strpos($cline, "#") === 0 )
       continue;

    $sline = preg_replace("/[[:blank:]]+/"," ",$cline);
    $sarray = explode(" ", $sline);
    
    // lets check if this service is enabled in etc/rc.conf
    if ( $jail == "#system" )
       $rcconf="/etc/rc.conf";
    else {
       // Get jail path
       exec("$sc ". escapeshellarg("jail ". $jail . " path"), $jarray);
       $rcconf=$jarray[0] . "/etc/rc.conf";
    }

    // Now look if the service is already enabled
    $sflag = $sarray[0] . '_enable="YES"';
    $contents = file_get_contents($rcconf);
    $pattern = preg_quote($sflag, '/');
    $pattern = "/^.*$pattern.*\$/m";
    if (preg_match_all($pattern, $contents, $matches))
       $senabled=true;
    else
       $senabled=false;

    if ( $senabled ) {
      echo "<a href=\"?p=appinfo&app=".rawurlencode($pbiorigin)."&jail=$jailUrl&service=$sarray[0]&servicerc=$sarray[1]&action=stop\"><img src=\"/images/application-exit.png\" height=24 width=24> Stop $sarray[0]</a><br>\n";
      echo "<a href=\"?p=appinfo&app=".rawurlencode($pbiorigin)."&jail=$jailUrl&service=$sarray[0]&servicerc=$sarray[1]&action=restart\"><img src=\"/images/restart.png\" height=24 width=24> Restart $sarray[0]</a><br>\n";
    } else
      echo "<a href=\"?p=appinfo&app=".rawurlencode($pbiorigin)."&jail=$jailUrl&service=$sarray[0]&servicerc=$sarray[1]&action=start\"><img src=\"/images/start.png\" height=24 width=24> Start $sarray[0]</a><br>\n";

  }

}

function parse_service_config()
{
  global $pbicdir;
  global $pbiorigin;
  global $pbiindexdir;
  global $jail;
  global $sc;

  $lines = file($pbicdir . "/service-configure");
  foreach($lines as $line_num => $line)
  {
    $cline = trim($line);
    if ( empty($cline) )
       continue;
    if ( strpos($cline, "#") === 0 )
       continue;

    $sline = preg_replace("/[[:blank:]]+/"," ",$cline);
    $sarray = explode(" ", $sline);
    
    if ( $jail == "#system" )
      $ip = "localhost";
    else {
      // Get jail address
      exec("$sc " 
           . escapeshellarg("jail ". $jail . " ipv4")
           , $jarray);
      $ip = $jarray[0];
      $ip = substr(strstr($ip, "|"), 1);
      $ip = substr($ip, 0, strpos($ip, "/"));
    }

    // Split up our variables
    $stype = array_shift($sarray);
    $surl = array_shift($sarray);
    foreach( $sarray as $selem)
      $snickname = $snickname . " " . $selem;

    if ( $stype == "URL" ) {
      $newurl = str_replace("{IP}", $ip, $surl);
      if ( strpos($newurl, "http") === false )
         $newurl = "http://" . $newurl;
      echo "<a href=\"$newurl\" target=\"_new\"><img src=\"/images/configure.png\" height=24 width=24> $snickname</a><br>\n";
    }

  }

}

// Run the done_cfg script
function done_cfg()
{
  global $jail;
  global $jailUrl;
  global $pbicdir;
  global $sc;

  $jid = "__system__";
  if ( $jail != "#system" ) {
    exec("$sc ". escapeshellarg("jail ". $jail . " id"), $jarray);
    $jid=$jarray[0];
  }

  // Talk to dispatcher to run done script
  $output = run_cmd("donecfg ". escapeshellarg($pbicdir) ." ".escapeshellarg($jid));
}

// Set the current value for a config file setting
function set_cfg_value($cfg, $value)
{
  global $jail;
  global $jailUrl;
  global $updatedConfig;
  global $pbicdir;
  global $sc;
  $updatedConfig=true;

  $key = $cfg['key'];

  $jid = "__system__";
  if ( $jail != "#system" ) {
    exec("$sc ". escapeshellarg("jail ". $jail . " id"), $jarray);
    $jid=$jarray[0];
  }

  // Talk to dispatcher to set config value
  $output = run_cmd("setcfg ". escapeshellarg($pbicdir) ." ".escapeshellarg($jid)." ". escapeshellarg($key) .  " " . escapeshellarg($value) );
}

// Get the current value for a config file setting
function get_cfg_value($cfg)
{
  global $jail;
  global $jailUrl;
  global $pbicdir;
  global $sc;

  $key = $cfg['key'];
  $default = $cfg['default'];

  $jid = "__system__";
  if ( $jail != "#system" ) {
    exec("$sc ". escapeshellarg("jail ". $jail . " id"), $jarray);
    $jid=$jarray[0];
  }
  
  // Talk to dispatcher to get config value
  $output = run_cmd("getcfg ". escapeshellarg($pbicdir) ." ".escapeshellarg($jid)." ". escapeshellarg($key) );
  if ( ! empty($output[0]) )
     return $output[0];

  return $default; 
}

// Display a input = number type box
function display_number_box($cfg)
{
  global $currentval;
  global $desc;
  global $longdesc;
  global $default;
  global $postkey;
  global $itemup;

  $min = $cfg['min'];
  $max = $cfg['max'];
  echo "  <tr>\n";
  echo "    <td><input type=\"number\" title=\"$longdesc\" name=\"$postkey\" min=\"$min\" max=\"$max\" value=\"$currentval\"></td>\n";
  echo "    <td title=\"$longdesc\">$desc$itemup</td>\n";
  echo "  </tr>\n";
}

// Display a option box
function display_combo_box($cfg)
{
  global $currentval;
  global $desc;
  global $longdesc;
  global $default;
  global $postkey;
  global $itemup;

  echo "  <tr>\n";
  echo "    <td><select title=\"$longdesc\" name=\"$postkey\">";
  $i=1;
  for ( ;; ) {
    $akey = "option" . $i;
    if ( empty($cfg[$akey]) )
       break;
    unset($ops);
    $ops = explode("::::", $cfg[$akey]);
    $option = $ops[0];
    $disp = $ops[1];
    $selected="";
    if ( $option == $currentval )
      $selected="selected";
    echo "      <option value=\"$option\" $selected>$disp</option>\n";
    $i++;
  }
  echo "    </select></td>\n";
  echo "    <td title=\"$longdesc\">$desc$itemup</td>\n";
  echo "  </tr>\n";
}

// Display a string/password input box
function display_string_box($cfg)
{
  global $currentval;
  global $desc;
  global $longdesc;
  global $default;
  global $postkey;
  global $itemup;

  $maxlen = $cfg['maxlen'];

  if ( empty($maxlen) )
     $maxlen="20";

  $type = "text";
  if ( $cfg['type'] == "PASSWORDBOX" )
     $type = "password";

  echo "  <tr>\n";
  echo "    <td><input type=\"$type\" title=\"$longdesc\" name=\"$postkey\" value=\"$currentval\" maxlength=\"$maxlen\"></td>\n";
  echo "    <td title=\"$longdesc\">$desc$itemup</td>\n";
  echo "  </tr>\n";
}

// Display the configuration widget
function display_config_details()
{
  global $pbicdir;
  global $pbiorigin;
  global $jail;
  global $jailUrl;
  global $jailPath;
  global $sc;

  global $updatedConfig;
  $updatedConfig = false;

  // Get the jail path
  exec("$sc ". escapeshellarg("jail $jail path"), $jArray);
  $jailPath=$jArray[0];
  
  // Init the array to load in config data
  unset($appConfig);
  $appConfig = array();

  // Load the config file
  require($pbicdir . "/service-configfile");

  // Start the form
  echo "<form method=\"post\" action=\"?p=appinfo&app=".rawurlencode($pbiorigin)."&jail=$jailUrl#tabs-configure\">\n";
  echo " <table class=\"jaillist\" style=\"width:100%\">";
  echo "  <tr>\n";
  echo "   <th></th>\n";
  echo "   <th width=99%></th>\n";
  echo "  </tr>";

  // Set some globals we use for widget items
  global $currentval;
  global $desc;
  global $longdesc;
  global $default;
  global $postkey;
  global $itemup;

  // Now loop through the array and build the form
  foreach ($appConfig as $cfgWidget) {
    // Skip any array missing the type
    if ( empty($cfgWidget['type']) )
       continue;

    // Get some of the basic settings
    $currentval = get_cfg_value($cfgWidget);
    $desc = $cfgWidget['desc'];
    $longdesc = str_replace("<br>", "\n", $cfgWidget['longdesc']);
    $default = $cfgWidget['default'];
    $postkey = md5($desc);
    $itemup = "";

    // Check if the value has been updated in the UI
    if ( ! empty($_POST["$postkey"]) ) {
       $newval = $_POST["$postkey"];
       if ( $currentval != $newval ) {
         $currentval = $newval;
         set_cfg_value($cfgWidget, $currentval);
         $itemup="<img src=\"/images/dialog-ok.png\" title=\"Updated!\" width=22 height=22 style=\"float: right;\">";
         $updatedConfig=true;
       }
    }

    switch ($cfgWidget['type']) {
        case "COMBOBOX":
            display_combo_box($cfgWidget);
            break;
        case "NUMBERBOX":
            display_number_box($cfgWidget);
            break;
        case "STRINGBOX":
        case "PASSWORDBOX":
            display_string_box($cfgWidget);
            break;
    }
   
  }

  // All done, finish the form
  echo "  <tr><td colspan=2 align=center><input type=\"submit\" value=\"Save Config\" class=\"btn-style\"></td></tr>\n";
  echo " </table>\n";
  echo "<form>\n";

  // If we updated settings, we can now run the done script
  if ( $updatedConfig )
    done_cfg();
}

// Display the service details
function display_service_details()
{
  global $pbicdir;

  // Does this have rc.d scripts to start?
  if ( file_exists($pbicdir . "/service-start") )
     parse_service_start();

  // Check if this has a service configuration 
  if ( file_exists($pbicdir . "/service-configure") )
     parse_service_config();

}

function display_install_chooser()
{
  global $pbiorigin;
  global $pbiname;
  global $jailUrl;
  global $jail;
  global $pbiInstalled;
  global $pkgCmd;
  global $SCERROR;

   if ( $pbiInstalled ) {
     $output="";
     exec("/usr/local/bin/syscache ".escapeshellarg("pkg $jail local $pbiorigin rdependencies"), $output);
     // Dont display removal option unless app is not required by others
     if ( "$output[0]" == "$SCERROR" )
       print("    <button title=\"Delete $pbiname from $jail\" style=\"background-color: Transparent;background-repeat:no-repeat;border: none;background-image: url('/images/application-exit.png');background-size: 100%; height: 48px; width: 48px;\" onclick=\"delConfirm('" . $pbiname ."','".rawurlencode($pbiorigin)."','".$pkgCmd."','".$jailUrl."')\" height=48 width=48></button>\n");
     else
	print("<center><img align=\"center\" height=48 width=48 src=\"/images/warning.png\" alt=\"This application has dependencies which prevent it from being removed.\"><p>Required</p></center>");
   } else {
     global $pbiindexdir;
     if ( file_exists("$pbiindexdir/$pbiorigin/LICENSE") ) {
       // Read the license data
       $pbilic = file_get_contents("$pbiindexdir/$pbiorigin/LICENSE");
       print("    <button title=\"Install $pbiname\" style=\"background-color: Transparent;background-repeat:no-repeat;border: none;\" onclick=\"addConfirmLic('" . $pbiname ."','".rawurlencode($pbiorigin)."','".$pkgCmd."','".$jailUrl."','".$pbilic."')\"><img src=\"/images/install.png\" height=48 width=48></button>\n");
     } else {
       print("    <button title=\"Install $pbiname\" style=\"background-color: Transparent;background-repeat:no-repeat;border: none;\" onclick=\"addConfirm('" . $pbiname ."','".rawurlencode($pbiorigin)."','".$pkgCmd."','".$jailUrl."')\"><img src=\"/images/install.png\" height=48 width=48></button>\n");
     }
  }

}

function display_app_link($pbilist, $jail)
{

  $rlist = explode(" ", $pbilist);
  $totalCols = 2;
  $col = 1;
  echo " <table class=\"jaillist\" style=\"width:100%\">";
  echo "  <tr>\n";
  echo "   <th></th>\n";
  echo "   <th></th>\n";
  echo "  </tr>";

  foreach($rlist as $related) {
    parse_details($related, $jail, $col);
    if ( $col == $totalCols )
       $col = 1;
    else
       $col++;
    }

    // Close off the <tr>
    if ( $col != $totalCols ) {
      while ($col <= $totalCols ) {
        echo "<td>&nbsp;</td>";
        $col++;
      }
    }
    echo "</tr>";

    echo "</table>\n";
}

  // Start the main script now
  if ( empty($_GET['app']) )
     die("Missing app=");

  $pbiorigin = $_GET['app'];

  // Check if we are starting / stopping a service
  if ( ! empty($_GET['service']) )
     do_service_action();

  $repo="remote";
  // Load the PBI details page
  $cmd="pbi app $pbiorigin";
  exec("$sc ". escapeshellarg("$cmd name") 
     . " " . escapeshellarg("pkg $jail $repo $pbiorigin version") 
     . " " . escapeshellarg("$cmd author")
     . " " . escapeshellarg("$cmd website") 
     . " " . escapeshellarg("$cmd comment")
     . " " . escapeshellarg("$cmd confdir")
     . " " . escapeshellarg("$cmd description")
     . " " . escapeshellarg("pkg $jail $repo $pbiorigin name")
     . " " . escapeshellarg("pkg $jail $repo $pbiorigin size")
     , $pbiarray);

  $pbiname = $pbiarray[0];
  $pbiver = $pbiarray[1];
  $pbiauth = $pbiarray[2];
  $pbiweb = $pbiarray[3];
  $pbicomment = $pbiarray[4];
  $pbicdir = $pbiarray[5];
  $pbidesc = $pbiarray[6];
  $pkgsize = $pbiarray[8];

  if ( empty($pbiname) or $pbiname == "$SCERROR" ) {
     $isPBI = false;
     $pbiname = $pbiarray[7];
  } else {
     $isPBI = true;
  }

  if ( empty($pbiname) )
    die("No such app: $pbi");

  if ( $isPBI ) {
    // Get second tier PBI data
    $cmd="pbi app $pbiorigin";
    unset($pbiarray);
    exec("$sc ". escapeshellarg("$cmd license") 
      . " " . escapeshellarg("$cmd type") 
      . " " . escapeshellarg("$cmd tags") 
      . " " . escapeshellarg("$cmd relatedapps") 
      . " " . escapeshellarg("$cmd plugins") 
      . " " . escapeshellarg("$cmd options") 
      . " " . escapeshellarg("$cmd rating")
      . " " . escapeshellarg("$cmd screenshots")
      . " " . escapeshellarg("pkg $jail remote $pbiorigin dependencies")
      , $pbiarray);
    $pbilicense = $pbiarray[0];
    $pbitype = $pbiarray[1];
    $pbitags = $pbiarray[2];
    $pbirelated = $pbiarray[3];
    $pbiplugins = $pbiarray[4];
    $pbioptions = $pbiarray[5];
    $pbirating = $pbiarray[6];
    $pbiss = $pbiarray[7];
    $pbideps = $pbiarray[8];
    if ( $pbideps == $SCERROR)
       unset($pbideps);
    $pkgCmd="pbi";
  } else {
    $pkgCmd="pkg";

    // Not a PBI, fallback to loading data from PKGNG
    exec("$sc ". escapeshellarg("pkg $jail $repo $pbiorigin maintainer") 
       . " " . escapeshellarg("pkg $jail $repo $pbiorigin website")
       . " " . escapeshellarg("pkg $jail $repo $pbiorigin comment")
       . " " . escapeshellarg("pkg $jail $repo $pbiorigin description")
       , $pkgarray);
    $pbiauth = $pkgarray[0];
    $pbiweb = $pkgarray[1];
    $pbicomment = $pkgarray[2];
    $pbidesc = $pkgarray[3];

  }

  // Get the current work queue status of the dispatcher
  $dStatus = getDispatcherStatus();

  // Check if this app has service details
  $hasService=false;
  if ( $isPBI and ( file_exists($pbicdir . "/service-start") or file_exists($pbicdir . "/service-configure") ) )
     $hasService=true;

  // Check if this app has config files to edit
  $hasConfig=false;
  if ( $isPBI and ( file_exists($pbicdir . "/service-configfile") ) )
     $hasConfig=true;

   // Does this PBI have icons?
   $hasIcons=false;
   if ( $onDesktop == "true" and $isPBI )
      if ( file_exists($pbicdir . "/xdg-desktop") or file_exists($pbicdir . "/xdg-menu") )
         $hasIcons=true;

   // Check if this app is installed
   $pkgoutput = syscache_ins_pkg_list("$jail");
   $pkglist = explode(", ", $pkgoutput[0]);
   if ( array_search($pbiorigin, $pkglist) !== false)
      $pbiInstalled = true;
   else
      $pbiInstalled = false;

?>
   
<br>
<table class="jaillist" style="width:<?php if ( $deviceType == "computer" ) { echo "600px"; } else { echo "100%"; } ?>">
  <tr>
    <th colspan=3>
      <?php 
         echo "$pbiname - $pbiver"; 
 	 if ( "$jail" != "#system" )
           echo " ($jail)";
      ?>
    </th>
  </tr>
  <tr>
     <td style="vertical-align: middle; width: 60px;">
      <?php
 	 $appbusy=false;
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
	   print("<img align=\"center\" valign=\"center\" src=\"images/working.gif\" title=\"Working...\">");
	   echo("<script>setTimeout(function () { location.reload(1); }, 8000);</script>");
         } else {
	   display_install_chooser();
	 }
      ?>
    </td>
    <td align=left style="">
      <img align="left" height=64 width=64 src="images/pbiicon.php?i=<?php echo "$pbicdir"; ?>/icon.png">
       <a href="<?php echo "$pbiweb"; ?>" target="_new"><?php echo "$pbiauth"; ?></a> 
       <a href="http://www.freshports.org/<?php echo "$pbiorigin"; ?>" target="_new"><img src="/images/external-link.png" height=20 width=20 title="View this package in FreshPorts"></a><br>
       Version: <b><?php echo "$pbiver"; ?></b><br>
<?php
  if ( $isPBI ) {
    if ( ! empty($pbirating) and $pbirating != $SCERROR ) {
      if ( strpos($pbirating, "5") === 0 )
        print("<a href=\"http://wiki.pcbsd.org/index.php/AppCafe/$pbiorigin\" target=\"_new\"><img src=\"/images/rating-5.png\" height=16 width=80 title=\"$pbirating\"></a>");
      if ( strpos($pbirating, "4") === 0 )
        print("<a href=\"http://wiki.pcbsd.org/index.php/AppCafe/$pbiorigin\" target=\"_new\"><img src=\"/images/rating-4.png\" height=16 width=80 title=\"$pbirating\"></a>");
      if ( strpos($pbirating, "3") === 0 )
        print("<a href=\"http://wiki.pcbsd.org/index.php/AppCafe/$pbiorigin\" target=\"_new\"><img src=\"/images/rating-3.png\" height=16 width=80 title=\"$pbirating\"></a>");
      if ( strpos($pbirating, "2") === 0 )
        print("<a href=\"http://wiki.pcbsd.org/index.php/AppCafe/$pbiorigin\" target=\"_new\"><img src=\"/images/rating-2.png\" height=16 width=80 title=\"$pbirating\"></a>");
      if ( strpos($pbirating, "1") === 0 )
        print("<a href=\"http://wiki.pcbsd.org/index.php/AppCafe/$pbiorigin\" target=\"_new\"><img src=\"/images/rating-1.png\" height=16 width=80 title=\"$pbirating\"></a>");
      if ( strpos($pbirating, "0") === 0 )
        print("<a href=\"http://wiki.pcbsd.org/index.php/AppCafe/$pbiorigin\" target=\"_new\"><img src=\"/images/rating-0.png\" height=16 width=80 title=\"No rating yet, click to rate!\"></a>");
     } else
        print("<a href=\"http://wiki.pcbsd.org/index.php/AppCafe/$pbiorigin\" target=\"_new\"><img src=\"/images/rating-0.png\" height=16 width=80 title=\"No rating yet, click to rate!\"></a>");

     print(" <a href=\"http://wiki.pcbsd.org/index.php/AppCafe/$pbiorigin\" target=\"_new\"><img src=\"/images/info-tips.png\" height=18 width=18 title=\"Wiki Page\"></a><br>");
   }

   print("Size: $pkgsize<br>");
   print("</td>");

   // Time to show icon menu
   if ( $hasIcons and $pbiInstalled ) {
      echo "<td align=\"left\" style=\"font-size: 75%\">\n";
      echo "	<b>Icons:</b><br>\n";
      // Do we have desktop icons?
      if ( file_exists($pbicdir . "/xdg-desktop") )
        echo "<a href=\"appcafe: pbi icon add-desktop $pbiorigin \" style=\"text-decoration: underline;\">Add Desktop</a><br>";

      // Do we have menu icons?
      if ( file_exists($pbicdir . "/xdg-menu") ) {
        echo "<a href=\"appcafe: pbi icon add-menu $pbiorigin\" style=\"text-decoration: underline;\">Add Menu</a><br>\n";
        echo "<a href=\"appcafe: qsudo pbi icon add-menu $pbiorigin\" style=\"text-decoration: underline;\">Add Menu (All Users)</a><br>";
      }
     echo "</td>\n";
   }
?>
  </tr>
  <tr>
    <td colspan="3">
       <p><?php echo $pbidesc; ?></p>
    </td>
  </tr>

<?php if ( $isPBI) { ?>

  <tr>
    <td colspan="3">
<div id="tab-container" class='tab-container'>
   <ul class='etabs'>
     <?php  if ( $hasService and $pbiInstalled ) { ?>
     <li class='tab'><a href="#tabs-service">Services</a></li>
     <?php } ?>
     <?php  if ( $hasConfig and $pbiInstalled ) { ?>
     <li class='tab'><a href="#tabs-configure">Configuration</a></li>
     <?php } ?>
     <?php  if ( ! empty($pbiss) ) { ?>
     <li class='tab'><a href="#tabs-screenshots">Screenshots</a></li>
     <?php } ?>
     <?php  if ( ! empty($pbirelated) ) { ?>
     <li class='tab'><a href="#tabs-related">Related</a></li>
     <?php } ?>
     <?php  if ( ! empty($pbiplugins) ) { ?>
     <li class='tab'><a href="#tabs-plugins">Plugins</a></li>
     <?php } ?>
     <?php  if ( ! empty($pbioptions) ) { ?>
     <li class='tab'><a href="#tabs-options">Options</a></li>
     <?php } ?>
     <?php  if ( ! empty($pbideps) ) { ?>
     <li class='tab'><a href="#tabs-deps">Dependencies</a></li>
     <?php } ?>
   </ul>
   <div class="panel-container">
     <?php  // Do we have screenshots to display?
         if ( $hasService and $pbiInstalled ) {
            echo "<div id=\"tabs-service\">\n";
	    display_service_details();
	    echo "</div>\n";
	 }
         if ( $hasConfig and $pbiInstalled ) {
            echo "<div id=\"tabs-configure\">\n";
	    display_config_details();
	    echo "</div>\n";
	 }
         if ( ! empty($pbiss) ) {
            echo "<div id=\"tabs-screenshots\">\n";
            $sslist = explode(" ", $pbiss);
            foreach($sslist as $screenshot)
              echo "<a href=\"$screenshot\" target=\"_new\" data-lightbox=\"screenshots\"><img border=0 src=\"$screenshot\" height=128 width=128></a>&nbsp;";
	    echo "</div>\n";
         }

	 // Do we have related items to show?
         if ( ! empty($pbirelated) ) {
            echo "<div id=\"tabs-related\">\n";
	    display_app_link($pbirelated, $jail);
	    echo "</div>\n";
         }

	 // Do we have plugins to show?
         if ( ! empty($pbiplugins) ) {
            echo "<div id=\"tabs-plugins\">\n";
	    display_app_link($pbiplugins, $jail);
	    echo "</div>\n";
         }

	 // Do we have options to show?
         if ( ! empty($pbioptions) ) {
            echo "<div id=\"tabs-options\">\n";
            $olist = explode(" ", $pbioptions);
            foreach($olist as $option)
              echo "  <b>$option</b><br>\n";
	    echo "</div>\n";
         }
	 // Do we have deps to show?
         if ( ! empty($pbideps) ) {
            echo "<div id=\"tabs-deps\">\n";
            $dlist = explode(" ", $pbideps);
            foreach($dlist as $dep)
              echo "  <b>$dep</b><br>\n";
	    echo "</div>\n";
         }
     ?>
   </div>
</div>
    </td>
  </tr>

<?php } ?>

</table>

<script type="text/javascript">
  $('#tab-container').easytabs();
</script>
