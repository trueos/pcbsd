<?php
defined('DS') OR die('No direct access allowed.');

  // Get the default network interface for this jail
  $defaultnic = exec("netstat -f inet -nrW | grep '^default' | awk '{ print $6 }'");

  // Get the default IP4 base range
  $defaultip4base = exec("netstat -f inet -nrW | grep '^default' | awk '{ print $2 }' | cut -d '.' -f 1-3");

  // Check for VIMAGE support
  $vimage = exec("/sbin/sysctl -qn kern.features.vimage");

  // Get the iocage pool
  $curpool = get_iocage_pool();

  // Check if the zpool changed
  if ( ! empty($_POST['iocpool']) and $curpool != $_POST['iocpool'] )
  {
    $curpool = $_POST['iocpool'];
    $dccmd = array("iocage activate " . $curpool);
    send_dc_cmd($dccmd);
  }
  
  $dccmd = array("iocage get ip4_autostart default", "iocage get ip4_autoend default", "iocage get ip4_autosubnet default");
  $output = send_dc_cmd($dccmd);
  $ip4start = $output["iocage get ip4_autostart default"];
  $ip4end = $output["iocage get ip4_autoend default"];
  $ip4subnet = $output["iocage get ip4_autosubnet default"];

  if ( (! empty($_POST['iocpool']) and $vimage != 1) and ( empty($_POST['ip4start']) or empty($_POST['ip4end']) or empty($_POST['ip4subnet']) ) )
  {
    $errormsg="ERROR: You must specify a valid IPv4 range / netmask!";
  }

  // Save the ip4 ranges / settings
  $setranges=true;
  if ( empty($_POST['ip4start']) or empty($_POST['ip4end']) or empty($_POST['ip4subnet']) ) {
    $setranges=false;
  } else {
    $ip4start = $_POST['ip4start'];
    $ip4end = $_POST['ip4end'];
    $ip4subnet = $_POST['ip4subnet'];
    if ( is_numeric($ip4start) and is_numeric($ip4end) and is_numeric($ip4subnet) ) {
      // validate
      if ( $ip4start > 254 or $ip4start < 1 ) {
        $setranges=false;
        $errormsg="ERROR: The ip4 start range must be between 1-254!";
      }
      if ( $ip4end > 254 or $ip4end < 1 ) {
        $setranges=false;
        $errormsg="ERROR: The ip4 end range must be between 1-254!";
      }
      if ( $ip4end <= $ip4start ) {
        $setranges=false;
        $errormsg="ERROR: The ip4 end range must be less than start range!";
      }
      if ( $ip4subnet < 16 or $ip4subnet > 30 ) {
        $setranges=false;
        $errormsg="ERROR: The ip4 subnet should be between 16-30";
      }
    } else {
      $setranges=false;
      $errormsg="ERROR: The ranges must be numbers!";
    }
  }

  if ( $ip4start == "none" )
    $ip4start="";
  
  if ( $ip4end == "none" )
    $ip4end="";

  if ( $ip4subnet == "none" )
    $ip4subnet="";

  if ( $setranges ) {
    $dccmd = array("iocage set ip4_autostart=$ip4start default", "iocage set ip4_autoend=$ip4end default", "iocage set ip4_autosubnet=$ip4subnet default");
    send_dc_cmd($dccmd);
  }

  if ( $setranges and ! empty($_GET['firstrun']) )
  {
    require("pages/jails.php");
  } else {

    if ( $firstrun )
      echo "<h1>Welcome to AppCafe!</h1><br>";
    else
      echo "<h1>Jail Configuration</h1><br>";

    if ( $vimage == 1 )
      echo "<p>The following ZFS pool will be used to store your Jails:</p>";
    else
      echo "<p>Each AppCafe managed jail requires an IP address on your network. Please specify a range of usable IPs which can be assigned to jails.</p>";

    if ( ! empty($errormsg) ) {
      echo "<br><p style=\"color:red;\">$errormsg</p>";
    }
?>
<table class="jaillist" style="width:100%">
<tr>
   <th></th>
   <th></th>
</tr>

<form method="post" action="?p=jailconfig&firstrun=<?php if ( $firstrun ) { echo "1"; } ?>">
<?php if ( $vimage != 1) { ?>
<tr>
  <td style="text-align: center; vertical-align: middle;">
  Available IPv4 Range
  </td>
  <td style="text-align: left; vertical-align: middle;">
    <?php echo $defaultip4base; ?>. 
    <input name="ip4start" type="text" size=3 maxlength=3 value="<?php echo "$ip4start"; ?>" /> - 
    <input name="ip4end" type="text" size=3 maxlength=3 value="<?php echo "$ip4end"; ?>" /> / 
    <input name="ip4subnet" type="text" size=2 maxlength=2 value="<?php echo "$ip4subnet"; ?>" />
  </td>
</tr>
<?php } ?>
<tr>
  <td style="text-align: center; vertical-align: middle;">Jail zpool:</td>
  <td style="text-align: left; vertical-align: middle;">
  <select name="iocpool">
  <?php
    $pools = get_zpools();
    foreach ($pools as $pool )
    {
       echo "<option value=\"" . $pool . "\"";
       if ($pool == $curpool )
         echo " selected>";
       else
         echo ">";
       echo $pool . "</option>\n";
    }
  ?>
  </select>
  </td>
</tr>
<tr>
  <td colspan="2"><input name="submit" type="submit" value="Save" class="btn-style" /></td>
</tr>

</form>

</table>

<?php
  }
?>
