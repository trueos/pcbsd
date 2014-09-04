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
    echo "<center><p>error logging in</p></center>";
      }
  }

  echo '<center>
  <form method="post" action="'.SELF.'">
  <table>
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

  <tr><td colspan=2 align=center><input type="submit" name="submit" value="Login" class="button"/></td></tr>
  </form></html>';

  exit(0); 
?>
