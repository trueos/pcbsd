<?
defined('DS') OR die('No direct access allowed.');
?>

<h1>Installed Plugins</h1>
<br>
<table class="jaillist" style="width:100%">
<tr>
   <th style="width: 50%"></th>
   <th style="width: 50%"></th>
</tr>
<?

   $skipstop = $skip + 50;

   $totalCols = 2;

   // Now loop through pbi origins
   $col=1;

   // Set the counter
   $curItem=0;
   $atEnd = true;

   $pbilist = syscache_ins_plugin_list();
   $parray = explode(", ", $pbilist);

   foreach ($parray as $pbiline) {
     if ( $pbiline == " " )
       continue;
     $pbiarray = explode(" ",$pbiline); 
     parse_plugin_details($pbiarray[0], $col, true, false);
     if ( $col == $totalCols )
       $col = 1;
     else
       $col++;

     $curItem++;
   }

   echo "</tr>";
?>

</table>
</div>
