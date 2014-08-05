<?
  // Page to display left menu for appcafe / jails
  // Will need a major visual upgrade

  if ( $page == "appcafe" or $page == "sysapp" or $page == "sysappinfo" ) {
?>

<a href="?p=sysapp">System Apps</a>
<br><br>

<?
  $sc="pbi list cats";
  exec("/usr/local/bin/syscache ". escapeshellarg("$sc"), $catarray);
  $catlist = explode(", ", $catarray[0]);
  foreach ( $catlist as $cat ) {
    if ( empty($cat) )
      continue;
    echo "<a href=\"$?p=appcafe&cat=$cat\">$cat</a><br>";
  }

?>

 
<?
  } else {
?>

JailMenu

<? 
  }
?>
