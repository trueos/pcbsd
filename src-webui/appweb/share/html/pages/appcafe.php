<?

if ( ! empty($_GET['cat']) )
  $header="Browsing Category: ". $_GET['cat'];
else
  $header="Recommended Applications";


function parse_details($pbiorigin, $jail, $col) 
{
  global $sc;
  global $totalCols;
  global $inslist;

  if ( empty($jail) )
    $jail="#system";
  else
    $jail="#" . $jail;

  $cmd="pbi app $pbiorigin";
  exec("$sc ". escapeshellarg("$cmd name") . " " . escapeshellarg("$cmd comment") . " " . escapeshellarg("$cmd confdir"), $pbiarray);

  $pbiname = $pbiarray[0];
  $pbicomment = $pbiarray[1];
  $pbicdir = $pbiarray[2];

  if ( empty($pbiname) )
  {
    exec("$sc " . escapeshellarg("pkg $jail remote $pbiorigin name"), $pkgarray);
    $pbiname = $pkgarray[0];
  }

  if ( $col == 1 )
    print ("<tr>\n");

  // Get our values from this line
  print("  <td>");
  if ( array_search($pbiorigin, $inslist) !== false)
    print("    <button title=\"Delete this application\" style=\"float:right;\" onclick=\"delConfirm('" . $pbiname ."','".$pbiorigin."','pbi','system')\">X</button>");
  else
    print("    <button title=\"Install this application\" style=\"float:right;\" onclick=\"addConfirm('" . $pbiname ."','".$pbiorigin."','pbi','system')\">+</button>");

  print("    <a href=\"/?p=appinfo&app=$pbiorigin\" title=\"$pbicomment\"><img border=0 align=\"center\" height=48 width=48 src=\"images/pbiicon.php?i=$pbicdir/icon.png\" style=\"float:left;\"></a>\n");
  print("    <a href=\"/?p=appinfo&app=$pbiorigin\">$pbiname</a><br>\n");

  print("\n  </td>");

  if ( $col == $totalCols )
    print ("</tr>\n");
}

?>

<h1><? echo $header; ?></h1>
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

   if ( ! empty($_GET['cat']) )
   {
     exec("$sc ". escapeshellarg("pbi list apps"), $pbiarray);
     $fulllist = explode(", ", $pbiarray[0]);
     $catsearch = $_GET['cat'] . "/";
     $pbilist = array_filter($fulllist, function($var) use ($catsearch) { return preg_match("|^$catsearch|", $var); });

   } else {
     exec("$sc ". escapeshellarg("pbi list recommended")." ". escapeshellarg("pbi list new"), $pbiarray);
     $pbilist = explode(", ", $pbiarray[0]);
     $newlist = explode(", ", $pbiarray[1]);
   }

   exec("$sc ". escapeshellarg("pkg #system installedlist"), $insarray);
   
   $inslist = explode(", ", $insarray[0]);

   // Now loop through pbi origins
   $col=1;
   foreach ($pbilist as $pbiorigin) {
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
