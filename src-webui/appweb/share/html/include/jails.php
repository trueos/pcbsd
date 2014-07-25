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
         print("  <td>$linearray[1]</td>\n");
         print("  <td>$linearray[2]</td>\n");
         print("  <td>$linearray[3]</td>\n");
         print ("</tr>\n");
      }

   }  // end subarray loop
?>

</table>
