<br>
<?php
  defined('DS') OR die('No direct access allowed.');

  // Set some globals for mobile detection
  $detect = new Mobile_Detect;
  $deviceType = ($detect->isMobile() ? ($detect->isTablet() ? 'tablet' : 'phone') : 'computer');
  $scriptVersion = $detect->getScriptVersion();

  $appsettings = parse_ini_file("/usr/local/etc/appcafe.conf");

  if ( ! file_exists("/usr/local/etc/appcafe.pwd") and $appsettings['auth'] != "pam" )
     die( "No username / password setup!");

  if ( empty($appsettings['auth']) or $appsettings['auth'] == "local" )
  {
    $userdb = parse_ini_file("/usr/local/etc/appcafe.pwd");
    $username = $userdb['username'];
    $password = $userdb['password'];
    $auth="local";
    if ( empty($username) or empty($password) )
      die( "No username / password setup!");
  } else {
    $username = "root";
    $auth="pam"; 
  }

  if (isset($_GET['logout'])) {
    $_SESSION['username'] = '';
    header('Location:  ' . $_SERVER['PHP_SELF']);
  }

  if ( isset($_POST['username'])) {
    $goodlogin = false;
    if ( $auth == "local" && $_POST['username'] == $username && password_verify($_POST['password'], $password) ) {
      $goodlogin = true;
     } else if ( $auth == "pam" ) {

       $descriptorspec = array(
         0 => array("pipe", "r"),  // stdin is a pipe that the child will read from
         1 => array("pipe", "w")   // stdout is a pipe that the child will write to
       );

       $cwd = '/tmp';
       $env = array();

       $process = proc_open("/usr/local/bin/sudo -k -S /usr/local/share/appcafe/dispatcher-localauth-pam", $descriptorspec, $pipes, $cwd, $env);

       if (is_resource($process)) {
         fwrite($pipes[0], $_POST['password'] . "\n");
         fclose($pipes[0]);

         $idcontents = stream_get_contents($pipes[1]);
         fclose($pipes[1]);

         $return_value = proc_close($process);
	 if ( $return_value == 0 ) {
           $idcontents = str_replace(array("\n","\r"," "),"",$idcontents);
           $goodlogin = true;
           $_SESSION['dispatchid'] = $idcontents;
         }
       }

     }

     if ( $goodlogin ) {
       $_SESSION['timeout'] = time();
       $_SESSION['username'] = $_POST['username'];

       if ( $auth == "local" ) {
         // We logged in, so lets set the dispatcher ID and such
         $rawpassword = $_POST['password'];
         putenv("PHP_DISUSER=$username");
         putenv("PHP_DISPASS=$rawpassword");
         unset($output);
         $return_var=1;
         exec("/usr/local/bin/sudo /usr/local/share/appcafe/dispatcher getdisid", $output, $return_var);
         $_SESSION['dispatchid'] = $output[0];
         if ( $return_var != 0 )
         {
           print_r($output);
           die("Failed getting dispatcher ID!");
         }
       }

       // Now we can relocate back to main page
       header('Location:  ' . $_SERVER['PHP_SELF']);
     } else {
       //invalid login
       $perror="error logging in!";
     }
  }


?>
<html>
<head>
<meta name="viewport" content="width=device-width,initial-scale=1" />
<link rel="stylesheet" href="/css/_reset.css" />
<link rel="stylesheet" href="/css/main.css" />
<link rel="stylesheet" href="/css/tabs.css" />
<link rel="stylesheet" href="/css/popbox.css" />
<link rel="stylesheet" href="http://fonts.googleapis.com/css?family=Open+Sans:400">
</head>
<body>
<?php
  if ( $timeout )
    echo "Session timed out, please log in again...<br>";

  if ($deviceType == "computer" or $deviceType == "tablet") {
    echo "<div id=\"body\" style=\"height:100%;width:400px;margin-top:0.50em;margin-left:auto;margin-right: auto;\">
";
  } else {
    echo "<div id=\"body\" style=\"height:100%;width:100%;position:absolute;margin-top:0.00em;margin-left:0.00em;\">";
  }
?>
<p><?php echo "$perror"; ?></p>
<form method="post" action="<?php echo "$SELF"; ?>">
<table class="login" width="<?php if ($deviceType == "computer") { echo "400"; } else { echo "100%"; } ?>">
<tr>
  <th></th>
  <th></th>
</tr>
<tr><td colspan=2 align=center>
<center><img src="/images/appcafe.png" height=64 width=64><br>
<center>AppCafe Login
</td></tr>
<tr>
<td colspan=2><p align="center"><label for="username">Username</label><br>
<input type="text" id="username" name="username" value="" /></p></td>
 </tr>
<tr>
<td colspan=2><p align="center"><label for="password">Password</label><br>
<input type="password" id="password" name="password" value="" /></p></td>
</tr>

<tr><td colspan=2 align=center><center><input type="submit" name="submit" value="Login" class="btn-style"/></td></tr>
</form>
</div>
<body>
</html>
<?php
  exit(0); 
?>
