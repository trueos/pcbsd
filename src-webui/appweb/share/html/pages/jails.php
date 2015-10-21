<?php
defined('DS') OR die('No direct access allowed.');

if ( ! empty($_GET['deleteJail'] ) )
{
   // Time to schedule a deletion
   $delJail=$_GET['deleteJail'];
   $dccmd = array("iocage destroy -f $delJail");
   send_dc_cmd($dccmd);
   hideurl();
}
 

if ( ! empty($_GET['toggle']) )
{
  $tjail = $_GET['toggle'];
  $sjail = $_GET['status'];
  if ( $sjail == "Running" ) {
    $dccmd = array("iocage stop $tjail");
    send_dc_cmd($dccmd);
  } else {
    $dccmd = array("iocage start $tjail");
    send_dc_cmd($dccmd);
  }
  hideurl();
}

if ( ! empty($_GET['autostart']) )
{
  $tjail = $_GET['autostart'];
  $dccmd = array("iocage set boot=on $tjail");
  send_dc_cmd($dccmd);
  hideurl();
}

function print_jail($jail, $status)
{

  // Get some information about this jail
  $sccmd = array("jail $jail autostart", "jail $jail type", "jail $jail tag");
  $response = send_sc_query($sccmd);
  $jauto = $response["jail $jail autostart"];
  $jtype = $response["jail $jail type"];
  $jtag = $response["jail $jail tag"];

  $jtag = substr($jtag, 8);

  if ( $jauto == "true" )
     $autostatus="Enabled";
  else
     $autostatus="Disabled";

  print ("<tr>\n");
  print("  <td><a href=\"?p=jailinfo&jail=$jail\" style=\"text-decoration: underline;\">$jtag</a></td>\n");
  print("  <td><a href=\"/?p=jails&autostart=$jail\" style=\"text-decoration: underline;\">$autostatus</a></td>\n");
  if ( $status == "Running" )
    print("  <td><a href=\"/?p=jails&toggle=$jail&status=$status\" style=\"color: green; text-decoration: underline;\">$status</a></td>\n");
  else
    print("  <td><a href=\"/?p=jails&toggle=$jail&status=$status\" style=\"color: red; text-decoration: underline;\">$status</a></td>\n");
  if ( $status == "Running" ) {
    $sccmd = array("jail $jail ip");
    $response = send_sc_query($sccmd);
    $jip = $response["jail $jail ip"];
    print("  <td>$jip</td>\n");
  } else
    print("  <td></td>\n");

  print ("</tr>\n");
}

   // Get the jail list
   unset($jailoutput);
   $jailoutput = get_jail_list(true);

   // No jails, lets display a intro page
   if ( empty($jailoutput[0]) and empty($jailoutput[1]) ) {
?>

<h1>Welcome to the Jail Manager!</h1>
<br>
<p>This manager will assist you in the creation and management of jails on this machine. To get started, let us first <a href="/?p=jailcreate" style="text-decoration: underline;">create a new jail.</a></p><br>
<center><img src="/images/warden.png"></center>
<?php
   } else {
     if ($noJails == "YES") {
       echo "<br> -- No jails are running! Please start a jail to browse the AppCafe -- <br><br>";
     }
     // We have jails to display
?>


<h1>System Jail Listing</h1>
<br>
<table class="jaillist" style="width:100%">
<tr>
   <th>Jail Name</th>
   <th>Autostart</th>
   <th>Status</th>
   <th>IP</th>
</tr>

<?php
   $running=$jailoutput[0];
   $stopped=$jailoutput[1];
   $rarray = explode( ", ", $running);
   $sarray = explode( ", ", $stopped);

   foreach ($rarray as $jail) {
     if ( empty($jail) )
        continue;
     if ( $jail == $delJail )
        continue;
     print_jail($jail, "Running");
   }

   foreach ($sarray as $jail) {
     if ( empty($jail) )
        continue;
     if ( $jail == $delJail )
        continue;
     print_jail($jail, "Stopped");
   }

?>

</table>

<?php } ?>
