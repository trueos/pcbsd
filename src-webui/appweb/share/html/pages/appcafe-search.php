<?
 defined('DS') OR die('No direct access allowed.');

 if ( empty($_GET['jail']) or ! empty($_GET['changeappcafejail']))
 {
    display_jail_appcafeselection("appcafe-search");
    return;
 }

 if ( ! empty($_POST['search']) )
   $header="Searching for: ". $_POST['search'];
 else
   $header="Package Search";

 if ( ! empty($_POST['searchtext']) ) {
    $searchtext = $_POST['searchtext'];
    $searchraw = $_POST['searchraw'];
 }
?>

<h1><? echo $header; ?></h1>
<br>
<form method="post" action="?p=appcafe-search&jail=<? echo "$jailUrl"; ?>">
Search: <input name="searchtext" type="text" value="<? echo "$searchtext"; ?>" /> <input type="image" style="float: relative;" width="28" height="28" src="images/search.png" alt="Search" /><br>
Search raw packages: <input name="searchraw" type="checkbox" value="checked" <? if ( $searchraw == "checked") { echo "checked"; } ?> /><br>
</form>

<?

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
<?
 } else {
   $totalCols = 2;
?>
<table class="jaillist" style="width:100%">
<tr>
   <th></th>
   <th></th>
</tr>
<?
 }

 // Do the search
 if ( $searchraw == "checked" )
   $cmd = "pkg search $searchtext $jail 20";
 else
   $cmd = "pbi search $searchtext all 20";

 exec("$sc ". escapeshellarg("$cmd"), $pbiarray);
 $pbilist = explode(", ", $pbiarray[0]);

 // Now loop through pbi origins
 $col=1;
 foreach ($pbilist as $pbiorigin) {
   if ( parse_details($pbiorigin, $jail, $col, true, false) == 0 ) {
     if ( $col == $totalCols )
        $col = 1;
     else
        $col++;
   }

 }

?>
