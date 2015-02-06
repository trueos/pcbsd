<? defined('DS') OR die('No direct access allowed.'); ?>
<div id="container" style="width:100%;height:100%;" taborder="0">

<?
   if ( strpos($page, "jail") === false )
   {
   //style="background-color: #e15a1f;height:3.00em;width:100%;position:absolute;margin-top:0.00em;margin-left:0.00em;align:left">
?>
<div id="cssmenu" style="height:3.50em;">
<ul>
  <li <? if ( $page=="appcafe" ) { echo "class='active'"; } ?>><a href="/?p=appcafe&jail=<? echo "$jailUrl"; ?>"><img src="/images/appcafe.png" height=32 width=32> Home</a></li>
  <li <? if ( $page=="sysapp" ) { echo "class='active'"; } ?>><a href="/?p=sysapp&jail=<? echo "$jailUrl"; ?>"><img src="/images/install.png" height=32 width=32> Installed Apps</a></li>
  <li <? if ( $page=="appcafe-search" ) { echo "class='active'"; } ?>><a href="/?p=appcafe-search&jail=<? echo "$jailUrl"; ?>"><img src="/images/search.png" height=32 width=32> App Search</a></li>
  <li><a href="/?p=jails" style="align:right;"><img src="/images/warden.png" height=32 width=32> Warden</a></li>
<?
if (USERNAME)
  echo "  <li><a href=\"/?logout=true\"><img src=\"/images/logout.png\" height=32 width=32> Logout</a></li>";
?>
</ul>
</div>

<?
} else {
  // Jail menus
?>
<div id="cssmenu" style="height:3.50em;">
<ul>
  <li <? if ( $page=="jails" ) { echo "class='active'"; } ?>><a href="/?p=jails"><img src="/images/jail.png" height=32 width=32> Home</a></li>
  <li <? if ( $page=="jailcreate" ) { echo "class='active'"; } ?>><a href="/?p=jailcreate"><img src="/images/add.png" height=32 width=32> Create Jail</a></li>
  <li <? if ( $page=="appcafe" ) { echo "class='active'"; } ?>><a href="/?p=appcafe&jail=<? echo "$jailUrl"; ?>"><img src="/images/appcafe.png" height=32 width=32> AppCafe</a></li>
<?
if (USERNAME)
  echo "  <li><a href=\"/?logout=true\"><img src=\"/images/logout.png\" height=32 width=32> Logout</a></li>";
?>
</ul>
</div>

<?
}
?>

<body>
<? if ( $page != "dispatcher" ) { ?>
<div id="leftmenu" style="height:100%;width:12.00em;position:absolute;margin-top:10px;marin-left:10px;">
  <? 
     require("include/leftmenu.php");
  ?>
</div>
<? } ?>

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
<? if ( $page != "dispatcher" ) { ?>
<div id="notifier" style="height:35px;width:600px;position:absolute;margin-top:10px;margin-left:12.50em;"></div>

<div id="body" style="height:100%;width:700px;position:absolute;margin-top:55px;margin-left:12.50em;">
<? } else { ?>
<div id="body" style="height:100%;width:700px;position:absolute;margin-top:10px;margin-left:0.50em;">
<? } ?>
