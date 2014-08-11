<?
  require("../include/functions.php");
  $bgcolor="white";

  // Command to prod dispatcher for current status
  // Eventually we will pep this up with extra parsing about
  // the status
  $narray = run_cmd("status");
  if ( $narray[0] == "Idle" or empty($narray[0]) ) {
    $rarray = run_cmd("results");
    $lastElement=count($rarray);
    $lastElement--;
    if ( ! empty($rarray[$lastElement]) ) {
      $results = explode(" ", $rarray[$lastElement]);
      if ( $results[4] == "install" ) {
        $result = "Installed ". $results[3];
      } else {
        $result = "Removed ". $results[3];
      }

      if ( $results[0] == "SUCCESS" )
	$result = "<img align=absmiddle height=30 width=30 src=\"../images/dialog-ok.png\">".$result;
      else
	$result = "<img align=absmiddle height=30 width=30 src=\"../images/application-exit.png\">".$result;
    }
    $output="$result";
  } else {
    $output="$narray[0]";
  }
?>
<html>
<META HTTP-EQUIV="refresh" CONTENT="6">
<body style="background-color:<? echo $bgcolor; ?>;">
<? echo "$output"; ?>
</body>
</html>
