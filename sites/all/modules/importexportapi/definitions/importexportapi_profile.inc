<?php

/**
 * @file
 * Implements data definitions on behalf of profile.module.
 */

/**
 * Implementation of hook_def().
 */
function profile_def() {
  $defs = array();

  $def = array(
    '#type' => 'entity',
    '#title' => t('Profile field'),
    '#db_default_table' => 'profile_fields',
    '#xml_plural' => 'profile-fields',
    '#csv_plural' => 'profile-fields'
  );

  $def['fid'] = array(
    '#type' => 'int',
    '#title' => t('Field ID'),
    '#key' => TRUE
  );
  $def['title'] = array(
    '#title' => t('Title (human-readable)'),
    '#alt_key_for' => 'fid'
  );
  $def['field_name'] = array(
    '#title' => t('Name (machine-readable)'),
    '#alt_key_for' => 'fid',
    '#db_field_unaliased' => 'name'
  );
  $def['type'] = array(
    '#title' => t('Type'),
    '#required' => TRUE
  );
  $def['category'] = array(
    '#title' => t('Category'),
    '#default_value' => NULL
  );
  $def['explanation'] = array(
    '#title' => t('Explanation'),
    '#default_value' => NULL
  );
  $def['options'] = array(
    '#type' => 'freeform',
    '#title' => t('Selection options'),
    '#default_value' => NULL,
    '#xml_plural' => 'options',
    '#xml_mapping' => 'option'
  );
  $def['weight'] = array(
    '#type' => 'int',
    '#title' => t('Weight')
  );
  $def['visibility'] = array(
    '#type' => 'int',
    '#title' => t('Visibility')
  );
  $def['page'] = array(
    '#title' => t('Page title'),
    '#default_value' => NULL
  );
  $def['required'] = array(
    '#type' => 'int',
    '#title' => t('Required')
  );
  $def['register'] = array(
    '#type' => 'int',
    '#title' => t('Visible in user registration form')
  );

  $defs['profile_field'] = $def;

  return $defs;
}

/**
 * Implementation of hook_def_alter().
 */
function profile_def_alter(&$def) {
  if (isset($def['user'])) {
    $fields = _importexportapi_profile_get_fields();

    if (!empty($fields)) {
      $def['user']['profile'] = array(
        '#type' => 'array',
        '#title' => t('Profile information'),
        '#db_default_table' => 'profile_values',
        '#xml_plural' => 'profile-values',
        '#csv_plural' => 'profile-values',
        '#process' => array(
          '_importexportapi_profile_process_values' => array($fields)
        )
      );

      $def['user']['profile']['fid'] = array(
        '#type' => 'int',
        '#title' => t('Field ID'),
        '#reference_entity' => 'profile_field',
        '#key_component' => TRUE
      );
      $def['user']['profile']['uid'] = array(
        '#type' => 'int',
        '#title' => t('User ID'),
        '#reference_entity' => 'user',
        '#key_component' => TRUE
      );
      $def['user']['profile']['value'] = array(
        '#title' => t('Field value'),
        '#default_value' => NULL,
        '#xml_plural' => 'values'
      );
    }
  }
}

/**
 * Implementation of hook_db_def_tables().
 */
function profile_db_def_tables() {
  $tables = array();

  $tables['profile_fields'] = 'pf';
  $tables['profile_values'] = 'pv';

  return $tables;
}

/**
 * Callback for processing of a user's profile field values.
 */
function _importexportapi_profile_process_values($field_id, $field, $format, $op, $source_get, $fields) {
  if (isset($field['#value'])) {
    foreach ($field['#value'] as $child_id => $child) {
      if (($format == 'xml' && $op == 'put') || ($source_get == 'xml' && $format == 'db')) {
        $fid = $child['fid']['#value'];
        if (isset($fid)) {
          $field_type = $fields[$fid]->type;
          if (isset($field_type) && $field_type == 'list') {
            $field['#value'][$child_id]['value']['#type'] = 'freeform';
          }

          if (isset($field_type) && $field_type == 'date') {
            $field['#value'][$child_id]['value']['#type'] = 'serialized';
          }

          if (isset($fid) && !empty($fields[$fid]->required)) {
            $field['#value'][$child_id]['value']['#required'] = TRUE;
          }
        }
      }

      if ($field['#value'][$child_id]['value']['#type'] == 'freeform') {
        $field['#value'][$child_id]['value'] = importexportapi_process_freeform('value', $field['#value'][$child_id]['value'], $format, $op, $source_get);
      }

      if ($field['#value'][$child_id]['value']['#type'] == 'serialized') {
        $field['#value'][$child_id]['value'] = importexportapi_process_serialized('value', $field['#value'][$child_id]['value'], $format, $op, $source_get);
      }
    }
  }

  return $field;
}

/**
 * Retrieves all available profile fields, and some of their attributes.
 *
 * @param $refresh
 *   Whether or not to reset the internal cache of this function (defaults to
 *   FALSE).
 *
 * @return
 *   Array of available profile fields, with the field names as keys, and with
 *   an object holding title, required, and type attributes as values.
 */
function _importexportapi_profile_get_fields($refresh = FALSE) {
  static $fields;

  if (!isset($fields) || $refresh) {
    $fields = array();
    $result = db_query('SELECT fid, name, title, required, type FROM {profile_fields} ORDER BY category, weight');
    while ($field = db_fetch_object($result)) {
      $fields[$field->fid] = $field;
      unset($fields[$field->fid]->fid);

    }
  }

  return $fields;
}
