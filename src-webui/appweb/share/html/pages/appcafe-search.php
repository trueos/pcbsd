<?php
 defined('DS') OR die('No direct access allowed.');

 if ( empty($_GET['jail']) or ! empty($_GET['changeappcafejail']))
 {
    display_jail_appcafeselection("appcafe-search");
    return;
 }

 if ( ! empty($_GET['search']) )
   $header="Searching for: ". $_GET['search'];
 else
   $header="Package Search";

 if ( ! empty($_GET['searchtext']) ) {
    $searchtext = $_GET['searchtext'];
    $searchraw = $_GET['searchraw'];
 }
?>

<h1><?php echo $header; ?></h1>
<br>
<form action="/" method="get">
<input type="hidden" name="p" value="appcafe-search">
<input type="hidden" name="jail" value="<?php echo "$jailUrl"; ?>">
Search: <input name="searchtext" type="text" float: left; align="middle" value="<?php echo "$searchtext"; ?>" /> <input type="image" style="position:absolute;margin:0 0 0 4px;" width="28" height="28" src="images/search.png" align="middle" alt="Search" /><br>
<br>
Search all available PBIs and packages: <input name="searchraw" type="checkbox" value="checked" <?php if ( $searchraw == "checked") { echo "checked"; } ?> /><br>
</form>

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

 exec("$sc ". escapeshellarg("$cmd"), $pbiarray);
 $pbilist = explode(", ", $pbiarray[0]);
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
