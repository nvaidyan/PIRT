<?php

/**
 * @file
 * Common functions that are used by both the 'csv' 'get' and the 'csv' 'put'
 * engines.
 */

/**
 * Callback for build of data definition fields, in preparation for a 'csv'
 * 'get' or 'csv' 'put' operation.
 */
function importexportapi_csv_build($field, $op) {
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

  if ($field['#type'] == 'entity' || $field['#type'] == 'array' || isset($field['#csv_plural'])) {
    if (!isset($field[$op]['#csv_plural'])) {
      $values[$op]['#csv_plural'] = isset($field['#csv_plural']) ? $field['#csv_plural'] : str_replace('_', '-', $field['#id']);
    }
  }

  if (($field['#type'] != 'entity' && $field['#type'] != 'array') || isset($field['#csv_mapping'])) {
    if (!isset($field[$op]['#csv_mapping'])) {
      $values[$op]['#csv_mapping'] = isset($field['#csv_mapping']) ? $field['#csv_mapping'] : str_replace('_', '-', $field['#id']);
    }
    if (isset($field['#csv_hidden'])) {
      $values[$op]['#csv_hidden'] = $field['#csv_hidden'];
    }
  }

  if (!empty($values)) {
    importexportapi_set_def($field_array, $values);
  }

  foreach (element_children($field) as $child) {
    importexportapi_csv_build($field[$child], $op_orig);
  }
}

/**
 * Callback for build of alternate key fields, in preparation for a 'csv'
 * 'get' or 'csv' 'put' operation.
 */
function importexportapi_csv_build_alt_key($field, $op) {
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

  if (!empty($field['#alt_key_rename'])) {
    $values[$op]['#csv_mapping'] = $field['#alt_key_rename'] .'-'. $field[$op]['#csv_mapping'];
  }

  if (!empty($values)) {
    importexportapi_set_def($field_array, $values);
  }
}
