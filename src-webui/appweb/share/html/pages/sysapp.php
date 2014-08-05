<?
function parse_details($pbiorigin, $jail="") 
{
  if ( empty($jail) )
    $jail="#system";
  else
    $jail="#$jail";

  $sc="pbi app $pbiorigin";
  exec("/usr/local/bin/syscache ". escapeshellarg("$sc name") . " " . escapeshellarg("pkg $jail local $pbiorigin version") . " " . escapeshellarg("$sc author") . " " . escapeshellarg("$sc website") . " " . escapeshellarg("$sc comment") . " " . escapeshellarg("$sc confdir"), $pbiarray);

  $pbiname = $pbiarray[0];
  $pbiver = $pbiarray[1];
  $pbiauth = $pbiarray[2];
  $pbiweb = $pbiarray[3];
  $pbicomment = $pbiarray[4];
  $pbicdir = $pbiarray[5];

  if ( empty($pbiname) )
  {
    exec("/usr/local/bin/syscache " . escapeshellarg("pkg $jail local $pbiorigin name"), $pkgarray);
    $pbiname = $pkgarray[0];
  }

  // Get our values from this line
  print ("<tr>\n");
  print("  <td><a href=\"/?p=sysappinfo&app=$pbiorigin\"><img border=0 align=\"center\" height=48 width=48 src=\"images/pbiicon.php?i=$pbicdir/icon.png\"></a></td>\n");
  print("  <td><a href=\"/?p=sysappinfo&app=$pbiorigin\">$pbiname - $pbiver</a><br><a href=\"$pbiweb\" target=\"_new\" style=\"text-decoration: underline;\">$pbiauth</a></td>\n");
  print ("</tr>\n");
}

?>

<h1>Installed System Applications</h1>
<br>
<table class="jaillist" style="width:100%">
<tr>
   <th>Apps</th>
   <th>Details</th>
</tr>

<?

   $pkgoutput = syscache_ins_pkg_list();
   $pbioutput = syscache_pbidb_list();

   $pkglist = explode(", ", $pkgoutput[0]);
   $pbilist = explode(", ", $pbioutput[0]);

   // Now loop through pbi origins
   foreach ($pbilist as $pbiorigin)
     // Is this PBIs origin package installed?
     if ( array_search($pbiorigin, $pkglist) !== false)
       parse_details($pbiorigin);
?>

</table>
