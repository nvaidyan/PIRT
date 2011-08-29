<?php
// $Id: flashy.wrapper.inc,v 1.1.2.1 2009/04/16 21:47:55 crell Exp $

/**
 * Main controller class for configuring a Flashy player.
 */
class Flashy extends SWFObject {
  /**
   * List of paths to files to send to this player.
   *
   * @var array
   */
  protected $paths = array();

  /**
   * The path to a playlist file to send to this player.
   *
   * @var string
   */
  protected $playlist = '';

  /**
   * Constructor
   */
  function __construct() {

    $player = base_path() . drupal_get_path('module', 'flashy') .'/videoPlayer.swf';
    parent::__construct($player);

    // Set some reasonable defaults.
    $this->flashVar('maintainAspectRatio', 'true');
    $this->minimumVersion('9.0.28');
    $this->param('allowFullScreen', 'true');
  }

    /**
   * Enqueue another media file to play.
   *
   * If a playlist file is specified, values added here are ignored.
   *
   * @param $path
   *   The path to the file.
   * @return
   *   The called object.
   */
  function addMedia($path) {
    if ($path) {
      $this->paths[] = $path;
    }
    return $this;
  }

  /**
   * Define the XSPF playlist file for this player to use.
   *
   * If specified, the playlist overrides any files added using addMedia().
   *
   * @param $path
   *   The path to the playlist file.
   * @return
   *   The called object.
   */
  function setPlaylist($path) {
    $this->playlist = $path;
    return $this;
  }

  /**
   * Gets a list of all flash variables relevant to this player.
   *
   * Relevant flash variables include those defined by flashVar() as well as
   * the file or playlist to display.
   *
   * @return
   *   An array of flashVars as an associative array.
   */
  public function getFlashVars() {
    $flash_vars = parent::getFlashVars();

    // Tack on the special variables so that the Javascript code is cleaner.
    if ($this->isMultiple()) {
      $flash_vars['playlist'] = $this->getMediaUrl();
    }
    else {
      $flash_vars['video'] = $this->getMediaUrl();
    }

    return $flash_vars;
  }

  /**
   * Get a single dimension for this player.
   *
   * If an explicitly dimension is specified, that will be used.  If not,
   * the dimensions will be derived from the first file listed, if possible.
   * If that is not possible, a system-wide default will be used.
   *
   * If the dimension is not explicitly specified, it will be specified by this
   * function.
   *
   * @param $key
   *   The dimension to use, either height or width.
   * @return
   *   The value in pixels of that dimension.
   */
  protected function getDimension($key) {
    $derived = array();
    if (empty($this->dimensions[$key])) {
      // If the file is local and we have the getid3() module available, try to
      // derive the dimensions of the first file.
      if ($this->isLocalFile() && module_exists('getid3') && getid3_load() && $id3 = new getID3()) {
        $path = dirname(realpath($_SERVER['SCRIPT_FILENAME'])) .'/'. $this->paths[0];

        $id3->encoding = 'UTF-8';
        $info = $id3->analyze($path);

        // We only want to bother analyzing the image once, but if a dimension
        // has already been set explicitly then we don't want to overwrite it.
        if (empty($this->dimensions['height'])) {
          $this->dimensions['height'] = empty($info['video']['resolution_y']) ? variable_get('flashy_default_height', 300) : $info['video']['resolution_y'];
        }
        if (empty($this->dimensions['width'])) {
          $this->dimensions['width'] = empty($info['video']['resolution_x']) ? variable_get('flashy_default_height', 300) : $info['video']['resolution_x'];
        }
      }
      else {
        // If the for some reason we cannot derive the dimensions, just
        // use the defaults.
        $this->dimensions[$key] = variable_get('flashy_default_'. $key, 300);
      }
    }
    return $this->dimensions[$key];
  }

  /**
   * Gets the path to the file, as a fully qualified path.
   *
   */
  public function getMediaUrl() {
    if ($this->playlist) {
      return url($this->playlist, array('absolute' => TRUE));
    }
    if (count($this->paths) > 1) {
      // Do something with dynamic playlists.
    }
    else {
      return url($this->paths[0], array('absolute' => TRUE));
    }
  }

  /**
   * Determine if this player will display multiple files.
   *
   * @return
   *   TRUE if the player should operate in multi-file mode, FALSE otherwise.
   */
  public function isMultiple() {
    return ((bool) $this->playlist) || (count($this->paths) > 1);
  }

  /**
   * Determine if the specified path is a local file or remote.
   *
   * If the file is local then we are able to do extra file system manipulation
   * to it, such as automatically deriving its dimensions.
   *
   * @return
   *   TRUE if the file is on the local filesystem, FALSE otherwise.
   *
   */
  public function isLocalFile($index = 0) {
    if (empty($this->paths[$index])) {
      return FALSE;
    }

    $url = parse_url($this->paths[$index]);
    return empty($url['host']);
  }

  /**
   * Gets the path to the file, as a path relative to Drupal root.
   *
   * If the file is not a local file, this will return NULL.
   *
   * @return
   *   The path to the file relative to Drupal root, or NULL if the file
   *   is non-local.
   */
  public function getMediaPath() {
    $url = parse_url($this->path);
    if ($url['host']) {
      return NULL;
    }
    else {
      return $this->path;
    }
  }

  /**
   * Override the render() method for extra error checking.
   */
  public function render() {
    // If there is no media file queued up, simply return '' and do nothing.
    if ($this->playlist || $this->paths) {
      return theme('swfembed_embed', $this);
    }
    else {
      return '';
    }
  }
}
