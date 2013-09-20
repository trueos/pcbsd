<?php

 require("config.php");
 require("functions.php");

 if ( $loggedin != 0 )
 {
  require("login.php");
  exit(1);
 }

 if ( isset($_GET['add']) )
    add_mac(); 
 else {
    if ( isset($_GET['doadd']) )
      doadd_mac();
    if ( isset($_GET['del']) )
      rem_mac($_GET['del']);

    // list the mac files
    list_mac();
 }

?>
