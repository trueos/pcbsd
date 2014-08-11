<?
  // Page to display left menu for appcafe / jails
  // Will need a major visual upgrade

  if ( $page == "appcafe" or $page == "sysapp" or $page == "appinfo" or $page == "dispatcher" ) {
?>

<a href="?p=sysapp">System Apps</a>
<br><br>

<?
  exec("$sc ". escapeshellarg("pbi list cats"), $catarray);
  $catlist = explode(", ", $catarray[0]);
  foreach ( $catlist as $cat ) {
    if ( empty($cat) )
      continue;
    exec("$sc ". escapeshellarg("pbi cat $cat name"). " " . escapeshellarg("pbi cat $cat icon"). " " . escapeshellarg("pbi cat $cat comment"), $catdetails);
    echo "<img height=32 width=32 src=\"images/pbiicon.php?i=$catdetails[1]\"><a href=\"?p=appcafe&cat=$cat\" title=\"$catdetails[2]\">$catdetails[0]</a><br>";
    unset($catdetails);
  }

?>

 
<?
  } else {
?>

JailMenu

<? 
  }
?>
