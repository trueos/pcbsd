<?
  defined('DS') OR die('No direct access allowed.');

  // Set some globals for mobile detection
  $detect = new Mobile_Detect;
  $deviceType = ($detect->isMobile() ? ($detect->isTablet() ? 'tablet' : 'phone') : 'computer');
  $scriptVersion = $detect->getScriptVersion();

  if ( ! file_exists("/usr/local/etc/appcafe.pwd") )
     die( "No username / password setup!");

  $userdb = parse_ini_file("/usr/local/etc/appcafe.pwd");
  $username = $userdb['username'];
  $password = $userdb['password'];

  if ( empty($username) or empty($password) )
     die( "No username / password setup!");

  if(isset($_GET['logout'])) {
      $_SESSION['username'] = '';
      header('Location:  ' . $_SERVER['PHP_SELF']);
  }

  if(isset($_POST['username'])) {
      if($_POST['username'] == $username && password_verify($_POST['password'], $password) ) {
        $_SESSION['timeout'] = time();
        $_SESSION['username'] = $_POST['username'];
        header('Location:  ' . $_SERVER['PHP_SELF']);
      }else {
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
<?
  if ( $timeout )
    echo "Session timed out, please log in again...<br>";

  if ($deviceType == "computer" or $deviceType == "tablet") {
    echo "<div id=\"body\" style=\"height:100%;width:400px;margin-top:0.50em;margin-left:auto;margin-right: auto;\">
";
  } else {
    echo "<div id=\"body\" style=\"height:100%;width:100%;position:absolute;margin-top:0.00em;margin-left:0.00em;\">";
  }
?>
<p><? echo "$perror"; ?></p>
<form method="post" action="<? echo "$SELF"; ?>">
<table class="jaillist" width="<? if ($deviceType == "computer") { echo "500"; } else { echo "100%"; } ?>">
<tr>
  <th></th>
  <th></th>
</tr>
<tr><td colspan=2 align=center>
<img src="/images/appcafe.png" height=64 width=64><br>
AppCafe Login
</td></tr>
<tr>
<td><label for="username">Username</label></td><td><input type="text" id="username" name="username" value="" /></p></td>
 </tr>
<tr>
<td><label for="password">Password</label></td>
<td><input type="password" id="password" name="password" value="" /></td>
</tr>

<tr><td colspan=2 align=center><input type="submit" name="submit" value="Login" class="btn-style"/></td></tr>
</form>
</div>
<body>
</html>
<?
  exit(0); 
?>
