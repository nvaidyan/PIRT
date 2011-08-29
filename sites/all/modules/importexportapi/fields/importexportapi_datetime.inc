<?php

/**
 * @file
 * Callback functions for the 'datetime' data definition field type.
 */

/**
 * Callback for processing of datetime field types.
 */
function importexportapi_process_datetime($field_id, $field, $format, $op, $source_get) {
  if ($format != 'db') {
    if ($op == 'put') {
      if (!empty($field['#value'])) {
        $field['#value'] = format_date($field['#value'], 'custom', 'D, d M Y H:i:s O');
      }
    }
    elseif ($op == 'get') {
      $field['#value'] = strtotime($field['#value']);
    }
  }

  return $field;
}
