<?php defined('DS') OR die('No direct access allowed.'); ?>
<div id="container" style="width:100%;height:100%;" taborder="0">

<?php
if ( strpos($page, "plugin") === false and ! $pluginDispatcher)
{
  $dflag="appcafe";
?>
<div id="cssmenu" style="height:3.50em;">
<ul>
  <li <?php if ( $page=="appcafe" ) { echo "class='active'"; } ?>><a href="/?p=appcafe"><img src="/images/appcafe.png" height=32 width=32> &nbsp;&nbsp;Home</a></li>
  <li <?php if ( $page=="sysapp" ) { echo "class='active'"; } ?>><a href="/?p=sysapp"><img src="/images/install.png" height=32 width=32> &nbsp;&nbsp;Installed Apps</a></li>
  <li <?php if ( $page=="appcafe-search" ) { echo "class='active'"; } ?>><a href="/?p=appcafe-search"><img src="/images/search.png" height=32 width=32> &nbsp;&nbsp;App Search</a></li>
  <li><a href="/?p=plugins" style="align:right;"><img src="/images/jail.png" height=32 width=32> &nbsp;&nbsp;App Containers</a></li>
<?php
if (USERNAME)
  echo "  <li><a href=\"/?logout=true\"><img src=\"/images/logout.png\" height=32 width=32> Logout</a></li>";
?>
  <li id="notifier"></li>
</ul>
</div>

<?php
} else {
  // Plugin menus
  $dflag="plugins";
?>
<div id="cssmenu" style="height:3.50em;">
<ul>
  <li <?php if ( $page=="plugins" ) { echo "class='active'"; } ?>><a href="/?p=plugins"><img src="/images/jail.png" height=32 width=32> &nbsp;&nbsp;Home</a></li>
  <li <?php if ( $page=="sysplugins" ) { echo "class='active'"; } ?>><a href="/?p=sysplugins"><img src="/images/install.png" height=32 width=32> &nbsp;&nbsp;Installed Apps</a></li>
  <li <?php if ( $page=="pluginconfig" ) { echo "class='active'"; } ?>><a href="/?p=pluginconfig"><img src="/images/configure.png" height=32 width=32> &nbsp;&nbsp;Configuration</a></li>
<?php
if ( $sysType != "APPLIANCE" ) {
?>
  <li <?php if ( $page=="appcafe" ) { echo "class='active'"; } ?>><a href="/?p=appcafe"><img src="/images/appcafe.png" height=32 width=32> &nbsp;&nbsp;System Packages</a></li>
<?php
}
if (USERNAME)
  echo "  <li><a href=\"/?logout=true\"><img src=\"/images/logout.png\" height=32 width=32> Logout</a></li>";
?>
  <li id="notifier"></li>
</ul>
</div>

<?php
}
?>

<body>
<?php if ( $page != "dispatcher" ) { ?>
<div id="leftmenu" style="height:100%;width:12.00em;position:absolute;margin:16px;">
  <?php 
     require("include/leftmenu.php");
  ?>
</div>
<?php } ?>

<script type="text/javascript">
$(document).ready(function () {
    var interval = 10000;   //number of mili seconds between each call
    var refresh = function() {
        $.ajax({
            url: "/pages/notifier.php?p=<?php echo $dflag; ?>",
            cache: false,
            success: function(html) {
                $('#notifier').html(html);
                setTimeout(function() {
                    refresh();
                }, interval);
            }
        });
    };
    refresh();
});
</script>
<?php if ( $page != "dispatcher" ) { ?>

<div id="body" style="height:100%;width:700px;position:absolute;margin-top:10px;margin-left:12.50em;">
<?php } else { ?>
<div id="body" style="height:100%;width:700px;position:absolute;margin-top:10px;margin-left:0.50em;">
<?php } ?>
