<?
   if ( ! empty($_GET['jail'])) {
     $jail = $_GET['jail'];
     echo "<h1>$jail : Installed Applications</h1>";
   } else {
     echo "<h1>Installed System Applications</h1>";
   }
?>

<br>
<table class="jaillist" style="width:100%">
<tr>
   <th></th>
   <th></th>
</tr>
<?
   if ( ! empty($_GET['jail']))
     $jail = $_GET['jail'];

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
</div>
