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

  if (preg_match('/[\'^£$%&*()}{@#~?><> ,|=_+¬-]/', $jailname)) {
     $badData=true;
     $jailMsg="Invalid jail name, no special chars allowed!";
  }

  $flags = "";
  $sshpub = "";
  // Check if SSH is enabled
  if ( ! empty($_POST['sshd']) and $_POST['sshd'] == "checked" ) {
    $badData = true;
    $jailMsg="SSH public key required!";
    if ( ! empty($_POST['sshkey']) ) {
       $badData = false;
       $sshpub = str_replace("\n", "", $_POST['sshkey']);
    }
  }
  $flags .= "sshd ::::" . $sshpub . ":::: ";

  $customipv4 = "";
  if ( ! empty($_POST['ipv4']) and $_POST['ipv4'] == "checked" ) {
    $badData = true;
    $jailMsg="Valid IP address required!";
    if ( ! empty($_POST['customipv4']) ) {
       $customipv4 = str_replace("\n", "", $_POST['customipv4']);
       if ( strpos($customipv4, "/") === false )
         $valIP = $customipv4;
       else
         $valIP = strstr($customipv4, "/", true);

       if ( filter_var($valIP, FILTER_VALIDATE_IP) ) {
         if ( strpos($customipv4, "/") === false )
           $customipv4 .="/24";
         $badData = false;
       }
    }
  }
  $flags .= "customipv4 ::::" . $customipv4 . ":::: ";

  if ( ! $badData ) {
     $dccmd = array("iocage create $jailname $flags");
     send_dc_cmd($dccmd);
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

<script type="text/javascript">
function chk_onchange()
{
 var ctl = document.getElementById(this.getAttribute("enableWhenTrue"));
 ctl.disabled = !this.checked;
}
</script>

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
  <td><input name="jailname" type="text" class="tooltip" title="A valid hostname for this jail" value="<?php echo "$jailname"; ?>" /></td>
</tr>
</tr>
  <td>Enable remote access (SSH)</td>
  <td><input name="sshd" type="checkbox" value="checked" enableWhenTrue="sshkey" onchange="chk_onchange.call(this)"></td>
</tr>
<tr>
  <td>SSH Public Key (Username "freebsd")</td>
  <td><textarea class="tooltip" title="Paste your id_rsa.pub file contents here" id="sshkey" name="sshkey" disabled></textarea></td>
</tr>
</tr>
  <td>Manually Specify IPv4 address</td>
  <td><input name="ipv4" type="checkbox" value="checked" enableWhenTrue="customipv4" onchange="chk_onchange.call(this)"></td>
</tr>
<tr>
  <td> IPv4 Address</td>
  <td><input class="tooltip" title="A manually specified IPv4 address - Example: 10.0.10.5/24" type="text" id="customipv4" name="customipv4" disabled></td>
</tr>
<tr>
  <td colspan="2"><input name="submit" type="submit" value="Create Jail" class="btn-style" /></td>
</tr>

</form>

</table>

<?php } ?>
