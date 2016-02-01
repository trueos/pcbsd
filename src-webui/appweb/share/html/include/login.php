<br>
<?php
  $page = $_GET['p'];
  defined('DS') OR die('No direct access allowed.');

  // Set some globals for mobile detection
  $detect = new Mobile_Detect;
  $deviceType = ($detect->isMobile() ? ($detect->isTablet() ? 'tablet' : 'phone') : 'computer');
  $scriptVersion = $detect->getScriptVersion();

  $appsettings = parse_ini_file("/usr/local/etc/appcafe.conf");

  if (isset($_GET['logout'])) {
    $_SESSION['username'] = '';
    header('Location:  ' . $_SERVER['PHP_SELF']);
  }

  if ( isset($_POST['username'])) {
    $goodlogin = false;
    if ( ! $loadedglobals ) {
      require('vendor/autoload.php');
      require("include/globals.php");
      require("include/functions.php");
    }

    $user = $_POST['username'];
    $pass = $_POST['password'];
    // Lets try auth with WebSocket server
    $sccmd = array("username" => "$user", "password" => "$pass");
    $response = send_sc_query($sccmd, "auth");
    if ( $response["code"] == "401" or $response["message"] == "Unauthorized" ) {
      print_r($response);
      $goodlogin = false;
    } else {
      $goodlogin = true;
      $apikey = $response[0];
    }
  
    if ( $goodlogin ) {
       $_SESSION['timeout'] = time();
       $_SESSION['username'] = $_POST['username'];
       $_SESSION['apikey'] = $apikey;

       // Now we can relocate back to main page
       header('Location:  ' . $_SERVER['PHP_SELF'] . "?p=" . $page);
    } else {
       //invalid login
       $perror="error logging in!";
    }
  }
  


?>
<html>
<head>
<title>AppCafe Login</title>
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
<form method="post" action="<?php echo "$SELF" . "?p=" . $page; ?>">
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
<input type="text" id="username" name="username" value="root" readonly/></p></td>
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
