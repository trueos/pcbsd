<?
   defined('DS') OR die('No direct access allowed.');

   // Setup global variables and such

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

   // Legacy stuff from old AppCafe 1.0 - Could be removed at some point
   $jail = "#system";
   $jailUrl = "__system__";

   // Set if we are viewing recommended or all PBI
   $allPBI = "false";
   if ( ! empty($_GET['allPBI']))
     $allPBI = $_GET['allPBI'];

   // Are we running through the AppCafe UI Qt program?
   $onDesktop = "false";
   if ( ! empty($_GET['AppCafeUI']))
     $onDesktop = $_GET['AppCafeUI'];
?>
