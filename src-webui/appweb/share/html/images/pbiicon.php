<?


function LoadPNG($imgname)
{
    /* Attempt to open */
    $im = @imagecreatefrompng($imgname);

    /* See if it failed */
    if(!$im)
      $im = @imagecreatefrompng("/usr/local/share/pbi-manager/icons/default.png");

    imageAlphaBlending($im, true);
    imageSaveAlpha($im, true);

    return $im;
}

header('Content-Type: image/png');
$ipath=$_GET['i'];
$img = LoadPNG($ipath);
imagepng($img);
imagedestroy($img);

?>
