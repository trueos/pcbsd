<?
function parse_details($pbiorigin, $jail, $col) 
{
  global $sc;
  global $totalCols;

  if ( empty($jail) )
    $jail="#system";
  else
    $jail="#" . $jail;

  $cmd="pbi app $pbiorigin";
  exec("$sc ". escapeshellarg("$cmd name") . " " . escapeshellarg("pkg $jail local $pbiorigin version") . " " . escapeshellarg("$cmd comment") . " " . escapeshellarg("$cmd confdir"), $pbiarray);

  $pbiname = $pbiarray[0];
  $pbiver = $pbiarray[1];
  $pbicomment = $pbiarray[2];
  $pbicdir = $pbiarray[3];

  if ( empty($pbiname) )
  {
    exec("$sc " . escapeshellarg("pkg $jail local $pbiorigin name"), $pkgarray);
    $pbiname = $pkgarray[0];
  }

  if ( $col == 1 )
    print ("<tr>\n");

  // Get our values from this line
  print("  <td>");
  print("    <a href=\"/?p=appinfo&app=$pbiorigin\" title=\"$pbicomment\"><img border=0 align=\"center\" height=48 width=48 src=\"images/pbiicon.php?i=$pbicdir/icon.png\" style=\"float:left;\"></a>\n");
  print("    <a href=\"/?p=appinfo&app=$pbiorigin\">$pbiname</a><br>$pbiver<br>\n");
  print("  </td>");

  if ( $col == $totalCols )
    print ("</tr>\n");
}

?>

<h1>Installed System Applications</h1>
<br>
<?
  if ( $deviceType == "computer" ) { 
    $totalCols = 4;
?>
<table class="jaillist" style="width:768px">
<tr>
   <th></th>
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
