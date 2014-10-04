<? defined('DS') OR die('No direct access allowed.'); ?>
</div>

<div id="meta" class="dark">
	<ul>
		<li><a href="http://www.ixsystems.com" id="copyright" title="Created by iXsystems 2014">iXsystems 2014</a></li>
		<li>Version: .001</li>
		<li></li>
	</ul>
</div>

<script src="js/main.js"></script>
<script src="js/doubletaptogo.js"></script>
<script>
	$( function()
	{
		$( '#nav li:has(ul)' ).doubleTapToGo();
	});
	$( function()
	{
		$( '#installwidget li:has(ul)' ).doubleTapToGo();
		return false;
	});
</script>

</div><!----End of container---->

</body>
</html>
