<?
   if ( empty($_GET['app']) )
      die("Missing app=");

   $pbiorigin = $_GET['app'];

   if ( ! empty($_GET['delete']) )
   {
        print "<h2>DELETED -$pbiorigin<br></h2>";
	hideurl("?p=sysappinfo&app=$pbiorigin");
   }

   // Load the PBI details page
   $sc="pbi app $pbiorigin";
   exec("/usr/local/bin/syscache ". escapeshellarg("$sc name") . " " . escapeshellarg("pkg #system local $pbiorigin version") . " " . escapeshellarg("$sc author") . " " . escapeshellarg("$sc website") . " " . escapeshellarg("$sc comment") . " " . escapeshellarg("$sc confdir") . " " . escapeshellarg("$sc description") . " " . escapeshellarg("$sc screenshots"), $pbiarray);

  $pbiname = $pbiarray[0];
  $pbiver = $pbiarray[1];
  $pbiauth = $pbiarray[2];
  $pbiweb = $pbiarray[3];
  $pbicomment = $pbiarray[4];
  $pbicdir = $pbiarray[5];
  $pbidesc = $pbiarray[6];
  $pbiss = $pbiarray[7];

  if ( empty($pbiname) )
  {
    exec("/usr/local/bin/syscache " . escapeshellarg("pkg #system local $pbiorigin name"), $pkgarray);
    $pbiname = $pkgarray[0];
  }

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
      <img align="center" height=64 width=64 src="images/pbiicon.php?i=<? echo "$pbicdir"; ?>/icon.png"><br><br>
      <button onclick="delConfirm()">Delete App</button>
    </td>
    <td>
       <a href="<? echo "$pbiweb"; ?>" target="_new"><? echo "$pbiauth"; ?></a><br>
       Version: <b><? echo "$pbiver"; ?></b><br>
    </td>
  </tr>
  <tr>
    <td colspan="2" width=400>
      <? echo "$pbidesc"; ?>
    </td>
  </tr>
  <tr>
    <td colspan="2">
      <?  // Do we have screenshots to display?
	 if ( ! empty($pbiss) ) {
	    echo "Screenshots: <br>";
            $sslist = explode(" ", $pbiss);
            foreach($sslist as $screenshot)
            {
              echo "<a href=\"$screenshot\" target=\"_new\"><img border=0 src=\"$screenshot\" height=50 width=50></a>&nbsp;";
            }
         }
      ?>
    </td>
  </tr>
</table>
