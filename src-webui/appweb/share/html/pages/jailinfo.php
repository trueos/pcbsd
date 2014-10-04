<?
defined('DS') OR die('No direct access allowed.');

  if ( empty($jail) or $jail == "#system" )
     die("Invalid jail specified!");

  exec("$sc ". escapeshellarg("jail $jail ipv4")
       . " " . escapeshellarg("jail $jail ipv6")
       , $jailinfo);
  $jailipv4 = $jailinfo[0];
  $jailipv6 = $jailinfo[1];

  // Get the default network interface for this jail
  $dnic = run_cmd("warden get iface $jail");
  $jailnic = $dnic[0];

  // Get the list of nics available
  $nics = get_nics();

  // Are we saving this jail?
  if ( ! empty($_POST['jailipv4']) )
  {
     $postjailipv4 = $_POST['jailipv4'];
     $postjailnic = $_POST['jailnic'];
     if ( $postjailnic == "SYSDEFAULT" )
        $postjailnic="";

     // Has the IP changed?
     if ( $postjailipv4 != $jailipv4 )
     {
  	run_cmd("warden set ipv4 $jail $postjailipv4");
  	$jailipv4 = $postjailipv4;
     }

     // Has the jail NIC changed?
     if ( $postjailnic != $jailnic )
     {
  	run_cmd("warden set iface $jail $postjailnic");
  	$jailnic = $postjailnic;
     }

  }

?>
<p><? echo "$jailMsg"; ?></p>
<button title="Delete <? echo "$jail"; ?>" style="background-color: Transparent;background-repeat:no-repeat;float:right;border: none;background-image: url('/images/remove.png');background-size: 100%; height: 48px; width: 48px;" onclick="delJailConfirm('<? echo "$jail"; ?>')"></button>
<h1>Jail Configuration for <? echo "$jail"; ?></h1>
<table class="jaillist" style="width:100%">
<tr>
   <th></th>
   <th></th>
</tr>

<form method="post" action="?p=jailinfo&jail=<? echo "$jailUrl"; ?>">
<tr>
  <td>Jail Nickname</td>
  <td><? echo "$jail"; ?></td>
</tr>
<tr>
  <td>Jail IPv4 Address</td>
  <td><input name="jailipv4" type="text" value="<? echo "$jailipv4"; ?>" /></td>
</tr>
<tr>
  <td>Network Interface</td>
  <td><select name="jailnic">
  <?
     if ( empty($jailnic) )
       echo "<option value=\"SYSDEFAULT\" selected>System Default</option>";
     else
       echo "<option value=\"SYSDEFAULT\">System Default</option>";
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
