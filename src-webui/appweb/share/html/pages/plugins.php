<?php
defined('DS') OR die('No direct access allowed.');

 if ( ! empty($_GET['cat']) )
   $header="Browsing Category: ". $_GET['cat'];
 else
   $header="Apps Browser";
?>

<h1><?php echo $header; ?></h1>
<br>
<?php
     $totalCols = 2;
?>
<table class="jaillist" style="width:100%">
<tr>
   <th style="width:50%"></th>
   <th style="width:50%"></th>
</tr>
<?php

   if ( ! empty($_GET['cat']) )
   {
      $sccmd = array("pbi list cages");
      $response = send_sc_query($sccmd);
      $fulllist = $response["pbi list cages"];
      $catsearch = $_GET['cat'] . "/";
      $pbilist = array_filter($fulllist, function($var) use ($catsearch) { return preg_match("|^$catsearch|", $var); });

   } else {
      $sccmd = array("pbi list cages");
      $response = send_sc_query($sccmd);
      $pbilist = $response["pbi list cages"];
      shuffle($pbilist);
      array_splice($pbilist, 16);

   }

   // Now loop through pbi origins
   $col=1;
   foreach ($pbilist as $cageorigin) {
     if ( parse_plugin_details($cageorigin, $col) == 0 ) {
       if ( $col == $totalCols )
          $col = 1;
       else
          $col++;
       }
   } 

 echo "</tr>";
 echo "</table>";
?>

