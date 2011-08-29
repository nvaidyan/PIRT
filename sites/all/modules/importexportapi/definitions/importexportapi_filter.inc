<?php

/**
 * @file
 * Implements data definitions on behalf of filter.module.
 */

/**
 * Implementation of hook_def().
 */
function filter_def() {
  $defs = array();

  $def = array(
    '#type' => 'entity',
    '#title' => t('Input format'),
    '#db_default_table' => 'filter_formats',
    '#xml_plural' => 'input-formats',
    '#csv_plural' => 'input-formats'
  );

  $def['format'] = array(
    '#type' => 'int',
    '#title' => t('Input format ID'),
    '#key' => TRUE
  );
  $def['format_name'] = array(
    '#title' => t('Input format name'),
    '#alt_key_for' => 'format',
    '#db_field_unaliased' => 'name'
  );
  $def['role'] = array(
    '#type' => 'freeform',
    '#title' => t('Roles'),
    '#db_field_unaliased' => 'roles',
    '#xml_plural' => 'roles',
    '#csv_plural' => 'roles'
  );
  $def['cache'] = array(
    '#type' => 'int',
    '#title' => t('Cacheable')
  );

  $defs['input_format'] = $def;

  $def = array(
    '#type' => 'entity',
    '#title' => t('Filter'),
    '#db_default_table' => 'filters',
    '#xml_plural' => 'filters',
    '#csv_plural' => 'filters'
  );
  $def['fid'] = array(
    '#type' => 'int',
    '#title' => 'Filter ID',
  );
  $def['format'] = array(
    '#type' => 'int',
    '#title' => t('Input format ID'),
    '#key_component' => TRUE,
    '#reference_entity' => 'input_format'
  );
  $def['module'] = array(
    '#title' => t('Responsible module'),
    '#key_component' => TRUE
  );
  $def['delta'] = array(
    '#type' => 'int',
    '#title' => t('Delta'),
    '#key_component' => TRUE
  );
  $def['weight'] = array(
    '#type' => 'int',
    '#title' => t('Weight')
  );

  $defs['filter'] = $def;

  return $defs;
}

/**
 * Implementation of hook_node_def_alter().
 */
function filter_node_def_alter(&$def, $type) {
  $def['revisions']['format'] = array(
    '#type' => 'int',
    '#title' => t('Input format ID'),
    '#reference_entity' => 'input_format'
  );
}

/**
 * Implementation of hook_db_def_tables().
 */
function filter_db_def_tables() {
  $tables = array();

  $tables['filter_formats'] = 'ff';
  $tables['filters'] = 'fi';

  return $tables;
}
