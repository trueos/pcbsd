<?php
defined('DS') OR die('No direct access allowed.');

  $jail = $_GET['jail'];

  if ( empty($jail) or $jail == "#system" )
     die("Invalid jail specified!");

  $sccmd = array("jail $jail ipv4",
           "jail $jail ipv6",
           "jail $jail tag"
           );
  $response = send_sc_query($sccmd);
  $jailipv4 = $response["jail $jail ipv4"];
  $dnic = strstr($jailipv4, "|", TRUE);
  $jailipv4 = substr(strstr($jailipv4, "|"), 1);
  $jailipv6 = $response["jail $jail ipv6"];
  $jailtag = $response["jail $jail tag"];

  // Get the default network interface for this jail
  $defaultnic = exec("netstat -f inet -nrW | grep '^default' | awk '{ print $6 }'");
  if ( empty($dnic) )
    $dnic = $defaultnic;
  $jailnic = $dnic;

  // Get the list of nics available
  $nics = get_nics();

  // Are we saving this jail?
  if ( ! empty($_POST['jailipv4']) )
  {
     $postjailipv4 = $_POST['jailipv4'];
     $postjailnic = $_POST['jailnic'];
     if ( $postjailnic == "SYSDEFAULT" )
        $postjailnic=$defaultnic;

     // Has the IP changed?
     if ( $postjailipv4 != $jailipv4 )
     {
  	$jailipv4 = $postjailipv4;
     }

     // Has the jail NIC changed?
     if ( $postjailnic != $jailnic )
     {
  	$jailnic = $postjailnic;
     }
     run_cmd("iocage set ip4_addr=\"$postjailnic|$postjailipv4\" $jail");

  }

?>
<p><?php echo "$jailMsg"; ?></p>
<button title="Delete <?php echo "$jail"; ?>" style="background-color: Transparent;background-repeat:no-repeat;float:right;border: none;background-image: url('/images/remove.png');background-size: 100%; height: 48px; width: 48px;" onclick="delJailConfirm('<?php echo "$jail"; ?>')"></button>
<h1>Jail Configuration for <?php echo "$jail"; ?></h1>
<table class="jaillist" style="width:100%">
<tr>
   <th></th>
   <th></th>
</tr>

<form method="post" action="?p=jailinfo&jail=<?php echo "$jailUrl"; ?>">
<tr>
  <td>Jail Nickname</td>
  <td><?php echo "$jtag"; ?></td>
</tr>
<tr>
  <td>Jail IPv4 Address</td>
  <td><input name="jailipv4" type="text" value="<?php echo "$jailipv4"; ?>" /></td>
</tr>
<tr>
  <td>Network Interface</td>
  <td><select name="jailnic">
  <?php
     foreach($nics as $nic)
     {
       if ( $jailnic == $nic )
         echo "<option value=\"$nic\" selected>$nic</option>";
       else
         echo "<option value=\"$nic\">$nic</option>";
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
