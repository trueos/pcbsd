<?
  require("include/Mobile_Detect.php");
  require("include/globals.php");
  require("include/functions.php");

  // Figure out what page is being requested
  if ( empty($_GET["p"]))
     $page = "main";
  else
     $page = $_GET["p"];

  // Set some globals for mobile detection
  $detect = new Mobile_Detect;
  $deviceType = ($detect->isMobile() ? ($detect->isTablet() ? 'tablet' : 'phone') : 'computer');
  $scriptVersion = $detect->getScriptVersion();

  if ( $deviceType == "computer" )
  {
    require("include/header-computer.php");
    require("include/nav-computer.php");
  } else {
    require("include/header-phone.php");
    require("include/nav-phone.php");
  }

  // Does the page exist?
  if ( file_exists("pages/$page.php") === false ) {
    require("pages/error.php");
  } else {
    require("pages/$page.php");
  }


  if ( $deviceType == "computer" )
    require("include/footer-computer.php");
  else
    require("include/footer-phone.php");
?>
