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
?>
