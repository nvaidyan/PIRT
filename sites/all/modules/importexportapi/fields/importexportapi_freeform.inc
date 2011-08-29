<?php

/**
 * @file
 * Callback functions for the 'freeform' data definition field type.
 */

/**
 * Callback for processing of freeform field types.
 */
function importexportapi_process_freeform($field_id, $field, $format, $op, $source_get) {
  if ($format == 'xml' && $op == 'put') {
    if (!empty($field['#value'])) {
      $values = split("[,\n\r]", $field['#value']);
      $field['#value'] = array();

      foreach ($values as $key => $value) {
        $trim_value = trim($value);
        if (!empty($trim_value)) {
          $field['#value'][] = $trim_value;
        }
      }
    }
  }
  elseif ($source_get == 'xml' && $format == 'db') {
    if (!empty($field['#value']) && is_array($field['#value'])) {
      foreach ($field['#value'] as $key => $value) {
        $field['#value'][$key] = trim($value);
      }
      $field['#value'] = implode(', ', $field['#value']);
    }
  }

  return $field;
}
