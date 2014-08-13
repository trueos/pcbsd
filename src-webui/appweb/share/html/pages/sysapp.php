<h1>Installed System Applications</h1>
<br>
<?
  if ( $deviceType == "computer" ) { 
    $totalCols = 3;
?>
<table class="jaillist" style="width:768px">
<tr>
   <th></th>
   <th></th>
   <th></th>
</tr>
<?
  } else {
    $totalCols = 2;
?>
<table class="jaillist" style="width:100%">
<tr>
   <th></th>
   <th></th>
</tr>
<?
  }
?>

<?

   $pkgoutput = syscache_ins_pkg_list();
   $pbioutput = syscache_pbidb_list();

   $pkglist = explode(", ", $pkgoutput[0]);
   $pbilist = explode(", ", $pbioutput[0]);

   // Now loop through pbi origins
   $col=1;
   foreach ($pbilist as $pbiorigin)
     // Is this PBIs origin package installed?
     if ( array_search($pbiorigin, $pkglist) !== false) {
       parse_details($pbiorigin, "system", $col);
       if ( $col == $totalCols )
          $col = 1;
       else
         $col++;
     }

   // Close off the <tr>
   if ( $col != $totalCols )
      echo "</tr>";
?>

</table>
