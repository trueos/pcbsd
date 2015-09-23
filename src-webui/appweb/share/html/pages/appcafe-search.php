<?php
 defined('DS') OR die('No direct access allowed.');

$jail = "#system";
$jailUrl="__system__";

if ( ! empty($_GET['search']) )
    $header="Searching for: ". $_GET['search'];
if ( ! empty($_GET['searchtext']) ) {
    $searchtext = $_GET['searchtext'];
    $searchraw = $_GET['searchraw'];
 }
?>

<br>
<br>
<table class="header" style="width:100%">
<tr>
    <th>
        <h1><center>Package Search</h1>
    </th>
</tr>
</table>

<table class="login" style="width:100%";">
<tr>
    <th>
    </th>
</tr>
<tr>
    <th>
        <form action="/" method="get">
        <input type="hidden" name="p" value="appcafe-search">
        &nbsp&nbspSearch: <input name="searchtext" type="text" float: left; align="middle" value="<?php echo "$searchtext"; ?>" /> <input type="image" style="position:absolute;margin:0 0 0 4px;" width="28" height="28" src="images/search.png" align="middle" alt="Search" /><br>
    </th>
</tr>
<tr>
    <th>
        &nbsp&nbspSearch all available PBIs and packages: <input name="searchraw" type="checkbox" value="checked" <?php if ( $searchraw == "checked") { echo "checked"; } ?> /><br>
    </th>
</tr>

<h1><?php echo $header; ?></h1>
<br>
<br>

<?php

  if ( empty($searchtext) )
     return;

  if ( $deviceType == "computer" ) { 
    $totalCols = 3;
?>
<hr width="100%">
<table class="jaillist" style="width:100%">
<tr>
   <th></th>
   <th></th>
   <th></th>
</tr>
<?php
 } else {
   $totalCols = 2;
?>
<table class="jaillist" style="width:100%">
<tr>
   <th></th>
   <th></th>
</tr>
<?php
 }

 // Do the search
 if ( $searchraw == "checked" )
   $cmd = "pkg search '$searchtext' $jail 20";
 else
   $cmd = "pbi search '$searchtext' all 20";

 $sccmd = array("$cmd");
 $response = send_sc_query($sccmd);
 $pbilist = $response["$cmd"];
 $found=0;

 // Now loop through pbi origins
 $col=1;
 foreach ($pbilist as $pbiorigin) {
   if ( empty($pbiorigin) )
      continue;

   if ( $found > 60 )
      break;

   $found++;
   if ( parse_details($pbiorigin, $jail, $col, true, false) == 0 ) {
     if ( $col == $totalCols )
        $col = 1;
     else
        $col++;
   }

 }

 if ( $found == 0 )
 {
    if ( $searchraw == "checked" )
      echo "<tr><td colspan=3>No PBIs / Packages found!</td></tr>";
    else
      echo "<tr><td colspan=3>No PBIs found! Try searching for all available PBI / Packages.</td></tr>";
 } else {
   echo "</tr>";
 }

 echo "</table>";
?>
