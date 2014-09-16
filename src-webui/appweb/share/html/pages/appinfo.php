<?
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

// Display the configuration widget
function display_config_details()
{


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

   if ( $pbiInstalled )
     print("    <button title=\"Delete $pbiname from $jail\" style=\"background-color: Transparent;background-repeat:no-repeat;border: none;background-image: url('/images/application-exit.png');background-size: 100%; height: 48px; width: 48px;\" onclick=\"delConfirm('" . $pbiname ."','".rawurlencode($pbiorigin)."','".$pkgCmd."','".$jailUrl."')\" height=48 width=48></button>\n");
  else
     print("    <button title=\"Install $pbiname into $jail\" style=\"background-color: Transparent;background-repeat:no-repeat;border: none;background-image: url('/images/install.png');background-size: 100%; height: 48px; width: 48px;\" onclick=\"addConfirm('" . $pbiname ."','".rawurlencode($pbiorigin)."','".$pkgCmd."','".$jailUrl."')\"></button>\n");

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
    if ( $col == $totalCols )
       echo "  </tr>\n";

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
<table class="jaillist" style="width:<? if ( $deviceType == "computer" ) { echo "600px"; } else { echo "100%"; } ?>">
  <tr>
    <th colspan=3>
      <? 
         echo "$pbiname - $pbiver"; 
 	 if ( "$jail" != "#system" )
           echo " ($jail)";
      ?>
    </th>
  </tr>
  <tr>
     <td style="vertical-align: middle; width: 60px;">
      <?
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
    <td align=left style="width: <? if($hasIcons){ print("60%"); } else { print("100%"); }?>">
      <img align="left" height=64 width=64 src="images/pbiicon.php?i=<? echo "$pbicdir"; ?>/icon.png">
       <a href="<? echo "$pbiweb"; ?>" target="_new"><? echo "$pbiauth"; ?></a> 
       <a href="http://www.freshports.org/<? echo "$pbiorigin"; ?>" target="_new"><img src="/images/external-link.png" height=20 width=20 title="View this package in FreshPorts"></a><br>
       Version: <b><? echo "$pbiver"; ?></b><br>
<?
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
   if ( $hasIcons ) {
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
       <p><? echo $pbidesc; ?></p>
    </td>
  </tr>

<? if ( $isPBI) { ?>

  <tr>
    <td colspan="3">
<div id="tab-container" class='tab-container'>
   <ul class='etabs'>
     <?  if ( $hasService and $pbiInstalled ) { ?>
     <li class='tab'><a href="#tabs-service">Services</a></li>
     <? } ?>
     <?  if ( $hasConfig and $pbiInstalled ) { ?>
     <li class='tab'><a href="#tabs-configure">Configuration</a></li>
     <? } ?>
     <?  if ( ! empty($pbiss) ) { ?>
     <li class='tab'><a href="#tabs-screenshots">Screenshots</a></li>
     <? } ?>
     <?  if ( ! empty($pbirelated) ) { ?>
     <li class='tab'><a href="#tabs-related">Related</a></li>
     <? } ?>
     <?  if ( ! empty($pbiplugins) ) { ?>
     <li class='tab'><a href="#tabs-plugins">Plugins</a></li>
     <? } ?>
     <?  if ( ! empty($pbioptions) ) { ?>
     <li class='tab'><a href="#tabs-options">Options</a></li>
     <? } ?>
     <?  if ( ! empty($pbideps) ) { ?>
     <li class='tab'><a href="#tabs-deps">Dependencies</a></li>
     <? } ?>
   </ul>
   <div class="panel-container">
     <?  // Do we have screenshots to display?
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

<? } ?>

</table>

<script type="text/javascript">
  $('#tab-container').easytabs();
</script>
