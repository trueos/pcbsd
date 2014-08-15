<?
   // Check for endless scroll bits
   if ( empty($_GET['skip'])) {
?>

<div class="scrollapps">


<h1>Installed System Applications</h1>
<br>
<table class="jaillist" style="width:100%">
<tr>
   <th></th>
   <th></th>
</tr>
<? } else {
   require("../include/globals.php");
   require("../include/functions.php");
}

?>

<?
   if ( empty($_GET['skip']))
      $skip = 0;
   else
      $skip = $_GET['skip'];

   $skipstop = $skip + 50;

   $totalCols = 2;

   $pkgoutput = syscache_ins_pkg_list();
   $pbioutput = syscache_pbidb_list();

   $pkglist = explode(", ", $pkgoutput[0]);
   $pbilist = explode(", ", $pbioutput[0]);

   // Now loop through pbi origins
   $col=1;

   // Set the counter
   $curItem=0;
   $atEnd = true;

   foreach ($pbilist as $pbiorigin)
     // Is this PBIs origin package installed?
     if ( array_search($pbiorigin, $pkglist) !== false) {

       /* Disable the jscroll stuff for now 
       if ( $curItem < $skip ) {
         $curItem++;
         continue;
       }
       
       if ( $curItem >= $skipstop ) {
         $atEnd = false;
         break;
       }
       */

       parse_details($pbiorigin, "system", $col);
       if ( $col == $totalCols )
          $col = 1;
       else
         $col++;

       $curItem++;
     }

   echo "</tr>";
?>

<?
  // See if there is more data to load
  if ( ! $atEnd ) {
    echo "<a href=\"/pages/sysapp.php?skip=$skipstop\" class=\"jscroll-next\">next page</a>\n";
?>
<script type="text/javascript">
  $('.scrollapps').jscroll({
   nextSelector: 'a.jscroll-next:last',
  });
</script>
</table>
</div>
<? } ?>
