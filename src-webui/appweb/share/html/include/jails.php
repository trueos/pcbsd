<?
   if ( ! empty($_GET['toggle']) )
   {
	$tjail = $_GET['toggle'];
	$sjail = $_GET['status'];
	if ( $sjail == "Running" )
   	  run_cmd("warden stop $tjail");
 	else
   	  run_cmd("warden start $tjail");
	hideurl();
   }

   if ( ! empty($_GET['autostart']) )
   {
	$tjail = $_GET['autostart'];
   	run_cmd("warden auto $tjail");
	hideurl();
   }
?>


<h1>System Jail Listing</h1>
<br>
<table class="jaillist" style="width:100%">
<tr>
   <th>Jail Name</th>
   <th>Autostart</th>
   <th>Status</th>
   <th>Type</th>
</tr>

<?

   $jailoutput = run_cmd("warden list");
   foreach ($jailoutput as $line) {  // access subarray
      // Strip out the header information
      if ( strpos($line, "AUTOSTART") != false )
         continue;

      if ( strpos($line, '-') === false )
      {
         $line = $data = preg_replace('/[ ]{2,}|[\t]/', ' ', trim($line));
         // Get our values from this line
	 $linearray = explode( " ", $line);
         print ("<tr>\n");
         print("  <td>$linearray[0]</td>\n");
         print("  <td><a href=\"/?p=jails&autostart=$linearray[0]\" style=\"text-decoration: underline;\">$linearray[1]</a></td>\n");
         print("  <td><a href=\"/?p=jails&toggle=$linearray[0]&status=$linearray[2]\" style=\"text-decoration: underline;\">$linearray[2]</a></td>\n");
         print("  <td>$linearray[3]</td>\n");
         print ("</tr>\n");
      }

   }  // end subarray loop
?>

</table>
