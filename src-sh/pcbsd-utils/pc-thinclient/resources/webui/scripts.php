<?php

 require("config.php");
 require("functions.php");

 if ( $loggedin != 0 )
 {
   require("login.php");
   exit(1);
 }


 if ( isset($_GET['add']) )
    add_cfg(); 
 else if (isset($_GET['edit']) )
    edit_cfg($_GET['edit']); 
 else {
    if ( isset($_GET['doadd']) )
      doadd_cfg();
    if ( isset($_GET['doedit']) )
      doedit_cfg($_GET['doedit']);
    if ( isset($_GET['del']) )
      rem_cfg($_GET['del']);

    // list the config files
    list_cfgs();
 }

?>
