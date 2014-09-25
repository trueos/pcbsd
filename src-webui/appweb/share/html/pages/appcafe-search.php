<?
defined('DS') OR die('No direct access allowed.');

 if ( empty($_GET['jail']) or ! empty($_GET['changeappcafejail']))
 {
    display_jail_appcafeselection();
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
<form method="post" action="?p=appcafe-search">
Search: <input name="searchtext" type="text" value="<? echo "$searchtext"; ?>" /> <input name="submit" type="submit" value="Search"/><br>
Search raw packages: <input name="searchraw" type="checkbox" value="checked" <? if ( $searchraw == "checked") { echo "checked"; } ?> /><br>
</form>

<?

  if ( empty($searchtext) )
     return;

  if ( $deviceType == "computer" ) { 
    $totalCols = 3;
?>
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
?>

