<?
  require("include/functions.php");
  require("include/header.html");
  require("include/nav.html");

  // Figure out what page is being requested
  if ( empty($_GET["p"]))
     $page = "main";
  else
     $page = $_GET["p"];
 
  // Does the page exist?
  if ( file_exists("include/$page.php") === false ) {
    echo ("<h1>Invalid page specified!</h1>\n");
  } else {
    // Load the page now
    require("include/$page.php");
  }

  require("include/footer.html");
?>
