<?
defined('DS') OR die('No direct access allowed.');

if ( ! empty($_GET['deleteJail'] ) )
{
   // Time to schedule a deletion
   $delJail=$_GET['deleteJail'];
   run_cmd("warden delete $delJail --confirm");
   hideurl();
}
 

if ( ! empty($_GET['toggle']) )
{
  $tjail = $_GET['toggle'];
  $sjail = $_GET['status'];
  if ( $sjail == "Running" )
    run_cmd("warden stop $tjail");
  else
    run_cmd("warden start $tjail");
  hideurl();
}

if ( ! empty($_GET['autostart']) )
{
  $tjail = $_GET['autostart'];
  run_cmd("warden auto $tjail");
  hideurl();
}

function print_jail($jail, $status)
{

  // Get some information about this jail
  global $sc;

  exec("$sc ". escapeshellarg("jail $jail autostart")
       . " " . escapeshellarg("jail $jail type")
       . " " . escapeshellarg("jail $jail ipv4")
       . " " . escapeshellarg("jail $jail ipv6")
       , $jailinfo);
  $jauto = $jailinfo[0];
  $jtype = $jailinfo[1];
  
  if ( $jauto == "true" )
     $autostatus="Enabled";
  else
     $autostatus="Disabled";

  print ("<tr>\n");
  print("  <td><a href=\"?p=jailinfo&jail=$jail\" style=\"text-decoration: underline;\">$jail</a></td>\n");
  print("  <td><a href=\"/?p=jails&autostart=$jail\" style=\"text-decoration: underline;\">$autostatus</a></td>\n");
  if ( $status == "Running" )
    print("  <td><a href=\"/?p=jails&toggle=$jail&status=$status\" style=\"color: green; text-decoration: underline;\">$status</a></td>\n");
  else
    print("  <td><a href=\"/?p=jails&toggle=$jail&status=$status\" style=\"color: red; text-decoration: underline;\">$status</a></td>\n");
  if ( $status == "Running" ) 
    print("  <td><a href=\"/?p=sysapp&jail=$jail\" style=\"text-decoration: underline;\">View Packages</a></td>\n");
  else
    print("  <td>Start jail to view</td>\n");

  print ("</tr>\n");
}

?>


<h1>System Jail Listing</h1>
<br>
<table class="jaillist" style="width:100%">
<tr>
   <th>Jail Name</th>
   <th>Autostart</th>
   <th>Status</th>
   <th>Packages</th>
</tr>

<?
   unset($jailoutput);
   $jailoutput = get_jail_list(true);

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
