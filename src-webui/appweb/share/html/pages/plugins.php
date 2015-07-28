<?php
defined('DS') OR die('No direct access allowed.');

 if ( ! empty($_GET['cat']) )
   $header="Browsing Category: ". $_GET['cat'];
 else
   $header="Recommended Applications";
?>

<h1><?php echo $header; ?></h1>
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
        exec("$sc ". escapeshellarg("pbi list allapps"), $pbiarray);
      elseif ( $jail == "#system" && $sysType == "DESKTOP" )
        exec("$sc ". escapeshellarg("pbi list graphicalapps"), $pbiarray);
      else
        exec("$sc ". escapeshellarg("pbi list serverapps"), $pbiarray);

      $fulllist = explode(", ", $pbiarray[0]);
      $catsearch = $_GET['cat'] . "/";
      $pbilist = array_filter($fulllist, function($var) use ($catsearch) { return preg_match("|^$catsearch|", $var); });

   } else {
      exec("$sc ". escapeshellarg("pbi list recommended")." ". escapeshellarg("pbi list new"), $pbiarray);
      $pbilist = explode(", ", $pbiarray[0]);
      $newlist = explode(", ", $pbiarray[1]);
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

