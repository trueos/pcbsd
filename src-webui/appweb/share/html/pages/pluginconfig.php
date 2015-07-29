<?php
defined('DS') OR die('No direct access allowed.');

  // Get the default network interface for this jail
  $defaultnic = exec("netstat -f inet -nrW | grep '^default' | awk '{ print $6 }'");

  // Get the default IP4 base range
  $defaultip4base = exec("netstat -f inet -nrW | grep '^default' | awk '{ print $2 }' | cut -d '.' -f 1-3");

  // Get the list of nics available
  $nics = get_nics();

  $output = run_cmd("iocage get ip4_autostart default");
  $ip4start = $output[0];
  $output = run_cmd("iocage get ip4_autoend default");
  $ip4end = $output[0];

  // Save the ip4 ranges / settings
  $setranges=true;
  if ( empty($_POST['ip4start']) or empty($_POST['ip4end']) ) {
    $setranges=false;
  } else {
    $ip4start = $_POST['ip4start'];
    $ip4end = $_POST['ip4end'];
    if ( is_numeric($ip4start) and is_numeric($ip4end) ) {
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
    } else {
      $setranges=false;
      $errormsg="ERROR: The ranges must be numbers!";
    }
  }

  if ( $ip4start == "none" )
    $ip4start="";
  
  if ( $ip4end == "none" )
    $ip4end="";

  if ( $setranges ) {
    run_cmd("iocage set ip4_autostart=$ip4start default");
    run_cmd("iocage set ip4_autoend=$ip4end default");
  }

  if ( $setranges and ! empty($_GET['firstrun']) )
  {
    require("pages/plugins.php");
  } else {

    if ( $firstrun )
      echo "<h1>Welcome to AppCafe Plugins!</h1><br>";
    else
      echo "<h1>Plugin Configuration</h1><br>";

    echo "<p>Each AppCafe managed plugin requires an IP address on your network. Please specify a range of usable IPs which can be assigned to plugins.</p>";

    if ( ! empty($errormsg) ) {
      echo "<br><p style=\"color:red;\">$errormsg</p><br>";
    }
?>
<table class="jaillist" style="width:100%">
<tr>
   <th></th>
   <th></th>
</tr>

<form method="post" action="?p=pluginconfig&firstrun=<?php if ( $firstrun ) { echo "1"; } ?>">
<tr>
  <td style="text-align: center; vertical-align: middle;">
  Available IPv4 Range
  </td>
  <td style="text-align: left; vertical-align: middle;">
    <?php echo $defaultip4base; ?>. 
    <input name="ip4start" type="text" size=3 maxlength=3 value="<?php echo "$ip4start"; ?>" /> - 
    <input name="ip4end" type="text" size=3 maxlength=3 value="<?php echo "$ip4end"; ?>" />
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
