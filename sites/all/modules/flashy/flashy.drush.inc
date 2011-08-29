<?php
// $Id: flashy.drush.inc,v 1.1.2.3 2009/08/25 23:03:49 crell Exp $

/**
 * Implementation of hook_drush_command().
 *
 * @See drush_parse_command() for a list of recognized keys.
 * @return
 *   An associative array describing your command(s).
 */
function flashy_drush_command() {
  $items = array();

  $items['flashy post-install'] = array(
    'callback' => 'flashy_install',
    'description' => dt('Finish installation of flashy.'),
    'examples' => array(
      'drush flashy install' => dt('Finish installation of flashy.'),
    ),
  );

  return $items;
}

/**
 * Implementation of hook_drush_help().
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 * @return
 *   A string with the help text for your command.
 */
function flashy_drush_help($section) {
  switch ($section) {
    case 'drush:flashy post-install':
      return dt("Download Flashy's 3rd party dependencies and place in the correct location.");
  }
}

/**
 * Drush command to download Flashy's 3rd party dependencies.
 */
function flashy_install() {

  // The path to download FROM.
  $remote_file = "http://flashy.googlecode.com/files/videoPlayer.swf";

  // The filename we want to download TO.
  $destination_file = "videoPlayer.swf";
  $destination_filename = drupal_get_path('module', 'flashy') ."/{$destination_file}";

  // This part is standard for all modules using this pattern.
  $remote_file_local = basename($remote_file);

  if (file_exists($destination_filename)) {
    drush_log(dt("{$destination_file} Already Exists!"));
  }

  // Download it.
  if (!drush_shell_exec("wget " . $remote_file)) {
    drush_shell_exec("curl -O " . $remote_file);
  }

  if(!drush_shell_exec("cp {$remote_file_local} " . $destination_filename)) {
    drush_shell_exec("rm -rf $remote_file_local");
    return drush_set_error('DRUSH_PM_DOWNLOAD_FAILED', dt("Unable to download {$remote_file} to {$destination_filename}"));
  }

  if (file_exists($destination_filename)) {
    drush_shell_exec("rm -rf {$remote_file_local}");
    drush_log("Successfully downloaded {$remote_file} to {$destination_filename}.", 'ok');
  }
  else {
    return drush_set_error('DRUSH_PM_DOWNLOAD_FAILED', dt("Unable to download {$remote_file} to {$destination_filename}"));
  }

  return TRUE;
}
