<?
defined('DS') OR die('No direct access allowed.');

 if ( empty($_GET['jail']) or ! empty($_GET['changeappcafejail']))
 {
    display_jail_appcafeselection();
 } else {

   if ( ! empty($_GET['search']) )
     $header="Searching for: ". $_GET['search'];
   else
     $header="Package Search";
?>

<h1><? echo $header; ?></h1>
<br>
<?

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
