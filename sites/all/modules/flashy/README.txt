ABOUT

This module provides a wraper for the Flashy video player:

http://code.google.com/p/flashy/

The base module depends on SWFEmbed, and provides an object-oriented API for
embedding flash videos in your site.  If you have the getID3() module, it will
also try to auto-detect the size of the video if you do not specify an explicit
size for the player.

The included flashy_filefield and flashy_link modules provide CCK formatters
for the filefield and link CCK fields, respectively.  That allows you to upload
a .flv or other supported file format into a CCK-based node, configure a formatter,
and have it "just work" to display the video.  Referencing linked 3rd party videos
using the link field is also supported.  Additional configuration options will
be added if there is demand (and if people submit patches!)

REQUIREMENTS

- Drupal 6.x
- Autoload module: http://drupal.org/project/autoload
- SWFEmbed module: http://drupal.org/project/swfembed
- Flashy video player: http://code.google.com/p/flashy/

The filefield and link formatters, of course, also require CCK and the appropriate
CCK field module.

INSTALLATION

- Copy the flashy directory to your modules directory.
- Download the videoPlayer.swf file from the Flashy project on Google Code.  Place
it in the main flashy module directory.
- Go to admin/build/modules and enable the necessary modules.

DEVELOPING WITH FLASHY

This module is intended for developer use.  It provides a chainable OO API,
inherited from SWFEmbed, for creating and rendering video players.  See the
SWFEmbed handbook page for basic usage:

http://drupal.org/node/406560

Additional methods are also provided for Flash playing specifically.  They are all
heavily documented inline.  A brief sample follows:

  $flashy = flashy_create()
    ->addMedia($path)         // Add a single file to be played.
    ->flashVar('volume', 50)
    ->flashVar('loop', TRUE)
    ->flashVar('autoPlay', TRUE)
    ->flashVar('uiControls', 'PlayPause|Prev|Next|Seek|Volume|FullScreen|MuteUnmute')
    ->param('allowFullScreen', 'true')
    ->noFlash('Flash is not working :-(');
  $output .= $flashy->render();

  $flashy = flashy_create()
    ->addMedia($path)
    ->flashVar('volume', 20)
    ->height(100)
    ->width(400)
    ->param('allowFullScreen', 'true')
    ->noFlash('Flash is not working :-(');
  $output .= $flashy->render();

  $path = 'path/to/playlist.xml';
  $path = url($path, array('absolute' => TRUE));
  $flashy = new Flashy();
  $flashy
    ->setPlaylist($path)
    ->flashVar('volume', 80)
    ->param('allowFullScreen', 'true')
    ->noFlash('Playlist is not working.');
  $output .= $flashy->render() ;

See the API documentation provided on the Google Code site for a complete list
of available FlashVars.

AUTHORS AND CREDIT

Larry Garfield
http://www.palantir.net/

