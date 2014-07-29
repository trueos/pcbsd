<h1>Installed System Applications</h1>
<br>
<table class="jaillist" style="width:100%">
<tr>
   <th>Apps</th>
   <th>Details</th>
</tr>

<?

   $pbioutput = run_cmd("pbi info -v");
   foreach ($pbioutput as $line) {  // access subarray
      // Strip out the header information
      if ( empty($line) )
         continue;
      if ( strpos($line, "PBI Information for:") != false )
         continue;
      if ( strpos($line, "PBIs installed") != false )
         continue;
      if ( strpos($line, "Arch: ") != false )
         continue;


      // Look for Name:
      if ( strpos($line, "Name: ") === 0 ) {
	 $pbiname = substr($line, strpos($line, " ") + 1);
	 continue;
      }
      if ( strpos($line, "Version: ") === 0 ) {
	 $pbiver = substr($line, strpos($line, " ") + 1);
	 continue;
      }
      if ( strpos($line, "Author: ") === 0 ) {
	 $pbiauth = substr($line, strpos($line, " ") + 1);
	 continue;
      }
      if ( strpos($line, "Website: ") === 0 ) {
	 $pbiweb = substr($line, strpos($line, " ") + 1);
	 continue;
      }
      if ( strpos($line, "Description: ") === 0 ) {
	 $pbidesc = substr($line, strpos($line, " ") + 1);
	 continue;
      }
      if ( strpos($line, "Module: ") === 0 ) {
	 $pbimod = substr($line, strpos($line, " ") + 1);


         // Get our values from this line
         print ("<tr>\n");
         print("  <td><a href=\"/?p=sysappinfo&app=$pbiname\"><img border=0 align=\"center\" height=48 width=48 src=\"images/pbiicon.php?i=$pbimod/icon.png\"></a></td>\n");
         print("  <td><a href=\"/?p=sysappinfo&app=$pbiname\">$pbiname - $pbiver</a><br><a href=\"$pbiweb\" target=\"_new\" style=\"text-decoration: underline;\">$pbiauth</a></td>\n");
         print ("</tr>\n");

	 $pbiname = "";
	 $pbiver = "";
	 $pbiauth = "";
	 $pbiweb = "";
	 $pbidesc = "";
	 $pbimodule = "";
	 continue;
      }
   }  // end of loop parsing raw output

?>

</table>
