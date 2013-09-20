<?php

require("config.php");

// Remove a mac address sym-link
function rem_mac($mac)
{
  global $sdir;
  chdir($sdir);
  unlink($mac);
}

// Function to do the creation of the mac -> file symlink
function doadd_mac()
{
  global $sdir;
 
  if ( ! isset($_POST['macaddress']) or ! isset($_POST['cfgfile']) )
     die("Unset vars! Did you enter a mac address?");

  $mac=$_POST['macaddress'];
  $cfg=$_POST['cfgfile'];
  chdir($sdir);
  symlink($cfg, $mac . ".cfg");
  
}

// Function to add a mac address sym-link -> config file
function add_mac()
{
  global $sdir;

  echo "<a href=\"?view=clients\">Back</a><br>";
  echo "<br>";
  echo "Please enter the MAC address, and select a config to install with:<br>";

  echo "<form method=\"post\" action=\"?view=clients&doadd=true\">\n";
  echo "Mac Address: <input type=\"text\" name=\"macaddress\" size=\"50\"><br>\n";
  echo "Config File: <select name=cfgfile>\n";

  chdir($sdir);
  $dir_handle = @opendir($sdir) or die("Unable to open $sdir");

  //running the while loop
  while ($file = readdir($dir_handle))
  {
     if ( "$file" == ".." or "${file}" == "." or is_dir($file) or is_link($file))
	continue;
     echo "<option value=\"$file\">$file</option>";
  }

  echo "</select><br><br>\n";
  echo "<input type=\"submit\" name=\"Save\" value=\"Add MAC\">";
  echo "</form>\n";

}

// Function to list mac address sym-links in the scripts dir
function list_mac()
{
  global $sdir;

  // Display client MAC address scripts
  chdir($sdir);
  $dir_handle = @opendir($sdir) or die("Unable to open $sdir");

  $bgcolor = "#BDBDBD";

  echo "<a href=\"?view=clients&add=true\">Add a MAC address</a><br>";
  echo "<table border=\"0\" cellspacing=\"0\"><tr bgcolor=\"#1A73CC\"><td><b>MAC Address</b></td><td></td><td><b>Config File</b></td><td></td></tr><br>";

  //running the while loop
  while ($file = readdir($dir_handle))
  {
     if ( "$file" == ".." or "${file}" == "." or is_dir($file) or ! is_link($file))
	continue;

     // Get the file sym-link target
     $cfg = readlink($file);

     $mac = strstr($file, '.cfg', true);

     echo "<tr bgcolor=\"$bgcolor\" border=0><td>$mac</td><td>&nbsp;<b>></b>&nbsp;</td><td>$cfg</td><td>&nbsp; - <a href=\"?view=clients&del=$file\">Remove</a></td>";

     if ( $bgcolor == "#BDBDBD" )
       $bgcolor = "#FFFFFF";
     else
       $bgcolor = "#BDBDBD";
  }

  echo "</table>";

} // End of list MAC addreses

// Function to list cfg files
function list_cfgs()
{
  global $sdir;

  // Display cfg files
  chdir($sdir);
  $dir_handle = @opendir($sdir) or die("Unable to open $sdir");

  $bgcolor = "#BDBDBD";

  echo "<a href=\"?view=configs&add=true\">Add a Config</a><br>";
  echo "<table border=\"0\" cellspacing=\"0\"><tr bgcolor=\"#1A73CC\"><td><b>Config File</b></td><td></td><td></td></tr><br>";

  //running the while loop
  while ($file = readdir($dir_handle))
  {
     if ( "$file" == ".." or "${file}" == "." or is_dir($file) or is_link($file))
	continue;

     // Get the file sym-link target
     $config = strstr($file, '.cfg', true);

     echo "<tr bgcolor=\"$bgcolor\" border=0><td>$config</td><td><a href=\"?view=configs&edit=$file\">Edit</a></td><td>&nbsp; - <a href=\"?view=configs&del=$file\">Remove</a></td>";

     if ( $bgcolor == "#BDBDBD" )
       $bgcolor = "#FFFFFF";
     else
       $bgcolor = "#BDBDBD";
  }

  echo "</table>";

} // End of list config files

// Function to add the new config file
function doadd_cfg()
{
  global $sdir;

  chdir($sdir);
  $uploadfile = $sdir . "/" . basename($_FILES['usercfg']['name']);

  if (! move_uploaded_file($_FILES['usercfg']['tmp_name'], $uploadfile))
     die("Failed uploading config file");
}


function add_cfg()
{
  echo "<form enctype=\"multipart/form-data\" action=\"?view=configs&doadd=true\" method=\"POST\">";
  echo "<input type=\"hidden\" name=\"MAX_FILE_SIZE\" value=\"30000\" />";
  echo "Upload this config: <input name=\"usercfg\" type=\"file\" /><br>";
  echo "<input type=\"submit\" value=\"Upload Config\" /><br>";
  echo "</form>";
}


// Function to add a config file
function edit_cfg($cfg)
{
  global $sdir;

  // Get the file sym-link target
  $confignick = strstr($cfg, '.cfg', true);

  echo "<a href=\"?view=configs\">Back</a><br>";
  echo "<br>";
  echo "Editing the config: <b>$confignick</b><br>";

  echo "<form method=\"post\" action=\"?view=configs&doedit=$cfg\">\n";
  echo "<textarea name=\"script\" cols=\"100\" rows=\"25\">"; 
  readfile($sdir . "/" .  $cfg);
  echo "</textarea><br>\n";
  echo "<input type=\"submit\" name=\"Save\" value=\"Save Config\">";
  echo "</form>";

}

// Remove a mac address sym-link
function rem_cfg($cfg)
{
  global $sdir;
  chdir($sdir);
  unlink($cfg);
}

// Function to edit the config file
function doedit_cfg($cfg)
{
  global $sdir;
 
  if ( ! isset($_POST['script']) )
     die("Unset vars! Did you not enter an install script?");

  $contents=$_POST['script'];
  $contents=str_replace("\r\n", "\n", $contents);
  $handle = fopen($sdir . "/" . $cfg, "w");
  fwrite($handle, $contents);
}

?>
