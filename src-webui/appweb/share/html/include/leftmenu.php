<?php defined('DS') OR die('No direct access allowed.'); ?>
<br>
<?php
  if ( stripos($page, "plugin") !== false) {
    display_plugin_cats();
    return(0);
  }

  if ( $page != "dispatcher" and ! $pluginDispatcher ) {
    display_cats();
  }
?>
