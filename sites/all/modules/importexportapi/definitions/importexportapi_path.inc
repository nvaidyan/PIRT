<?php

/**
 * @file
 * Implements data definitions on behalf of path.module.
 */

/**
 * Implementation of hook_def().
 */
function path_def() {
  $defs = array();

  $def = array(
    '#type' => 'entity',
    '#title' => t('URL alias'),
    '#xml_plural' => 'url-aliases',
    '#csv_plural' => 'url-aliases'
  );

  $def['pid'] = array(
    '#type' => 'int',
    '#title' => t('URL alias ID'),
    '#key' => TRUE
  );
  $def['src'] = array(
    '#title' => t('System path'),
    '#xml_mapping' => 'system-path',
    '#csv_mapping' => 'system-path'
  );
  $def['dst'] = array(
    '#title' => t('Aliased path'),
    '#xml_mapping' => 'aliased-path',
    '#csv_mapping' => 'aliased-path',
    '#unique' => TRUE
  );

  $defs['url_alias'] = $def;

  return $defs;
}

/**
 * Implementation of hook_db_def_tables().
 */
function path_db_def_tables() {
  $tables = array();

  $tables['url_alias'] = 'u';

  return $tables;
}
