<?php
defined('DS') OR die('No direct access allowed.');

 if ( ! empty($_GET['cat']) )
   $header="Browsing Category: ". $_GET['cat'];
 else
   $header="Plugin Browser";
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
      exec("$sc ". escapeshellarg("pbi list cages"), $pbiarray);

      $fulllist = explode(", ", $pbiarray[0]);
      $catsearch = $_GET['cat'] . "/";
      $pbilist = array_filter($fulllist, function($var) use ($catsearch) { return preg_match("|^$catsearch|", $var); });

   } else {
      exec("$sc ". escapeshellarg("pbi list cages"), $pbiarray);
      $pbilist = explode(", ", $pbiarray[0]);
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

