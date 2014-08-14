<?

// Set the error string syscache returns if a particular request
// isn't available
$SCERROR="[ERROR] Information not available";

function hideurl($newurl = "")
{
   if (empty($newurl) )
     $p = "/?p=" . $_GET['p'];
   else
     $p = "$newurl";
   ?>
   <script>
	window.history.pushState('AppWeb', 'AppWeb', '<? echo $p; ?>');
   </script>
   <?
}

// Runs commands through the sudo dispatcher
function run_cmd($cmd)
{
   exec("/usr/local/bin/sudo /usr/local/share/appweb/dispatcher $cmd", $output);
   return $output;
}

function syscache_ins_pkg_list($jail="")
{
   if ( empty($jail) )
      $jail = "#system";
   else
      $jail = "#$jail";

   exec("/usr/local/bin/syscache ".escapeshellarg("pkg #system installedlist"), $output);
   return $output;
}

function syscache_pbidb_list()
{
   exec("/usr/local/bin/syscache ".escapeshellarg("pbi list apps"), $output);
   return $output;
}

function queueInstallApp()
{
   $app = $_GET['installApp'];
   $type = $_GET['installAppCmd'];
   $target = $_GET['installAppTarget'];
   if ( ! empty($app) and ! empty($type) and ! empty($target) )
      run_cmd("queue $type $app install $target");

   // Now we can remove those values from the URL
   $newUrl=http_build_query($_GET);
   $app=str_replace("/", "%2F", $app);
   $newUrl=str_replace("&installApp=$app", "", $newUrl);
   $newUrl=str_replace("installApp=$app", "", $newUrl);
   $newUrl=str_replace("&installAppCmd=$type", "", $newUrl);
   $newUrl=str_replace("installAppCmd=$type", "", $newUrl);
   $newUrl=str_replace("&installAppTarget=$target", "", $newUrl);
   $newUrl=str_replace("installAppTarget=$target", "", $newUrl);
   hideurl("?".$newUrl);
}

function queueDeleteApp()
{
   $app = $_GET['deleteApp'];
   $type = $_GET['deleteAppCmd'];
   $target = $_GET['deleteAppTarget'];
   if ( ! empty($app) and ! empty($type) and ! empty($target) )
      run_cmd("queue $type $app delete $target");

   // Now we can remove those values from the URL
   $newUrl=http_build_query($_GET);
   $app=str_replace("/", "%2F", $app);
   $newUrl=str_replace("&deleteApp=$app", "", $newUrl);
   $newUrl=str_replace("deleteApp=$app", "", $newUrl);
   $newUrl=str_replace("&deleteAppCmd=$type", "", $newUrl);
   $newUrl=str_replace("deleteAppCmd=$type", "", $newUrl);
   $newUrl=str_replace("&deleteAppTarget=$target", "", $newUrl);
   $newUrl=str_replace("deleteAppTarget=$target", "", $newUrl);
   hideurl("?".$newUrl);
}

function getDispatcherStatus()
{
   return run_cmd("status");
}

function get_installed_list($target = "#system")
{
  global $sc;
  exec("$sc ". escapeshellarg("pkg " . $target . " installedlist"), $insarray);
  return explode(", ", $insarray[0]);
}

function parse_details($pbiorigin, $jail, $col) 
{
  global $sc;
  global $totalCols;
  global $inslist;

  if ( empty($jail) )
    $jail="#system";
  else
    $jail="#" . $jail;

  if ( empty($inslist) )
    $inslist = get_installed_list($jail);

  $cmd="pbi app $pbiorigin";
  exec("$sc ". escapeshellarg("$cmd name")
    . " " . escapeshellarg("pkg $jail remote $pbiorigin version") 
    . " " . escapeshellarg("$cmd comment") 
    . " " . escapeshellarg("$cmd confdir")
    . " " . escapeshellarg("pkg $jail remote $pbiorigin name") 
    . " " . escapeshellarg("pkg $jail remote $pbiorigin version")
    . " " . escapeshellarg("pkg $jail remote $pbiorigin comment")
    , $pbiarray);

  $pbiname = $pbiarray[0];
  $pbiver = $pbiarray[1];
  $pbicomment = $pbiarray[2];
  $pbicdir = $pbiarray[3];
  if ( empty($pbiname) or $pbiname == "[ERROR] Information not available" )
    $pbiname = $pbiarray[4];
  if ( empty($pbiver) or $pbiver == "[ERROR] Information not available" )
    $pbiver = $pbiarray[5];
  if ( empty($pbicomment) or $pbicomment == "[ERROR] Information not available" )
    $pbicomment = $pbiarray[6];

  if ( $col == 1 )
    print ("<tr>\n");

  // Get our values from this line
  print("  <td>\n");

  // Is this app installed?
  if ( array_search($pbiorigin, $inslist) !== false)
    print("    <button title=\"Delete this application\" style=\"float:right;\" onclick=\"delConfirm('" . $pbiname ."','".$pbiorigin."','pbi','system')\">X</button>\n");
  else
    print("    <button title=\"Install this application\" style=\"float:right;\" onclick=\"addConfirm('" . $pbiname ."','".$pbiorigin."','pbi','system')\">+</button>\n");

  print("    <a href=\"/?p=appinfo&app=$pbiorigin\" title=\"$pbicomment\"><img border=0 align=\"center\" height=48 width=48 src=\"images/pbiicon.php?i=$pbicdir/icon.png\" style=\"float:left;\"></a>\n");
  print("    <a href=\"/?p=appinfo&app=$pbiorigin\" style=\"margin-left:5px;\">$pbiname</a><br>\n");
  print("    <a href=\"/?p=appinfo&app=$pbiorigin\" style=\"margin-left:5px;\">$pbiver</a>\n");
  print("  </td>\n");

  if ( $col == $totalCols )
    print ("</tr>\n");
}

function display_cats($iconsize = "32")
{
  global $sc;
  exec("$sc ". escapeshellarg("pbi list cats"), $catarray);
  $catlist = explode(", ", $catarray[0]);
  foreach ( $catlist as $cat ) {
    if ( empty($cat) )
      continue;
    exec("$sc ". escapeshellarg("pbi cat $cat name"). " " . escapeshellarg("pbi cat $cat icon"). " " . escapeshellarg("pbi cat $cat comment"), $catdetails);
    echo "<img height=$iconsize width=$iconsize src=\"images/pbiicon.php?i=$catdetails[1]\"><a href=\"?p=appcafe&cat=$cat\" title=\"$catdetails[2]\">$catdetails[0]</a><br>";
    unset($catdetails);
  }

}

?>

