<?php defined('DS') OR die('No direct access allowed.'); ?>
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

<?php

// Close websocket connection to syscache
$scclient->send("", "close");

?>
