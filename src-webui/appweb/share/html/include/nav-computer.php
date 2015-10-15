<?php
defined('DS') OR die('No direct access allowed.');

 $refPage = $_GET["ref"];
 $pluginDispatcher = false;
 $jailDispatcher = false;
 if ( stripos($refPage, "plugin") !== false )
   $pluginDispatcher = true;
 if ( stripos($refPage, "jail") !== false )
   $jailDispatcher = true;

if ( strpos($page, "plugin") !== false or $pluginDispatcher) {
  $dflag="plugins";
} elseif ( strpos($page, "jail") !== false or $jailDispatcher) {
  $dflag="jails";
} else {
  $dflag="appcafe";
}
?>
<div id="container" style="width:100%;height:100%;" taborder="0">

<div id="cssmenu" style="height:60px">
<ul>

<?php
  if ( $sysType != "APPLIANCE" ) {
?>
  <li <?php if ( $page=="appcafe" ) { echo "class='active'"; } ?>><a href="/?p=appcafe"><img src="/images/appcafe.png" height=28 width=28> &nbsp;&nbsp;System Packages</a></li>
<?php
  }
?>
  <li <?php if ( stripos($page, "plugin") !== false ) { echo "class='active'"; } ?>><a href="/?p=plugins" style="align:right;"><img src="/images/warden.png" height=28 width=28> &nbsp;&nbsp;App Containers</a></li>
  <li <?php if ( stripos($page, "jail") !== false ) { echo "class='active'"; } ?>><a href="/?p=jails" style="align:right;"><img src="/images/jail.png" height=28 width=28> &nbsp;&nbsp;Jails</a></li>
  <li id="notifier"><a href="?p=dispatcher"><img align=absmiddle height=32 width=32 src="../images/dialog-ok.png"> Status</a></li>
<?php
if (USERNAME)
  echo "  <li><a href=\"/?logout=true\"><img src=\"/images/logout.png\" height=28 width=28> Logout</a></li>";
?>
</ul>
</div>
<div style="height:2px;">
&nbsp;
</div>
<div id="subcssmenu" style="height:2.30em;">
<ul>
<?php
if ( $dflag == "appcafe" )
{
?>
  <li <?php if ( $page=="sysapp" ) { echo "class='active'"; } ?>><a href="/?p=sysapp"><img src="/images/install.png" height=24 width=24> &nbsp;&nbsp;Installed Apps</a></li>
  <li <?php if ( $page=="appcafe-search" ) { echo "class='active'"; } ?>><a href="/?p=appcafe-search"><img src="/images/search.png" height=24 width=24> &nbsp;&nbsp;App Search</a></li>
<?php
} elseif ( $dflag == "plugins" )
{
?>
  <li><a href="/?p=sysplugins"><img src="/images/install.png" height=24 width=24> &nbsp;&nbsp;Installed Apps</a></li>
  <li><a href="/?p=pluginconfig"><img src="/images/configure.png" height=24 width=24> &nbsp;&nbsp;Configuration</a></li>
<?php
} elseif ( $dflag == "jails" )
{
?>
  <li><a href="/?p=jailcreate"><img src="/images/add.png" height=24 width=24> &nbsp;&nbsp;New Jail</a></li>
  <li><a href="/?p=jailconfig"><img src="/images/configure.png" height=24 width=24> &nbsp;&nbsp;Configuration</a></li>
<?php
}
?>
</ul>
</div>


<body>
<?php if ( $page != "dispatcher" and stripos($page, "jail") === false ) { ?>
<div id="leftmenu" style="height:100%;width:140px;position:absolute;margin:16px;">
  <?php 
     require("include/leftmenu.php");
  ?>
</div>
<?php } ?>

<?php if ( $page != "dispatcher" and stripos($page, "jail") === false )  { ?>

<div id="body" style="height:100%;width:700px;position:absolute;margin-top:10px;margin-left:200px;">
<?php } else { ?>
<div id="body" style="height:100%;width:700px;position:absolute;margin-top:10px;margin-left:0.50em;">
<?php } ?>
