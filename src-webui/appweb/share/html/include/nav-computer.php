<div id="container" style="width:100%;height:100%;">

<nav id="nav" role="navigation">
	<a href="#nav" title="Show navigation">Show navigation</a>
	<a href="#" title="Hide navigation">Hide navigation</a>
	<ul class="clearfix">
		<li>
			<a href="/?p=appcafe&jail=<? echo "$jailUrl"; ?>" aria-haspopup="true"><span><img src="/images/appcafe.png" height=48 width=48> AppCafe</span></a>
                        <ul>
                                <li><a href="/?p=sysapp&jail=<? echo "$jailUrl"; ?>"><img src="/images/install.png" height=32 width=32> Installed Applications</a></li>
                                <li><a href="/?p=appcafe"><img src="/images/switch.png" height=32 width=32> Change System / Jail</a></li>
                        </ul>

		</li>
		<li>
			<a href="/?p=jails" aria-haspopup="true"><span><img src="/images/warden.png" height=48 width=48> Warden</span></a>
                        <ul>
                                <li><a href="/?p=jails"><img src="/images/jail.png" height=24 width=24> Browse Jails</a></li>
                                <li><a href="/?p=jailcfg"><img src="/images/configure.png" height=24 width=24> Configure</a></li>
                        </ul>

		</li>
	</ul>
</nav>

<body>
<div id="leftmenu" style="height:100%;width:12.00em;position:absolute;margin-top:4.00em;marin-left:10px;">
  <? 
     require("include/leftmenu.php");
  ?>

</div>

<div id="notifer" style="height:35px;width:600px;position:absolute;margin-top:3.80em;margin-left:12.50em;">
  <iframe scrolling="no" src="pages/notifier.php" height="35" width="565" seamless></iframe><a href="?p=dispatcher" title="View recent actions and results"><img style="vertical-align:top" height=34 width=34 src="images/notifier.png" border=0></a>
</div>

<div id="body" style="height:100%;width:700px;position:absolute;margin-top:6.50em;margin-left:12.50em;">
