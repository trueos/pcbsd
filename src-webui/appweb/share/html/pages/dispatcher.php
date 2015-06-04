<?
defined('DS') OR die('No direct access allowed.');

   // Did the user request to start updates on a jail / system?
   if ( ! empty($_GET['updateTarget']) ) {
      // Time to queue up some updates for the requested target
      $updateTarget=$_GET['updateTarget'];
      if ( $updateTarget == "__system__")
        $updateTargetName="#system";
      else
        $updateTargetName="$updateTarget";

      // Queue it up now
      run_cmd("pkgupdate $updateTarget");

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
      $logoutput=run_cmd("log ". $_GET['log']);

?>

<h1>Action Log (<a href="?p=dispatcher">Back</a>)</h1>
<br>
<table class="jaillist" style="width:768px">
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
       exec("$sc ". escapeshellarg("pkg ". $jname . " hasupdates"), $jarray);
       $hasupdates=$jarray[0];

       if ( $hasupdates == "true" ) {
          $pkgUpdates=true;
          // Get the list of updates to show user
          unset($jarray);
          exec("$sc ". escapeshellarg("pkg ". $jname . " updatemessage"), $jarray);

          echo "<div class=\"popbox\">\n";
          echo "  <a href=\"/?p=dispatcher&updateTarget=$targetUrl\" style=\"text-decoration: underline;\"><img src=\"/images/warning.png\" height=35 width=35 title=\"Updates available!\">Update packages for $target</a>";
          echo " (<a class=\"open\" href=\"#\">details</a>)\n";
          echo "  <div class=\"collapse\">\n";
          echo "    <div class=\"box\">\n";
          echo "      <div class=\"arrow\"></div>\n";
          echo "      <div class=\"arrow-border\"></div>\n";
	  echo "        <p align=\"left\">\n";
	  echo "        $jarray[0]</p>\n";
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
       echo "<h1><img src=\"/images/noupdates.png\" height=24 width=24 title=\"Up to date!\">All packages and jails are up to date!</h1><br>";

?>
<h1>Recent application actions</h1>
<br>
<table class="jaillist" style="width:768px">
<tr>
   <th>Action</th>
   <th>Application</th>
   <th>Target</th>
   <th>Result</th>
</tr>

<?

     $rarray = run_cmd("results");

     // Loop through the results
     foreach ($rarray as $res) {
       $results = explode(" ", $res);
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
