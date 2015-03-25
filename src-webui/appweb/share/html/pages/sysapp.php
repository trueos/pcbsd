<?
defined('DS') OR die('No direct access allowed.');

   if ( empty($jail)) {
?>
<table class="jaillist" style="width:768px">
<tr>
   <th>View installed applications</th>
</tr>
<?
   if ( $sysType != "APPLIANCE" )
     echo "<tr><td><a href=\"/?p=sysapp&jail=__system__\"><img src=\"/images/system.png\" height=32 width=32> Local System</a></td></tr>";
   $jailoutput = get_jail_list();

   $running=$jailoutput[0];
   $rarray = explode( ", ", $running);

   foreach ($rarray as $jname) {
     if ( empty($jname) )
        continue;

     unset($jarray);
     exec("$sc ". escapeshellarg("jail ". $jname . " ipv4"), $jarray);
     $jipv4=$jarray[0];

     echo "<tr><td><a href=\"/?p=sysapp&jail=$jname\"><img src=\"/images/jail.png\" height=32 width=32> $jname - $jipv4</a></td></tr>";
   }


?>
</table>

<?

   } else {
     echo "<h1>Installed Applications in $jail (";
     echo "<a href=\"/?p=exportpbis&jail=__system__\" style=\"text-decoration: underline;\">Export PBI list</a>)</h1>";
?>

<br>
<table class="jaillist" style="width:100%">
<tr>
   <th></th>
   <th></th>
</tr>
<?

   $skipstop = $skip + 50;

   $totalCols = 2;

   $pkgoutput = syscache_ins_pkg_list($jail);

   if ( $jail == "#system" )
     $pbioutput = syscache_pbidb_list();
   else
     $pbioutput = syscache_pbidb_list("serverapps");

   $pkglist = explode(", ", $pkgoutput[0]);
   $pbilist = explode(", ", $pbioutput[0]);

   // Now loop through pbi origins
   $col=1;

   // Set the counter
   $curItem=0;
   $atEnd = true;

   foreach ($pbilist as $pbiorigin)
     // Is this PBIs origin package installed?
     if ( array_search($pbiorigin, $pkglist) !== false) {
       // Is this PBI just a DEP for another app? If so, skip it
       $output="";
       exec("/usr/local/bin/syscache ".escapeshellarg("pkg $jail local $pbiorigin rdependencies"), $output);
       if ( "$output[0]" != "$SCERROR" )
          continue;

       parse_details($pbiorigin, "$jail", $col, true, false);
       if ( $col == $totalCols )
          $col = 1;
       else
         $col++;

       $curItem++;
     }

   echo "</tr>";
?>

</table>
<?
   }
?>
</div>
