<?
  defined('DS') OR die('No direct access allowed.');

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
    $_SESSION['username'] = $_POST['username'];
    header('Location:  ' . $_SERVER['PHP_SELF']);
      }else {
          //invalid login
    echo "<p>error logging in</p>";
      }
  }

  echo '<form method="post" action="'.SELF.'">
  <h2>Login</h2>
  <p><label for="username">Username</label> <input type="text" id="username" name="username" value="" /></p>
  <p><label for="password">Password</label> <input type="password" id="password" name="password" value="" /></p>
  <p><input type="submit" name="submit" value="Login" class="button"/></p>
  </form>';

  exit(0); 
?>
