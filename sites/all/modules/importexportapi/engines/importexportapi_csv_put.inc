<?php

/**
 * @file
 * The 'csv' 'put' engine for the Import / Export API.
 */

require_once(drupal_get_path('module', 'importexportapi') .'/engines/importexportapi_csv.inc');

/**
 * Callback for rendering data in CSV export format.
 */
function importexportapi_csv_put($data, $variables) {
  $export = array();
  $entity_count = array();
  $csv_docs = array();

  // Convert the structured data into a series of flat arrays.
  foreach ($data as $entity) {
    // Recurse down through children, and add all child values to the series
    // of flat arrays.
    _importexportapi_csv_put($export, $entity, $entity_count);
  }

  // Convert each flat array into a CSV-formatted string.
  foreach ($export as $entity_id => $entity_data) {
    $csv_docs[$entity_id] = importexportapi_csv_put_formatted($entity_data);
  }

  return $csv_docs;
}

/**
 * Helper function for importexportapi_csv_put(). Converts the specified
 * field and its children into a series of flat arrays.
 *
 * @param &$export
 *   The series of flat arrays to populate with values. These
 *   values are passed back by reference.
 * @param $fields
 *   The fields from which to assign values to the series of flat arrays.
 *   The children of these fields are recursively added as well.
 * @param &$entity_count
 *   An array of counters to track the number of entities or array fields
 *   that have been put into the flat arrays.
 */
function _importexportapi_csv_put(&$export, $fields, &$entity_count) {
  $is_first_row = FALSE;
  $entity_index = $fields['#id'];
  $entity_put = $fields['#put'];
  $mapping = $entity_put['#csv_plural'];

  if (!empty($fields['#parents'])) {
    // Maintain a unique index for all array fields - do this by prepending
    // the field's parents to the index.
    $entity_index = implode('.', $fields['#parents']) .'.'. $entity_index;
  }

  if (!isset($export[$mapping])) {
    // Initialisation that occurs when the first record of a particular entity
    // or array field is added.
    $export[$mapping] = array();
    $entity_count[$entity_index] = 1;
    $is_first_row = TRUE;
  }

  foreach (element_children($fields) as $field_index) {
    $field = $fields[$field_index];
    $put = $field['#put'];

    if ($field['#type'] != 'array') {
      if (empty($put['#hidden']) && empty($field['#generated'])) {
        if ($is_first_row) {
          // Row '0' is always the 'header' row - during the first time through
          // for each entity or array field, the header titles need to be added
          // to the flat array.
          $export[$mapping][0][] = $put['#csv_mapping'];
        }

        if (isset($field['#value']) && $field['#value'] !== '') {
          // Assign this field to the series of flat arrays.
          $export[$mapping][$entity_count[$entity_index]][] = (string) $field['#value'];
        }
        else {
          $export[$mapping][$entity_count[$entity_index]][] = '';
        }
      }
    }
    else {
      foreach (element_children($field['#value']) as $child) {
        // Recurse down into nested arrays.
        _importexportapi_csv_put($export, $field['#value'][$child], $entity_count);
      }
    }
  }

  $entity_count[$entity_index]++;
}

/**
 * Transforms a nested array into a string in CSV format, where fields and
 * records are character-delimited. This function is based on a custom version
 * of the fputcsv() function, which can be found at this URL:
 * http://php.net/fputcsv#57164
 *
 * @param $records
 *   A nested array of 'lines' and 'fields' to be transformed into CSV format.
 * @param $field_delimiter
 *   The character to use as a field delimiter (defaults to comma ','
 *   character).
 * @param $record_delimiter
 *   The character to use as a record delimiter (defaults to newline '\n'
 *   character).
 * @param $enclosure
 *   The character to use as an enclosure (defaults to double-quote '"'
 *   character).
 *
 * @return
 *   A string containing the lines and fields in CSV format.
 */
function importexportapi_csv_put_formatted($records, $field_delimiter = ',', $record_delimiter = "\n", $enclosure = "\"") {
  // Build the string
  $csv_doc = '';

  // for each array element, which represents a line in the CSV string...
  foreach ($records as $line) {
    // No leading delimiter
    $use_delimiter = FALSE;

    foreach ($line as $element) {
      // Replaces a double quote with two double quotes
      $element = str_replace("\"", "\"\"", $element);

      // Adds a delimiter before each field (except the first)
      if ($use_delimiter) {
        $csv_doc .= $field_delimiter;
      }

      // Encloses each field with $enclosure and adds it to the string
      $csv_doc .= $enclosure . $element . $enclosure;

      // Delimiters are used every time except the first.
      $use_delimiter = TRUE;
    }

    // Append new line
    $csv_doc .= $record_delimiter;
  }

  return $csv_doc;
}
