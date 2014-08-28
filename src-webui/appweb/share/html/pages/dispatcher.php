<?
   if ( ! empty($_GET['log']) )
   {
      $logoutput=run_cmd("log ". $_GET['log']);

?>

<h1>Action Log (<a href="?p=dispatcher">Back</a>)</h1>
<br>
<table class="jaillist" style="width:768px">
<tr>
   <th>Log output</th>
</tr>

<tr><td>
<?
   
   foreach ($logoutput as $line)
       echo "$line<br>";

   echo "</td></tr>";

   // No log file showing, show all logs
   } else {
?>
<h1>Recent application actions</h1>
<br>
<table class="jaillist" style="width:768px">
<tr>
   <th>Action</th>
   <th>Application</th>
   <th>Target</th>
   <th>Result</th>
</tr>

<?

     $rarray = run_cmd("results");

     // Loop through the results
     foreach ($rarray as $res) {
       $results = explode(" ", $res);
       if ( $results[2] == "pkgupdate" )
       {
         $target=$results[3];
	 if ( $results[3] == "__system__" )
            $target="Local System";
       
         echo "<tr><td>$results[2]</td>";
         echo "<td>Update Packages</td>";
         echo "<td>$target</td>";
         echo "<td><a href=\"?p=dispatcher&log=$results[1]\">$results[0]</a></td></tr>";
       } else {
         $target=$results[5];
	 if ( $results[5] == "__system__" )
            $target="Local System";
         echo "<tr><td>$results[4]</td>";
         echo "<td>$results[2] - $results[3]</td>";
         echo "<td>$results[5]</td>";
         echo "<td><a href=\"?p=dispatcher&log=$results[1]\">$results[0]</a></td></tr>";
       }
     }

   }
?>

</table>
