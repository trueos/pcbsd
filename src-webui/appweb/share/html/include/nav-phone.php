<div id="container" style="width:100%;height:100%;">

<nav id="nav" role="navigation">
	<a href="#nav" title="Show navigation">Show navigation</a>
	<a href="#" title="Hide navigation">Hide navigation</a>
	<ul class="clearfix">
		<li><a href="/">Home</a></li>
		<li>
			<a href="/?p=appcafe&jail=<? echo "$jailUrl"; ?>" aria-haspopup="true"><span><img src="/images/appcafe.png" height=24 width=24>AppCafe</span></a>
			<ul>
				<li><a href="/?p=sysapp&jail=<? echo "$jailUrl"; ?>"><img src="/images/install.png" height=24 width=24> Installed Applications</a></li>
				<li><a href="/?p=appcafe-browse&jail=<? echo "$jailUrl"; ?>"><img src="/images/categories.png" height=24 width=24> Browse Categories</a></li>
				<li><a href="/?p=appcafe"><img src="/images/switch.png" height=24 width=24> Change System / Jail</a></li>
			</ul>
		</li>
		<li>
			<a href="/?p=jails" aria-haspopup="true"><span><img src="/images/warden.png" height=24 width=24> Jails</span></a>
			<ul>
				<li><a href="/?p=jails"><img src="/images/jail.png" height=24 width=24> Browse Jails</a></li>
				<li><a href="/?p=jailcfg"><img src="/images/configure.png" height=24 width=24> Configure</a></li>
			</ul>
		</li>
	</ul>
</nav>

<body>

<div id="notifer" style="height:35px;width:300px;position:absolute;margin-top:0em;margin-left:50px;">
  <iframe src="pages/notifier.php" height="35px" width="264" seamless></iframe><a href="?p=dispatcher"><img style="vertical-align:top" height=35 width=35 src="images/notifier.png" border=0></a>
</div>

<div id="body" style="height:100%;width=100%;float:left;margin-top:6.00em;margin-left:5px;margin-right:5px;">
