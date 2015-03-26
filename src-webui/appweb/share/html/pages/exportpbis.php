<?php
 defined('DS') OR die('No direct access allowed.');

 if ( empty($_GET['jail']) or ! empty($_GET['changeappcafejail']))
 {
    display_jail_appcafeselection("exportpbis");
    return;
 }

 header("Cache-Control: public");
 header("Content-Description: File Transfer");
 header("Content-Type: application/octet-stream; "); 
 header("Content-Disposition: attachment; filename=pbiexportlist.txt");
 header("Content-Transfer-Encoding: binary"); 
 system("PATH=\$PATH:/usr/local/bin:/usr/local/sbin /usr/local/sbin/pbi_info");

?>
