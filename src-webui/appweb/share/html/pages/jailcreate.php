<?
defined('DS') OR die('No direct access allowed.');

$showForm=true;
$jailMsg="Please enter the following information to create a new jail:";

if ( ! empty($_POST['jailname']) and ! empty($_POST['jailipv4']) )
{
  $badData=false;

  // Lets validate the information before creating a new jail
  $jailname = $_POST['jailname'];
  if (preg_match('/[\'^£$%&*()}{@#~?><>,|=_+¬-]/', $jailname)) {
     $badData=true;
     $jailMsg="Invalid jail name, no special chars allowed!";
  }

  $jailipv4 = $_POST['jailipv4'];
  if ( ! filter_var($jailipv4, FILTER_VALIDATE_IP)) {
     $badData=true;
     $jailMsg="Invalid IPV4 address!";
  }

  // CHeck for the /24 at the end of the IP
  if ( strpos('/', $jailipv4) === false )
     $jailipv4 = $jailipv4 . "/24";
  

  if ( ! $badData ) {
     run_cmd("warden create $jailname --ipv4=$jailipv4");
     $showForm = false;
?>
<h1>Jail Creation</h1>
<br>
<p>The jail, <?echo "$jailname"; ?>, is queued for creation!</p><br>
<a href="?p=jails">Back to Jails</a>
<?
  } // End of $badData
}

if ( $showForm ) {
?>

<h1>Jail Creation</h1>
<br>
<p><? echo "$jailMsg"; ?></p>
<table class="jaillist" style="width:100%">
<tr>
   <th></th>
   <th></th>
</tr>

<form method="post" action="?p=jailcreate">
<tr>
  <td>Jail Nickname</td>
  <td><input name="jailname" type="text" value="<? echo "$jailname"; ?>" /></td>
</tr>
<tr>
  <td>Jail IPv4 Address</td>
  <td><input name="jailipv4" type="text" value="<? echo "$jailipv4"; ?>" /></td>
</tr>
<tr>
  <td colspan="2"><input name="submit" type="submit" value="createjail" /></td>
</tr>

</form>

</table>

<? } ?>
