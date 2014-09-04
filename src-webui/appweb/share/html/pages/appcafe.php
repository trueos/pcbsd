<?
defined('DS') OR die('No direct access allowed.');

function display_jail_appcafeselection()
{
  global $sc;
?>
<table class="jaillist" style="width:100%">
<tr>
   <th>AppCafe Store selection</th>
</tr>
<?
   echo "<tr><td><a href=\"/?p=appcafe&jail=__system__\"><img src=\"/images/system.png\" height=32 width=32> Local System</a></td></tr>";
   $jailoutput = get_jail_list();

   $running=$jailoutput[0];
   $rarray = explode( ", ", $running);

   foreach ($rarray as $jname) {
     if ( empty($jname) )
        continue;

     unset($jarray);
     exec("$sc ". escapeshellarg("jail ". $jname . " ipv4"), $jarray);
     $jipv4=$jarray[0];

     echo "<tr><td><a href=\"/?p=appcafe&jail=$jname\"><img src=\"/images/jail.png\" height=32 width=32> $jname - $jipv4</a></td></tr>";
   }


?>
</table>
<?

} // End of display_jail_appcafeselection

   if ( empty($_GET['jail']) or ! empty($_GET['changeappcafejail']))
   {
      display_jail_appcafeselection();
   } else {

     if ( ! empty($_GET['cat']) )
       $header="Browsing Category: ". $_GET['cat'];
     else
       $header="Recommended Applications";
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


     if ( ! empty($_GET['cat']) )
     {
       if ( $jail == "#system" )
         exec("$sc ". escapeshellarg("pbi list allapps"), $pbiarray);
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

     // Close off the <tr>
     if ( $col != $totalCols )
        echo "</tr>";

     echo "</table>";
   }
?>

