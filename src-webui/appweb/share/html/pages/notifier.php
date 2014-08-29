<?
  require("../include/globals.php");
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
	$result = "<img align=absmiddle height=40 width=40 src=\"../images/dialog-ok.png\">".$result;
      else
	$result = "<img align=absmiddle height=40 width=40 src=\"../images/application-exit.png\">".$result;
    }
    $output="$result";
  } else {
    $output="Idle";
  }

  $pkgUpdates=false;
  // Check if we can prod local system for updates
  $larray = array();
  if ( $sysType != "APPLIANCE" )
     $larray[] = "#system";

  // Build list of jails
  $jailoutput = get_jail_list();
  $running=$jailoutput[0];
  $rarray = explode( ", ", $running);

  // Combine the arrays
  $carray = array_merge($larray, $rarray);

  foreach ($carray as $jname) {
    if ( empty($jname) )
       continue;

    unset($jarray);
    exec("$sc ". escapeshellarg("pkg ". $jname . " hasupdates"), $jarray);
    $hasupdates=$jarray[0];
 
    if ( $hasupdates == "true" ) {
       $pkgUpdates=true;
       break;
    }
  }

  // We have updates! Show the notification icon
  if ( $pkgUpdates )
    $output=$output . " <img src=\"/images/warning.png\" height=35 width=34 title=\"Updates are available!\">";

?>
<a href="?p=dispatcher"><? echo "$output"; ?></a>
