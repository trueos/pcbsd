<?php
defined('DS') OR die('No direct access allowed.');

$showForm=true;
$jailMsg="Please enter the following information to create a new jail:";

if ( ! empty($_POST['jailname']) or ! empty($_POST['jailipv4']) )
{
  $badData=false;

  // Lets validate the information before creating a new jail
  $jailname = $_POST['jailname'];
  if ( empty($_POST['jailname']))
  { 
     $badData=true;
     $jailMsg="Missing host name!";
  }  

  if (preg_match('/[\'^£$%&*()}{@#~?><>,|=_+¬-]/', $jailname)) {
     $badData=true;
     $jailMsg="Invalid jail name, no special chars allowed!";
  }

  if ( ! $badData ) {
     $output = run_cmd("iocage create $jailname");
     $showForm = false;
?>
<h1>Jail Creation</h1>
<br>
<p>The jail, <?php echo "$jailname"; ?>, is queued for creation!</p><br>
<a href="?p=jails">Back to Jails</a>
<?php
  } // End of $badData
}

if ( $showForm ) {
?>

<h1>Jail Creation</h1>
<br>
<p><?php echo "$jailMsg"; ?></p><br>
<table class="jaillist" style="width:100%">
<tr>
   <th></th>
   <th></th>
</tr>

<form name="jailform" method="post" action="?p=jailcreate">
<tr>
  <td>Hostname</td>
  <td><input name="jailname" type="text" title="A valid hostname for this jail" value="<?php echo "$jailname"; ?>" /></td>
</tr>
<tr>
  <td colspan="2"><input name="submit" type="submit" value="Create Jail" class="btn-style" /></td>
</tr>

</form>

</table>

<?php } ?>
