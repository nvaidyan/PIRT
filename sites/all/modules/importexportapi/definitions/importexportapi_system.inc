<?php

/**
 * @file
 * Implements data definitions on behalf of system.module.
 */

/**
 * Implementation of hook_def().
 */
function system_def() {
  $defs = array();

  $def = array(
    '#type' => 'entity',
    '#title' => t('Variable'),
    '#xml_plural' => 'variables',
    '#csv_plural' => 'variables'
  );

  $def['name'] = array(
    '#title' => t('Variable name'),
    '#key_component' => TRUE,
    '#unique' => TRUE
  );
  $def['value'] = array(
    '#type' => 'serialized',
    '#title' => t('Variable value')
  );

  $defs['variable'] = $def;

  $def = array(
    '#type' => 'entity',
    '#title' => t('File'),
    '#db_default_table' => 'files',
    '#xml_plural' => 'files',
    '#csv_plural' => 'files'
  );

  $def['fid'] = array(
    '#type' => 'int',
    '#title' => t('File ID'),
    '#key' => TRUE,
    '#db_uses_sequences' => TRUE
  );
  $def['uid'] = array(
    '#type' => 'int',
    '#title' => t('User ID'),
    '#reference_entity' => 'users'
  );
  $def['file_name'] = array(
    '#title' => t('File name'),
    '#db_field_unaliased' => 'filename'
  );
  $def['file_path'] = array(
    '#type' => 'file',
    '#title' => t('File path'),
    '#db_field_unaliased' => 'filepath'
  );
  $def['file_mime'] = array(
    '#title' => t('File MIME type'),
    '#db_field_unaliased' => 'filemime'
  );
  $def['file_size'] = array(
    '#title' => t('File size'),
    '#db_field_unaliased' => 'filesize'
  );
  $def['status'] = array(
    '#type' => 'int',
    '#title' => t('Status'),
  );
  $def['timestamp'] = array(
    '#type' => 'int',
    '#title' => t('Last modified date'),
  );

  $defs['file'] = $def;

  return $defs;
}

/**
 * Implementation of hook_db_def_tables().
 */
function system_db_def_tables() {
  $tables = array();

  $tables['files'] = 'f';
  $tables['file_revisions'] = 'fr';
  $tables['variable'] = 'va';

  return $tables;
}
