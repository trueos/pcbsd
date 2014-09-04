<? defined('DS') OR die('No direct access allowed.'); ?>
<br>
<?
  // Page to display left menu for appcafe / jails
  // Will need a major visual upgrade

  if ( $page == "appcafe" or $page == "sysapp" or $page == "appinfo" or $page == "dispatcher" )
    display_cats();
  else
    display_jail_menu();
?>
