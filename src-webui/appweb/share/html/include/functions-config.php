<?php

defined('DS') OR die('No direct access allowed.');

// Display a input = number type box
function display_number_box($cfg)
{
  global $currentval;
  global $desc;
  global $longdesc;
  global $default;
  global $postkey;
  global $itemup;

  $min = $cfg['min'];
  $max = $cfg['max'];
  echo "  <tr>\n";
  echo "    <td><input type=\"number\" title=\"$longdesc\" name=\"$postkey\" min=\"$min\" max=\"$max\" value=\"$currentval\"></td>\n";
  echo "    <td title=\"$longdesc\">$desc$itemup</td>\n";
  echo "  </tr>\n";
}

// Display a option box
function display_combo_box($cfg)
{
  global $currentval;
  global $desc;
  global $longdesc;
  global $default;
  global $postkey;
  global $itemup;

  echo "  <tr>\n";
  echo "    <td><select title=\"$longdesc\" name=\"$postkey\">";
  $i=1;
  for ( ;; ) {
    $akey = "option" . $i;
    if ( empty($cfg[$akey]) )
       break;
    unset($ops);
    $ops = explode("::::", $cfg[$akey]);
    $option = $ops[0];
    $disp = $ops[1];
    $selected="";
    if ( $option == $currentval )
      $selected="selected";
    echo "      <option value=\"$option\" $selected>$disp</option>\n";
    $i++;
  }
  echo "    </select></td>\n";
  echo "    <td title=\"$longdesc\">$desc$itemup</td>\n";
  echo "  </tr>\n";
}

// Display a string/password input box
function display_string_box($cfg)
{
  global $currentval;
  global $desc;
  global $longdesc;
  global $default;
  global $postkey;
  global $itemup;

  $maxlen = $cfg['maxlen'];

  if ( empty($maxlen) )
     $maxlen="20";

  $type = "text";
  if ( $cfg['type'] == "PASSWORDBOX" )
     $type = "password";

  echo "  <tr>\n";
  echo "    <td><input type=\"$type\" title=\"$longdesc\" name=\"$postkey\" value=\"$currentval\" maxlength=\"$maxlen\"></td>\n";
  echo "    <td title=\"$longdesc\">$desc$itemup</td>\n";
  echo "  </tr>\n";
}

?>
