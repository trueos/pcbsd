<?

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

?>
