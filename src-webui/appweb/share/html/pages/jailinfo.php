<?
defined('DS') OR die('No direct access allowed.');

  if ( empty($jail) or $jail == "#system" )
     die("Invalid jail specified!");

  exec("$sc ". escapeshellarg("jail $jail ipv4")
       . " " . escapeshellarg("jail $jail ipv6")
       , $jailinfo);
  $jailipv4 = $jailinfo[0];
  $jailipv6 = $jailinfo[1];

?>


<h1>Jail Configuration for <? echo "$jail"; ?>
<button title="Delete <? echo "$jail"; ?>" style="background-color: Transparent;background-repeat:no-repeat;float:right;border: none;" onclick="delJailConfirm('<? echo "$jail"; ?>')"><img src="/images/remove.png" height=48 width=48></button>
</h1>
<p><? echo "$jailMsg"; ?></p>
<table class="jaillist" style="width:100%">
<tr>
   <th></th>
   <th></th>
</tr>

<form method="post" action="?p=jailinfo&jail=<? echo "$jailUrl"; ?>">
<tr>
  <td>Jail Nickname</td>
  <td><? echo "$jail"; ?></td>
</tr>
<tr>
  <td>Jail IPv4 Address</td>
  <td><input name="jailipv4" type="text" value="<? echo "$jailipv4"; ?>" /></td>
</tr>
<tr>
  <td colspan="2"><input name="submit" type="submit" value="Save" /></td>
</tr>

</form>

</table>
