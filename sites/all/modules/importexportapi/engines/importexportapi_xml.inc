<?php

/**
 * @file
 * Common functions that are used by both the 'xml' 'get' and the 'xml' 'put'
 * engines.
 */

/**
 * Callback for build of data definition fields, in preparation for an 'xml'
 * 'get' or 'xml' 'put' operation.
 */
function importexportapi_xml_build($field, $op) {
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

  if (!isset($field[$op]['#xml_mapping'])) {
    $values[$op]['#xml_mapping'] = isset($field['#xml_mapping']) ? $field['#xml_mapping'] : str_replace('_', '-', $field['#id']);
  }

  if ($field['#type'] == 'entity' || $field['#type'] == 'array' || isset($field['#xml_plural'])) {
    if (!isset($field[$op]['#xml_plural'])) {
      $values[$op]['#xml_plural'] = isset($field['#xml_plural']) ? $field['#xml_plural'] : str_replace('_', '-', $field['#id']);
    }
  }
  else {
    $is_local_reference = FALSE;
    if (isset($field['#reference_entity']) && (!isset($field['#reference_parent']) || $field['#reference_parent'])) {
      $parents = $field['#parents'];
      array_pop($parents);
      $parent_entity = array_pop($parents);

      if ($field['#reference_entity'] == $parent_entity) {
        $is_local_reference = TRUE;
      }
    }
    if ($is_local_reference || isset($field['#xml_hidden'])) {
      $values[$op]['#xml_hidden'] = isset($field['#xml_hidden']) ? $field['#xml_hidden'] : TRUE;
    }
  }

  if (!empty($values)) {
    importexportapi_set_def($field_array, $values);
  }

  foreach (element_children($field) as $child) {
    importexportapi_xml_build($field[$child], $op_orig);
  }
}

/**
 * Callback for build of alternate key fields, in preparation for an 'xml'
 * 'get' or 'xml' 'put' operation.
 */
function importexportapi_xml_build_alt_key($field, $op) {
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
  if (!empty($orig_field[$op]['#xml_hidden'])) {
    $values[$op]['#xml_hidden'] = TRUE;
  }

  if (!empty($field['#alt_key_rename'])) {
    $values[$op]['#xml_mapping'] = $values['#xml_mapping'] = str_replace('_', '-', $field['#alt_key_rename']) .'-'. $field[$op]['#xml_mapping'];
    if (isset($field[$op]['#xml_plural'])) {
      $values[$op]['#xml_plural'] = $values['#xml_plural'] =  str_replace('_', '-', $field['#alt_key_rename']) .'-'. $field[$op]['#xml_plural'];
    }
  }

  if (!empty($values)) {
    importexportapi_set_def($field_array, $values);
  }
}

/**
 * Loads the MiniXML 3rd party XML library.
 *
 * @return
 *   A new MiniXMLDoc object.
 */
function _importexportapi_include_minixml() {
  $path = './'. drupal_get_path('module', 'importexportapi') .'/libraries/minixml/minixml.inc.php';
  require_once($path);

  return new MiniXMLDoc();
}
