<?
   defined('DS') OR die('No direct access allowed.');

   // Check if we need to display desktop apps, or just server / CLI
   if ( file_exists("/usr/local/bin/startx") === false )
     $hasDesktop=false;
   else
     $hasDesktop=true;

   // Location of syscache client
   $sc="/usr/local/bin/syscache";

   // Location of PBI index dir
   $pbiindexdir="/var/db/pbi/index";

   // Probally shouldn't modify these below
   ///////////////////////////////////////////////////

   // Set if we are viewing recommended or all PBI
   $allPBI = "false";
   if ( ! empty($_GET['allPBI']))
     $allPBI = $_GET['allPBI'];

   // Are we running through the AppCafe UI Qt program?
   $onDesktop = "false";
   if ( ! empty($_GET['AppCafeUI']))
     $onDesktop = $_GET['AppCafeUI'];

   // Lets verify that the dispatcher ID is good
   unset($vdisid);
   $return_var=1;
   putenv("PHP_DISID=$DISPATCHID");
   exec("/usr/local/bin/sudo /usr/local/share/appcafe/dispatcher verify_disid", $output, $return_var);
   if ( $return_var != 0 )
   { 
     if ( $login_on_fail ) {
       include('include/login.php');
       exit(0);
     } else {
       die("Invalid auth token!");
     }
   }
?>
