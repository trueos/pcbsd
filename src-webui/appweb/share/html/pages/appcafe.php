<?php
defined('DS') OR die('No direct access allowed.');
$jail = "#system";
$jailUrl="__system__";


 if ( ! empty($_GET['cat']) )
   $header="Browsing Category: ". $_GET['cat'];
 else
   if ( $allPBI == "true" )
     $header="All Applications";
   else
     $header="Recommended Applications";
?>
<br>
<table class="header" style="width:100%">
<tr>
    <th>
        <h1><center><?php echo $header; ?></h1>
    </th>
</tr>
<br>
<?php

  if ( $deviceType == "computer" ) { 
  $totalCols = 3;
?>
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


   if ( ! empty($_GET['cat']) )
   {
      if ( $allPBI == "true" )
        $cmd = "pbi list allapps";
      elseif ( $jail == "#system" && $sysType == "DESKTOP" )
        $cmd = "pbi list graphicalapps";
      else
        $cmd = "pbi list serverapps";

      $sccmd = array("$cmd");
      $response = send_sc_query($sccmd);
      $pbiarray = $response["$cmd"];


      $fulllist = $pbiarray;
      $catsearch = $_GET['cat'] . "/";
      $pbilist = array_filter($fulllist, function($var) use ($catsearch) { return preg_match("|^$catsearch|", $var); });

   } else {
      $sccmd = array("pbi list recommended", "pbi list new");
      $response = send_sc_query($sccmd);
      $pbilist = $response["pbi list recommended"];
      $newlist = $response["pbi list new"];
      $pbilist = array_merge($pbilist, $newlist);
      //array_splice($pbilist, 16);
   }

   // Now loop through pbi origins
   $col=1;
   foreach ($pbilist as $pbiorigin) {
   if ( parse_details($pbiorigin, $jail, $col) == 0 ) {
     if ( $col == $totalCols )
        $col = 1;
     else
        $col++;
   }
 } 

 echo "</tr>";
 echo "</table>";
?>

