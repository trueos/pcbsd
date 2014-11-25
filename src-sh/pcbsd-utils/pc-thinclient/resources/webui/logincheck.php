<?

function session_defaults() { 
  $_SESSION['logged'] = false; 
  $_SESSION['uid'] = 0; 
  $_SESSION['username'] = ''; 
  $_SESSION['password'] = ''; 
  $_SESSION['cookie'] = ''; 
  $_SESSION['remember'] = false; 
}

if (!isset($_SESSION['uid']) ) { 
   session_defaults(); 
}

if ( isset($_GET['p']) )
{
   if ( $_GET['p'] == "logout" )
     session_defaults(); 
}

$loginfailed = 0;
$loggedin = "1";

function check_my_login() {
  global $loginfailed;
  global $loggedin;
  global $AUTHUSERS;
  global $AUTHTOKEN;
  global $username;

  if ( ! empty($_SESSION['username']) ){
    $cUser = $_SESSION['username'];
    $cTime = $_SESSION['time'];
    $cToken = $_SESSION['token'];
    $lTime = date("YmdHi");
    // If 2 hours have passed, log them out
    if ( $lTime - $cTime > 120 )
    {
      echo "<center>Session Expired...</center>";
      session_defaults(); 
      return;
    }

    $cHash = hash("sha256", "$AUTHTOKEN $cTime $cUser");
    if ( $cHash != $cToken ) {
      echo "<center>Invalid token...</center>";
      session_defaults(); 
      return;
    }

    // Update the token / time
    $username = $cUser;
    $_SESSION["time"] = $lTime;
    $_SESSION["token"] = hash("sha256", "$AUTHTOKEN $lTime $username");
    $loggedin = "0";
    return;
  }

  // Not logged in
  $loggedin = "1";
  if (isset($_POST['username']) and isset($_POST['password']) ) {
       $username = $_POST['username'];
       $pass = $_POST['password'];
     
       $ADMINPASS = $AUTHUSERS[$username]; 
       $ADMINUSER = $username;
     
       if ( "$pass" == "$ADMINPASS" and $username == $ADMINUSER and ! empty($ADMINPASS) and ! empty($ADMINUSER) )
       {
          $_SESSION["username"] = "$username";
          $lTime = date("YmdHi");
          $_SESSION["time"] = $lTime;
          $_SESSION["token"] = hash("sha256", "$AUTHTOKEN $lTime $username");
          $loggedin = "0";
       } else {
         session_defaults(); 
         $loginfailed = 1;
       }
  }
}

check_my_login();

?>
