<?
   if ( empty($_GET['app']) )
      die("Missing app=");

   $pbi = $_GET['app'];

   if ( ! empty($_GET['delete']) )
   {
        print "<h2>DELETED -$pbi<br></h2>";
	hideurl("?p=sysappinfo&app=$pbi");
   }

   // Load the PBI details page
   $pbioutput = run_cmd("pbi info $pbi");
   
   foreach ($pbioutput as $line) {  // access subarray
      // Strip out the header information
      if ( empty($line) )
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
      if ( strpos($line, "Arch: ") === 0 ) {
	 $pbiarch = substr($line, strpos($line, " ") + 1);
	 continue;
      }
      if ( strpos($line, "Description: ") === 0 ) {
	 $pbidesc = substr($line, strpos($line, " ") + 1);
	 continue;
      }
      if ( strpos($line, "Module: ") === 0 ) {
	 $pbimod = substr($line, strpos($line, " ") + 1);
	 continue;
      }
   }  // end of loop parsing raw output

   if ( empty($pbiname) )
      die("No such app installed: $pbi");
?>
   
<script>
function delConfirm() {
    if (confirm("Are you sure you want to delete this application?") == true) {
	window.location.href = "<? echo "?p=sysappinfo&app=$pbi&delete=true"; ?>";
    }
}
</script>

<br>
<table class="jaillist" style="width:100%">
  <tr>
    <th colspan=2><? echo "$pbiname - $pbiver"; ?></th>
  </tr>
  <tr>
    <td align=center>
      <img align="center" height=64 width=64 src="images/pbiicon.php?i=<? echo "$pbimod"; ?>/icon.png"><br><br>
      <button onclick="delConfirm()">Delete App</button>
    </td>
    <td>
       <a href="<? echo "$pbiweb"; ?>" target="_new"><? echo "$pbiauth"; ?></a><br>
       Version: <b><? echo "$pbiver"; ?></b><br>
       ArchType: <b><? echo "$pbiarch"; ?></b><br>
    </td>
  </tr>
  <tr>
    <td colspan="2">
      <? echo "$pbidesc"; ?>
    </td>
  </tr>
</table>
