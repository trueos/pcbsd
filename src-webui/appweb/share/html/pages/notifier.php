<?
  require("../include/functions.php");

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
      $result=$results[0] . " ". $results[4] . " " . $results[3];
    } else {
      $result="$cmd";
    }

    $output="$result";

  } else {
    $output="$narray[0]";
  }
?>
<html>
<META HTTP-EQUIV="refresh" CONTENT="6">
<body style="background-color:yellow;">
<? echo "$output"; ?>
</body>
</html>
