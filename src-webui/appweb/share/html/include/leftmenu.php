<?php defined('DS') OR die('No direct access allowed.'); ?>
<br>
<?php
  if ( $page == "plugins" or $page == "sysplugins" or $page == "plugininfo" ) {
    display_plugin_cats();
    return(0);
  }

  if ( $page != "dispatcher" ) {
    display_cats();
  }
?>
