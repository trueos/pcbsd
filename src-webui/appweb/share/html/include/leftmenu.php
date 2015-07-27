<?php defined('DS') OR die('No direct access allowed.'); ?>
<br>
<?php
  if ( $page == "jails" or $page == "jailinfo" or $page == "jailcreate" ) {
    display_jail_menu();
    return(0);
  }

  if ( $page != "dispatcher" ) {
    //display_jail_chooser();
    display_cats();
  }
?>
