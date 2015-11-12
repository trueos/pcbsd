<?php defined('DS') OR die('No direct access allowed.'); ?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Frameset//EN"
   "http://www.w3.org/TR/html4/frameset.dtd">

<html lang="en">

<head>
	<meta charset="utf-8" />
	<title>AppCafe - <?php echo "$jail"; ?></title>
	<meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
	<meta name="description" content="AppCafe" />
	<meta name="robots" content="all">
	<meta name="viewport" content="width=device-width,initial-scale=1" />
	<link rel="stylesheet" href="/css/_reset.css" />
	<link rel="stylesheet" href="/css/main.css" />
	<link rel="stylesheet" href="/css/tabs.css" />
        <link rel="stylesheet" href="/css/popbox.css" />
        <link rel="stylesheet" href="/css/lightbox.css" />
        <link rel="stylesheet" href="/css/cssmenu.css" />
        <link rel="stylesheet" href="/css/subcssmenu.css" />
        <link rel="stylesheet" href="/css/thickbox.css" />
        <link rel="stylesheet" href="/css/tooltipster.css" />
	<link rel="stylesheet" href="http://fonts.googleapis.com/css?family=Open+Sans:400">
<?php
print "<script type=\"text/javascript\">\n";
print " var wsserver=\"" . $wsremote . "\";\n";
print " var wstoken=\"" . $_SESSION['apikey'] . "\";\n";
print "</script>\n";
?>
	<script type="text/javascript" src="/js/jquery.min.js"></script>
	<script type="text/javascript" src="/js/jquery.tooltipster.min.js"></script>
	<script type="text/javascript" src="/js/notifications.js"></script>
	<script type="text/javascript" src="/js/appcafe.js"></script>
	<script type="text/javascript" src="/js/jquery.easytabs.min.js"></script>
	<script type="text/javascript" src="/js/jquery.jscroll.min.js"></script>
	<script type="text/javascript" src="/js/jquery.ba-hashchange.min.js"></script>
	<script type="text/javascript" src="/js/jquery.timer.js"></script>
	<script type="text/javascript" src="/js/popbox.min.js"></script>
	<script type="text/javascript" src="/js/lightbox.min.js"></script>
	<script type="text/javascript" src="/js/cssmenu.js"></script>
	<script type="text/javascript" src="/js/thickbox-compressed.js"></script>
	<style>
	<style>

		body
		{
			background-color: #f7efeb;
			padding: 5.25em; /* 20 */
		}

		#nav
		{
			width: 100%;
			font-family: 'Open Sans', sans-serif;
			font-weight: 400;
			font-size: 80%;
			position: absolute;
			top: 0%;
			left: 0%;
			margin-left: 0px;
			z-index:1550;
		}

			#nav > a
			{
				display: none;
			}

			#nav li
			{
				position: relative;
			}
				#nav li a
				{
					color: #fff;
					display: block;
				}
				#nav li a:active
				{
					background-color: #c00 !important;
				}

			#nav span:after
			{
				width: 0;
				height: 0;
				border: 0.313em solid transparent; /* 5 */
				border-bottom: none;
				border-top-color: #efa585;
				content: '';
				vertical-align: middle;
				display: inline-block;
				position: relative;
				right: -0.313em; /* 5 */
			}

			/* first level */

			#nav > ul
			{
				height: 3.75em; /* 60 */
				background-color: #e15a1f;
				z-index:1500;
			}
				#nav > ul > li
				{
					width: 50%;
					height: 100%;
					float: left;
					z-index:1500;
				}
					#nav > ul > li > a
					{
						height: 100%;
						font-size: 1.5em; /* 24 */
						line-height: 2.5em; /* 60 (24) */
						text-align: center;
						z-index:1500;
					}
						#nav > ul > li:not( :last-child ) > a
						{
							border-right: 1px solid #cc470d;
						}
						#nav > ul > li:hover > a,
						#nav > ul:not( :hover ) > li.active > a
						{
							background-color: #cc470d;
							z-index:1500;
						}


				/* second level */

				#nav li ul
				{
					background-color: #cc470d;
					display: none;
					position: absolute;
					top: 100%;
				}
					#nav li:hover ul
					{
						display: block;
						left: 0;
						right: 0;
					}
						#nav li:not( :first-child ):hover ul
						{
							left: -1px;
						}
						#nav li ul a
						{
							font-size: 1.25em; /* 20 */
							border-top: 1px solid #e15a1f;
							padding: 0.75em; /* 15 (20) */
						}
							#nav li ul li a:hover,
							#nav li ul:not( :hover ) li.active a
							{
								background-color: #e15a1f;
							}


		@media only screen and ( max-width: 62.5em ) /* 1000 */
		{
			#nav
			{
				width: 100%;
				position: static;
				margin: 0;
				z-index:1500;
			}
		}

		@media only screen and ( max-width: 40em ) /* 640 */
		{
			html
			{
				font-size: 100%; /* 12 */
			}

			#nav
			{
				position: absolute;
				margin-top: 0px;
				top: auto;
				left: auto;
			}
				#nav > a
				{
					width: 3.125em; /* 50 */
					height: 3.125em; /* 50 */
					text-align: left;
					text-indent: -9999px;
					background-color: #e15a1f;
					position: relative;
				}
					#nav > a:before,
					#nav > a:after
					{
						position: absolute;
						border: 2px solid #fff;
						top: 35%;
						left: 25%;
						right: 25%;
						content: '';
					}
					#nav > a:after
					{
						top: 60%;
					}

				#nav:not( :target ) > a:first-of-type,
				#nav:target > a:last-of-type
				{
					display: block;
				}


			/* first level */

			#nav > ul
			{
				height: auto;
				display: none;
				position: absolute;
				left: 0;
				right: 0;
			}
				#nav:target > ul
				{
					display: block;
				}
				#nav > ul > li
				{
					width: 100%;
					float: none;
				}
					#nav > ul > li > a
					{
						height: auto;
						text-align: left;
						padding: 0 0.833em; /* 20 (24) */
					}
						#nav > ul > li:not( :last-child ) > a
						{
							border-right: none;
							border-bottom: 1px solid #cc470d;
						}


				/* second level */

				#nav li ul
				{
					position: static;
					padding: 1.25em; /* 20 */
					padding-top: 0;
				}
		}

	</style>
    <script>
var $jq = jQuery.noConflict();
$jq(document).ready(function() {
  $jq('.tooltip').tooltipster();
});
</script>
</head>
