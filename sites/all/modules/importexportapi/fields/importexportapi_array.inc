<?php

/**
 * @file
 * Callback functions for the 'array' data definition field type.
 */

/**
 * Callback for processing of array field types.
 */
function importexportapi_process_array($field, $data, $format, $op, $source_get) {
  if ($op == 'get') {
    // Remove all fields that are children of array fields, since they have
    // been copied to the '#value' attribute of each copy of each array
    // field.
    foreach (element_children($data) as $child) {
      unset($data[$child]);
    }
  }

  return $data;
}
