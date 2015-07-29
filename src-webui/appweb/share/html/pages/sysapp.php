<?
defined('DS') OR die('No direct access allowed.');
?>
<table class="header" style="width:100%">
<tr>
    <th>
        <h1><center>Installed Applications</h1>
    </th>
</tr>
<br>
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
</div>
