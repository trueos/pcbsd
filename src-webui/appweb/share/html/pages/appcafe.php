<?

if ( ! empty($_GET['cat']) )
  $header="Browsing Category: ". $_GET['cat'];
else
  $header="Recommended Applications";

?>

<h1><? echo $header; ?></h1>
<br>
<?

  if ( $deviceType == "computer" ) { 
    $totalCols = 4;
?>
<table class="jaillist" style="width:768px">
<tr>
   <th></th>
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

<?

   if ( empty($_GET['jail']))
      $jail="#system";
   else
      $jail=$_GET['jail'];

   if ( ! empty($_GET['cat']) )
   {
     exec("$sc ". escapeshellarg("pbi list apps"), $pbiarray);
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
     parse_details($pbiorigin, "$jail", $col);
     if ( $col == $totalCols )
        $col = 1;
     else
       $col++;
   }

   // Close off the <tr>
   if ( $col != $totalCols )
      echo "</tr>";
?>

</table>
