<?
defined('DS') OR die('No direct access allowed.');

   $refPage = $_GET['ref'];

   // Did the user request to start updates on a jail / system?
   if ( ! empty($_GET['updateTarget']) ) {
      // Time to queue up some updates for the requested target
      $updateTarget=$_GET['updateTarget'];
      if ( $updateTarget == "__system__")
        $updateTargetName="#system";
      else
        $updateTargetName="$updateTarget";

      // Queue it up now
      $dccmd = array("pkgupdate $updateTarget");
      send_dc_cmd($dccmd);

      // Now we can remove those values from the URL
      $newUrl=http_build_query($_GET);
      $app=str_replace("/", "%2F", $app);
      $newUrl=str_replace("&updateTarget=$updateTarget", "", $newUrl);
      $newUrl=str_replace("updateTarget=$updateTarget", "", $newUrl);
      hideurl("?".$newUrl);
   }

   // Did the user request log files?
   if ( ! empty($_GET['log']) )
   {
      $dccmd = array("log ". $_GET['log']);
      $response = send_dc_cmd($dccmd);
      $logoutput = explode("\n", $response["log " . $_GET['log']]);

?>

<h1>Action Log (<a href="?p=dispatcher&ref=<?php echo $refPage; ?>">Back</a>)</h1>
<br>
<table class="status" style="width:768px">

<tr>
   <th>Log output</th>
</tr>

<tr><td>
<?
   
   foreach ($logoutput as $line)
       echo "$line<br>";

   echo "</td></tr>";

   // No log file showing, show all logs
   } else {

  
     // Lets check for updates first
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

       // Sanitize the names
       if ( $jname == "#system" ) {
          $target="Local System";
          $targetUrl="__system__";
       } else {
          $target="$jname";
          $targetUrl="$jname";
       }

       // if the user just clicked to update this jail/target
       if ( $updateTargetName == $jname ) {
          continue;
       }


       unset($jarray);
       $sccmd = array("pkg $jname hasupdates");
       $response = send_sc_query($sccmd);
       $hasupdates = $response["pkg $jname hasupdates"];

       if ( $hasupdates == "true" ) {
          $pkgUpdates=true;
          // Get the list of updates to show user
          unset($jarray);
          $sccmd = array("pkg $jname updatemessage");
          $response = send_sc_query($sccmd);
          $upmsg = $response["pkg $jname updatemessage"];

          echo "<div class=\"popbox\">\n";
          echo "  <a href=\"/?p=dispatcher&updateTarget=$targetUrl&ref=$refPage\" style=\"text-decoration: underline;\"><img src=\"/images/warning.png\" height=35 width=35 title=\"Updates available!\">Update packages for $target</a>";
          echo " (<a class=\"open\" href=\"#\">details</a>)\n";
          echo "  <div class=\"collapse\">\n";
          echo "    <div class=\"box\">\n";
          echo "      <div class=\"arrow\"></div>\n";
          echo "      <div class=\"arrow-border\"></div>\n";
	  echo "        <p align=\"left\">\n";
	  echo "        $upmsg</p>\n";
	  echo "        <a href=\"#\" class=\"close\">close</a>\n";
          echo "    </div>\n";
          echo "   </div>\n";
          echo "</div>\n";
          echo "<br>\n";

echo "<script type='text/javascript' charset='utf-8'>
  $('.popbox').popbox();
</script>\n";
          break;

       }
     }

     // We have updates! Show the notification icon
     if ( ! $pkgUpdates )
       echo "<h1><center><img src=\"/images/noupdates.png\" height=24 width=24 title=\"Up to date!\">All packages and jails are up to date!</h1><br>";

?>
<h1><center>Recent application actions</h1>
<br>
<table class="status" style="width:768px">
<tr>
   <th>Action</th>
   <th>Application</th>
   <th>Target</th>
   <th>Result</th>
</tr>

<?

     $dccmd = array("results");
     $response = send_dc_cmd($dccmd);
     $rarray = explode("\n", $response["results"]);

     // Loop through the results
     $rarray = array_reverse($rarray);
     foreach ($rarray as $res) {
       $results = explode(" ", $res);
       if ( empty($results[0]) )
         continue;
       if ( $results[2] == "iocage" ) {
         echo "<tr><td>jail: $results[3]</td>";
         echo "<td>$results[4]</td>";
         echo "<td>$results[5]</td>";
         echo "<td><a href=\"?p=dispatcher&log=$results[1]\" style=\"text-decoration: underline;\">$results[0]</a></td></tr>";
       } elseif ( $results[2] == "pkgupdate" ) {
         $target=$results[3];
	 if ( $results[3] == "__system__" )
            $target="Local System";
       
         echo "<tr><td>$results[2]</td>";
         echo "<td>Update Packages</td>";
         echo "<td>$target</td>";
         echo "<td><a href=\"?p=dispatcher&log=$results[1]\" style=\"text-decoration: underline;\">$results[0]</a></td></tr>";
       } else {
         $target=$results[5];
	 if ( $results[5] == "__system__" )
            $target="Local System";
         echo "<tr><td>$results[4]</td>";
         echo "<td>$results[2] - $results[3]</td>";
         echo "<td>$target</td>";
         echo "<td><a href=\"?p=dispatcher&log=$results[1]\" style=\"text-decoration: underline;\">$results[0]</a></td></tr>";
       }
     }

   }
?>

</table>
