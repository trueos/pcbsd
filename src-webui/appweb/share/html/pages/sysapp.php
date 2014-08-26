<?
   if ( empty($jail)) {
?>
<table class="jaillist" style="width:768px">
<tr>
   <th>View installed applications</th>
</tr>
<?
   echo "<tr><td><a href=\"/?p=syapp&jail=__system__\">Browse for System</a></td></tr>";
   $jailoutput = get_jail_list();

   $running=$jailoutput[0];
   $rarray = explode( ", ", $running);

   foreach ($rarray as $jname) {
     if ( empty($jname) )
        continue;
     echo "<tr><td><a href=\"/?p=sysapp&jail=$jname\">Browse jail: $jname</a></td></tr>";
   }


?>
</table>

<?

   } else {
     echo "<h1>Installed Applications in $jail</h1>";
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
   $pbioutput = syscache_pbidb_list();

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

       parse_details($pbiorigin, "$jail", $col, true);
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
