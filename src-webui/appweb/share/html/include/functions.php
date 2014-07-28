<?

function hideurl()
{
   $p = ($_GET['p']);
   ?>
   <script>
	window.history.pushState('AppWeb', 'AppWeb', '/?p=<? echo $p; ?>');
   </script>
   <?
}

// Runs commands through the sudo dispatcher
function run_cmd($cmd)
{
   exec("/usr/local/bin/sudo /usr/local/share/appweb/dispatcher $cmd", $output);
   return $output;
}

?>
