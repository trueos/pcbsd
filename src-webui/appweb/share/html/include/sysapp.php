<?
function parse_details($pbiorigin) 
{
  $sc="pbi app $pbiorigin";
  exec("/usr/local/bin/syscache ". escapeshellarg("$sc name") . " " . escapeshellarg("$sc version") . " " . escapeshellarg("$sc author") . " " . escapeshellarg("$sc website") . " " . escapeshellarg("$sc comment"), $pbiarray);

  $pbiname = substr($pbiarray[0], 11);
  $pbiver = $pbiarray[1];
  $pbiauth = $pbiarray[2];
  $pbiweb = $pbiarray[3];
  $pbicomment = $pbiarray[4];

  // Get our values from this line
  print ("<tr>\n");
  print("  <td><a href=\"/?p=sysappinfo&app=$pbiname\"><img border=0 align=\"center\" height=48 width=48 src=\"images/pbiicon.php?i=$pbiorigin/icon.png\"></a></td>\n");
  print("  <td><a href=\"/?p=sysappinfo&app=$pbiname\">$pbiname - $pbiver</a><br><a href=\"$pbiweb\" target=\"_new\" style=\"text-decoration: underline;\">$pbiauth</a></td>\n");
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

   $pbioutput = syscache_ins_pbi_list();

   foreach ($pbioutput as $line) {  // access subarray
      // Strip out the header information
      if ( empty($line) )
         continue;
      if ( strpos($line, "[INFOSTART]") === false )
         continue;

      $line = substr($line, 11);
      break;
   }

   $pbilist = explode(", ", $line);

   // Got a list of origins, now get details
   foreach ($pbilist as $pbiorigin)
     parse_details($pbiorigin);

?>

</table>
