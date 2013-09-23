<?php

date_default_timezone_set ( "UTC" );
session_start();

ini_set('display_errors', 'On');

// Load the config
require("config.php");
require("logincheck.php");

if ( $loggedin != 0 )
{
  require("login.php");
  exit(1);
}

?>

<html>
<head>
<title>PC-ThinClient PXE Management</title>
</head>
<a href="?view=clients">Clients</a> - <a href="?view=configs">Install Configs</a> - <a href="?p=logout">Logout</a>
<br>
<hr>
<?php

 if ( isset($_GET['view']) )
  $view = $_GET['view'];
 else
  $view = "clients"; 

 if ("$view" == "clients" ) {
  include("clients.php");
 } else if ("$view" == "configs" ) {
  include("scripts.php");
 }
?>
</html>
