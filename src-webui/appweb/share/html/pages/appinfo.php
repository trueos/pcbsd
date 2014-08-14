<?

function display_app_link($pbilist)
{
  $rlist = explode(" ", $pbilist);
  $totalCols = 2;
  $col = 1;
  echo " <table class=\"jaillist\" style=\"width:100%\">";
  echo "  <tr>\n";
  echo "   <th></th>\n";
  echo "   <th></th>\n";
  echo "  </tr>";

  foreach($rlist as $related) {
    parse_details($related, "system", $col);
    if ( $col == $totalCols )
       $col = 1;
    else
       $col++;
    }

    // Close off the <tr>
    if ( $col == $totalCols )
       echo "  </tr>\n";

    echo "</table>\n";
}

   if ( empty($_GET['app']) )
      die("Missing app=");

   // Get the current work queue status of the dispatcher
   $dStatus = getDispatcherStatus();

   $pbiorigin = $_GET['app'];

   // Load the PBI details page
   $cmd="pbi app $pbiorigin";
   exec("$sc ". escapeshellarg("$cmd name") . " " . escapeshellarg("pkg #system local $pbiorigin version") . " " . escapeshellarg("$cmd author") . " " . escapeshellarg("$cmd website") . " " . escapeshellarg("$cmd comment") . " " . escapeshellarg("$cmd confdir") . " " . escapeshellarg("$cmd description") . " " . escapeshellarg("$cmd screenshots"), $pbiarray);

  $pbiname = $pbiarray[0];
  $pbiver = $pbiarray[1];
  $pbiauth = $pbiarray[2];
  $pbiweb = $pbiarray[3];
  $pbicomment = $pbiarray[4];
  $pbicdir = $pbiarray[5];
  $pbidesc = $pbiarray[6];
  $pbiss = $pbiarray[7];

  // Get second tier data
  $cmd="pbi app $pbiorigin";
  unset($pbiarray);
  exec("$sc ". escapeshellarg("$cmd license") . " " . escapeshellarg("$cmd type") . " " . escapeshellarg("$cmd tags") . " " . escapeshellarg("$cmd relatedapps") . " " . escapeshellarg("$cmd plugins") . " " . escapeshellarg("$cmd options") . " " . escapeshellarg("$cmd rating"), $pbiarray);
  $pbilicense = $pbiarray[0];
  $pbitype = $pbiarray[1];
  $pbitags = $pbiarray[2];
  $pbirelated = $pbiarray[3];
  $pbiplugins = $pbiarray[4];
  $pbioptions = $pbiarray[5];
  $pbirating = $pbiarray[6];

  if ( empty($pbiname) )
  {
    exec("$sc " . escapeshellarg("pkg #system local $pbiorigin name"), $pkgarray);
    $pbiname = $pkgarray[0];
  }

  if ( empty($pbiname) )
    die("No such app: $pbi");

  // Check if this app is installed
  $pkgoutput = syscache_ins_pkg_list();
  $pkglist = explode(", ", $pkgoutput[0]);
  if ( array_search($pbiorigin, $pkglist) !== false)
     $pbiinstalled=true;
  else
     $pbiinstalled=false;

  // If the application is not installed, we need to fetch some stuff from rquery
  if ( ! $pbiinstalled )
  {
    exec("$sc " . escapeshellarg("pkg #system remote $pbiorigin version"), $pkgarray);
    $pbiver = $pkgarray[0];
  }

?>
   
<br>
<table class="jaillist" style="width:420px">
  <tr>
    <th colspan=2><? echo "$pbiname - $pbiver"; ?></th>
  </tr>
  <tr>
    <td align=left colspan=2>
      <img align="left" height=64 width=64 src="images/pbiicon.php?i=<? echo "$pbicdir"; ?>/icon.png">
       <a href="<? echo "$pbiweb"; ?>" target="_new"><? echo "$pbiauth"; ?></a><br>
       Version: <b><? echo "$pbiver"; ?></b><br>
      <?
	 if ( array_search("pbi $pbiorigin install system", $dStatus) !== false ) {
	   print("    Installing...");
         } else if ( array_search("pbi $pbiorigin delete system", $dStatus) !== false ) {
	   print("    Deleting...");
         } else if( $pbiinstalled ) {
	    print("    <button onclick=\"delConfirm('" . $pbiname ."','".$pbiorigin."','pbi','system')\">-Remove</button>");
         } else {
	    print("    <button onclick=\"addConfirm('" . $pbiname ."','".$pbiorigin."','pbi','system')\">+Install</button>");
	 }
      ?>
    </td>
  </tr>
  <tr>
    <td colspan="2">
       <p><? echo $pbidesc; ?></p>
    </td>
  </tr>
  <tr>
    <td colspan="2">
<div id="tab-container" class='tab-container'>
   <ul class='etabs'>
     <?  if ( ! empty($pbiss) ) { ?>
     <li class='tab'><a href="#tabs-screenshots">Screenshots</a></li>
     <? } ?>
     <?  if ( ! empty($pbirelated) ) { ?>
     <li class='tab'><a href="#tabs-related">Related</a></li>
     <? } ?>
     <?  if ( ! empty($pbiplugins) ) { ?>
     <li class='tab'><a href="#tabs-plugins">Plugins</a></li>
     <? } ?>
     <?  if ( ! empty($pbioptions) ) { ?>
     <li class='tab'><a href="#tabs-options">Options</a></li>
     <? } ?>
   </ul>
   <div class="panel-container">
     <?  // Do we have screenshots to display?
         if ( ! empty($pbiss) ) {
            echo "<div id=\"tabs-screenshots\">\n";
            $sslist = explode(" ", $pbiss);
            foreach($sslist as $screenshot)
              echo "<a href=\"$screenshot\" target=\"_new\"><img border=0 src=\"$screenshot\" height=50 width=50></a>&nbsp;";
	    echo "</div>\n";
         }

	 // Do we have related items to show?
         if ( ! empty($pbirelated) ) {
            echo "<div id=\"tabs-related\">\n";
	    display_app_link($pbirelated);
	    echo "</div>\n";
         }

	 // Do we have plugins to show?
         if ( ! empty($pbiplugins) ) {
            echo "<div id=\"tabs-plugins\">\n";
	    display_app_link($pbiplugins);
	    echo "</div>\n";
         }

	 // Do we have options to show?
         if ( ! empty($pbioptions) ) {
            echo "<div id=\"tabs-options\">\n";
            $olist = explode(" ", $pbioptions);
            foreach($olist as $option)
              echo "  <b>$option</b><br>\n";
	    echo "</div>\n";
         }
     ?>
   </div>
</div>
    </td>
  </tr>
</table>

<script type="text/javascript">
  $('#tab-container').easytabs();
</script>
