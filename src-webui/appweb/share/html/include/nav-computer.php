<?php defined('DS') OR die('No direct access allowed.'); ?>
<div id="container" style="width:100%;height:100%;" taborder="0">

<?php
   if ( strpos($page, "jail") === false )
   {
   //style="background-color: #e15a1f;height:3.00em;width:100%;position:absolute;margin-top:0.00em;margin-left:0.00em;align:left">
?>
<div id="cssmenu" style="height:3.50em;">
<ul>
  <li <?php if ( $page=="appcafe" ) { echo "class='active'"; } ?>><a href="/?p=appcafe&jail=<?php echo "$jailUrl"; ?>"><img src="/images/appcafe.png" height=32 width=32> &nbsp;&nbsp;Home</a></li>
  <li <?php if ( $page=="sysapp" ) { echo "class='active'"; } ?>><a href="/?p=sysapp&jail=<?php echo "$jailUrl"; ?>"><img src="/images/install.png" height=32 width=32> &nbsp;&nbsp;Installed Apps</a></li>
  <li <?php if ( $page=="appcafe-search" ) { echo "class='active'"; } ?>><a href="/?p=appcafe-search&jail=<?php echo "$jailUrl"; ?>"><img src="/images/search.png" height=32 width=32> &nbsp;&nbsp;App Search</a></li>
  <li><a href="/?p=jails" style="align:right;"><img src="/images/warden.png" height=32 width=32> &nbsp;&nbsp;Jails</a></li>
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
?>
<div id="cssmenu" style="height:3.50em;">
<ul>
  <li <?php if ( $page=="jails" ) { echo "class='active'"; } ?>><a href="/?p=jails"><img src="/images/jail.png" height=32 width=32> &nbsp;&nbsp;Home</a></li>
  <li <?php if ( $page=="jailcreate" ) { echo "class='active'"; } ?>><a href="/?p=jailcreate"><img src="/images/add.png" height=32 width=32> &nbsp;&nbsp;Create Jail</a></li>
  <li <?php if ( $page=="appcafe" ) { echo "class='active'"; } ?>><a href="/?p=appcafe&jail=<?php echo "$jailUrl"; ?>"><img src="/images/appcafe.png" height=32 width=32> &nbsp;&nbsp;AppCafe</a></li>
<?php
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
            url: "/pages/notifier.php",
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
