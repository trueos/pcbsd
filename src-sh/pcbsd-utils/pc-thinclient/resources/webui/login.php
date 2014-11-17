<html>
<form method="post" action="index.php">
<center>
<h2>Login required</h2>
<table>
<tr><td>
Username<br>
<input type="text" name="username"><br>
Password<br>
<input type="password" name="password"><br>
<center><input type="submit" name="login" value="Login"></center><br>

<?
if ( $loginfailed == 1 )
{
  echo "<h3><font color=\"#990000\">Login failed!</font></h3>";
}
?>
</td></tr>
</table>
</form>
</html>
