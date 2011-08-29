<?php

/**
 * @file
 * Common functions that are used by both the 'db' 'get' and the 'db' 'put'
 * engines.
 */

/**
 * Callback for build of data definition fields, in preparation for a 'db'
 * 'get' or 'db' 'put' operation.
 */
function importexportapi_db_build($field, $op) {
  $values = array();
  $field_array = array();
  $op_orig = $op;
  $op = '#'. $op;

  if ($field['#type'] == 'entity') {
    $field_array[] = $field['#id'];
  }
  else {
    $field_array = $field['#parents'];
    $field_array[] = $field['#id'];
  }

  if ($field['#type'] == 'entity' || $field['#type'] == 'array') {
    if (!isset($field[$op]['#db_default_table'])) {
      $values[$op]['#db_default_table'] = isset($field['#db_default_table']) ? $field['#db_default_table'] : $field['#id'];
    }
    if (!empty($field['#db_process_generate_id'])) {
      if (!isset($field[$op]['#db_process_generate_id']) && $op_orig == 'put') {
        $values[$op]['#db_process_generate_id'] = $field['#db_process_generate_id'];
      }
    }
  }
  else {
    $parent_field = importexportapi_get_def($field['#parents']);

    if (!isset($field[$op]['#db_table'])) {
      $values[$op]['#db_table'] = isset($field['#db_table']) ? $field['#db_table'] : $parent_field[$op]['#db_default_table'];
    }
    if (!isset($field[$op]['#db_field'])) {
      $values[$op]['#db_field'] = isset($field['#db_field']) ? $field['#db_field'] : $field['#id'];
    }
    if (!isset($field[$op]['#db_field_unaliased'])) {
      $values[$op]['#db_field_unaliased'] = isset($field['#db_field_unaliased']) ? $field['#db_field_unaliased'] : $field['#id'];
    }

    if ($op_orig == 'put') {
      if (!empty($field['#key']) || !empty($field['#key_component']) || !empty($field['#reference_entity'])) {
        if (!isset($field[$op]['#db_update_existing'])) {
          $values[$op]['#db_update_existing'] = isset($field['#db_update_existing']) ? $field['#db_update_existing'] : TRUE;
        }
      }
      if (!empty($field['#key'])) {
        if (!isset($field[$op]['#db_generate_id'])) {
          $values[$op]['#db_generate_id'] = isset($field['#db_generate_id']) ? $field['#db_generate_id'] : TRUE;
        }
        if (!isset($field[$op]['#db_force_generate_id'])) {
          $values[$op]['#db_force_generate_id'] = isset($field['#db_force_generate_id']) ? $field['#db_force_generate_id'] : FALSE;
        }
        if (!isset($field[$op]['#db_uses_sequences'])) {
          $values[$op]['#db_uses_sequences'] = isset($field['#db_uses_sequences']) ? $field['#db_uses_sequences'] : FALSE;
        }
      }
    }
    else {
      if (isset($field['#db_filter'])) {
        if (!isset($field[$op]['#db_filter'])) {
          $values[$op]['#db_filter'] = $field['#db_filter'];
        }
      }
      if (isset($field['#db_delta'])) {
        if (!isset($field[$op]['#db_delta'])) {
          $values[$op]['#db_delta'] = $field['#db_delta'];
        }
      }
    }
  }

  if (!empty($values)) {
    importexportapi_set_def($field_array, $values);
  }

  foreach (element_children($field) as $child) {
    importexportapi_db_build($field[$child], $op_orig);
  }
}

/**
 * Callback for build of data definition fields, in preparation for a 'db'
 * 'get' or 'db' 'put' operation.
 */
function importexportapi_db_build_references($field, $op) {
  $values = array();
  $field_array = array();
  $op_orig = $op;
  $op = '#'. $op;

  if ($field['#type'] == 'entity') {
    $field_array[] = $field['#id'];
  }
  else {
    $field_array = $field['#parents'];
    $field_array[] = $field['#id'];
  }

  if ($field['#type'] != 'entity' && $field['#type'] != 'array') {
    if (isset($field['#reference_field'])) {
      if (!isset($field[$op]['#db_reference_table']) || !isset($field[$op]['#db_reference_field'])) {
        $reference_array = array_merge(array($field['#reference_entity']), $field['#reference_field']);
        $reference_field = importexportapi_get_def($reference_array);

        if (!isset($field[$op]['#db_reference_table'])) {
          $values[$op]['#db_reference_table'] = isset($field['#db_reference_table']) ? $field['#db_reference_table'] : $reference_field[$op]['#db_table'];
        }
        if (!isset($field[$op]['#db_reference_field'])) {
          $values[$op]['#db_reference_field'] = isset($field['#db_reference_field']) ? $field['#db_reference_field'] : $reference_field[$op]['#db_field'];
        }
        if (!isset($field[$op]['#db_reference_field_unaliased'])) {
          $values[$op]['#db_reference_field_unaliased'] = isset($field['#db_reference_field_unaliased']) ? $field['#db_reference_field_unaliased'] : $reference_field[$op]['#db_field_unaliased'];
        }
        if (!isset($field[$op]['#db_reference_delta'])) {
          $values[$op]['#db_reference_delta'] = $field['#reference_delta'];
        }
        if (!isset($field[$op]['#db_reference_parent'])) {
          $values[$op]['#db_reference_parent'] = isset($field['#reference_parent']) ? $field['#reference_parent'] : NULL;
        }
      }
    }
  }
  else {
    if (!empty($field['#keys'])) {
      foreach ($field['#keys'] as $key_field) {
        if (isset($field[$key_field])) {
          $key_db_field = $field[$key_field][$op]['#db_field_unaliased'];
          $values[$op]['#db_keys'][$key_db_field] = $key_db_field;
        }
      }
    }
  }

  if (!empty($values)) {
    importexportapi_set_def($field_array, $values);
  }

  foreach (element_children($field) as $child) {
    importexportapi_db_build_references($field[$child], $op_orig);
  }
}

/**
 * Callback for build of alternate key fields, in preparation for a 'db'
 * 'get' or 'db' 'put' operation.
 */
function importexportapi_db_build_alt_key($field, $op) {
  $values = array();
  $field_array = array();
  $op_orig = $op;
  $op = '#'. $op;

  if ($field['#type'] == 'entity') {
    $field_array[] = $field['#id'];
  }
  else {
    $field_array = $field['#parents'];
    $field_array[] = $field['#id'];
  }

  $orig_field = importexportapi_get_def(array_merge($field['#parents'], array($field['#alt_key_for'])));
  if (!empty($orig_field[$op]['#db_reference_delta'])) {
    $values[$op]['#db_delta'] = $orig_field[$op]['#db_reference_delta'];
  }
  if (isset($orig_field[$op]['#db_reference_parent'])) {
    $values[$op]['#db_reference_parent'] = $orig_field[$op]['#db_reference_parent'];
  }

  if (!empty($field['#alt_key_rename'])) {
    $values[$op]['#db_field'] = $values['#db_field'] = $field['#alt_key_rename'] .'_'. $field[$op]['#db_field'];
  }

  if (!empty($values)) {
    importexportapi_set_def($field_array, $values);
  }
}

/**
 * Retrieves the list of database tables and their default aliases.
 *
 * @param $refresh
 *   Whether or not to reset the internal cache of this function (defaults to
 *   FALSE).
 *
 * @return
 *   An array of database tables (keys) and default aliases (values).
 */
function importexportapi_db_get_def_tables($refresh = FALSE) {
  static $tables;

  if (!isset($tables) || $refresh) {
    $tables = module_invoke_all('db_def_tables');
  }

  return $tables;
}
