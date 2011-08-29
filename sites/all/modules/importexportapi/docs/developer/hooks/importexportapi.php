<?php

/**
 * @file
 * These hooks are defined by modules that provide or alter data definitions,
 * or by modules that extend the Import / Export API.
 *
 * Modules can extend the Import / Export API in a number of ways, such as by
 * defining new field types, or by defining new 'get' or 'put' engines.
 */

/**
 * @addtogroup hooks
 * @{
 */


/**
 * Declare the data definition of one or more entities and its child fields.
 *
 * @return
 *   An array keyed by entity field name. Each element of the array is a nested
 *   data definition.
 */
function hook_def() {
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
 * Alter the data definition of one or more entities and its fields, as defined
 * through hook_def().
 *
 * @param &$def
 *   The data definitions of all entities and their fields, as defined through
 *   hook_def(). This parameter should be modified directly, as all changes to
 *   it are passed back by reference.
 */
function hook_def_alter(&$def) {
  if (isset($def['user'])) {
    $def['user']['mode'] = array(
      '#type' => 'int',
      '#title' => t('Comment viewing mode')
    );
    $def['user']['sort'] = array(
      '#type' => 'int',
      '#title' => t('Comment viewing order')
    );
    $def['user']['signature'] = array(
      '#title' => t('Signature')
    );
  }
}

/**
 * Define certain characteristics of one or more field types.
 *
 * @param $op
 *   What kind of characteristics are being sought. Possible values:
 *   - "placeholders": The placeholder to be used when constructing SQL queries
 *     that involve the specified field types.
 *   - "defaults": The default property values for each field type.
 *
 * @return
 *   This varies depending on the operation.
 *   - The "placeholders" operation should return an array in the form
 *     $field => $placeholder.
 *   - The "defaults" operation should return an array in the form
 *     $field => array('property1' => 'default1', ...).
 */
function hook_def_field_types($op) {
  $type = array();

  switch ($op) {
    case 'placeholders':
      $type['int'] = '%d';
      $type['float'] = '%f';
      $type['string'] = "'%s'";
      $type['file'] = "'%s'";
      $type['freeform'] = "'%s'";
      $type['serialized'] = "'%s'";
      $type['datetime'] = "'%s'";
      break;
    case 'defaults':
      $type['int'] = array('#required' => FALSE, '#default_value' => 0);
      $type['float'] = array('#required' => FALSE, '#default_value' => 0.0);
      $type['string'] = array('#required' => FALSE, '#default_value' => '');
      $type['file'] = array('#required' => FALSE, '#default_value' => '');
      $type['freeform'] = array('#required' => FALSE, '#default_value' => array(), '#process' => array('importexportapi_process_freeform' => array()));
      $type['serialized'] = array('#required' => FALSE, '#default_value' => '', '#process' => array('importexportapi_process_serialized' => array()));
      $type['datetime'] = array('#required' => FALSE, '#default_value' => '', '#process' => array('importexportapi_process_datetime' => array()));
      $type['array'] = array('#process' => array('importexportapi_process_array' => array()));
      $type['entity'] = array();
      break;
  }

  return $type;
}

/**
 * Define one or more 'get' or 'put' engines.
 *
 * @return
 *   A nested array of formats, 'get' and 'put' engines, and function callbacks.
 */
function hook_engines_get_put() {
  return array(
    'db' => array(
      'title' => t('Database'),
      'get' => array(
        'callback' => 'importexportapi_db_get',
        'build' => array('importexportapi_db_build' => array('get'), 'importexportapi_db_build_references' => array('get'))
      ),
      'put' => array(
        'callback' => 'importexportapi_db_put',
        'build' => array('importexportapi_db_build' => array('put'), 'importexportapi_db_build_references' => array('put')),
        'process' => array('importexportapi_db_put_resolve_alt_keys' => array())
      )
    ),
    'xml' => array(
      'title' => t('XML (eXtensible Markup Language)'),
      'get' => array(
        'callback' => 'importexportapi_xml_get',
        'build' => array('importexportapi_xml_build' => array('get')),
        'build_alt_key' => array('importexportapi_xml_build_alt_key' => array('get'))
      ),
      'put' => array(
        'callback' => 'importexportapi_xml_put',
        'build' => array('importexportapi_xml_build' => array('put')),
        'build_alt_key' => array('importexportapi_xml_build_alt_key' => array('put'))
      )
    ),
    'csv' => array(
      'title' => t('CSV (Comma Separated Values)'),
      'get' => array(
        'callback' => 'importexportapi_csv_get',
        'build' => array('importexportapi_csv_build' => array('get'))
      ),
      'put' => array(
        'callback' => 'importexportapi_csv_put',
        'build' => array('importexportapi_csv_build' => array('put'))
      )
    )
  );
}


/**
 * @} End of "addtogroup hooks".
 */
