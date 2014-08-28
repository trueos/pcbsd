<html>
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
      if ( $results[2] == "pkgupdate" ) {
        $target=$results[3];
	if ( $results[3] == "__system__" )
            $target="Local System";
        $result = "Updated packages on: ". $target;
      } elseif ( $results[4] == "install" ) {
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
    echo "<META HTTP-EQUIV=\"refresh\" CONTENT=\"6\">";
    $output="$narray[0]";
  }
?>
<body style="background-color:<? echo $bgcolor; ?>;">
<? echo "$output"; ?>
</body>
</html>
