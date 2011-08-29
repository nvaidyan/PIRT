<?php

/**
 * @file
 * The 'csv' 'get' engine for the Import / Export API.
 */

require_once(drupal_get_path('module', 'importexportapi') .'/engines/importexportapi_csv.inc');

/**
 * Callback for parsing CSV data into a structured array.
 */
function importexportapi_csv_get($def, $variables) {
  $import = array();

  if (empty($variables['raw']) || !is_array($variables['raw'])) {
    return NULL;
  }

  // Convert each CSV-formatted string into a flat array.
  foreach ($variables['raw'] as $entity_id => $entity_data) {
    $import[$entity_id] = importexportapi_csv_get_parsed($entity_data);
  }

  $data = array();
  $get = $def['#get'];

  // Check that the entity is available in the CSV.
  if (isset($import[$get['#csv_plural']]) && is_array($import[$get['#csv_plural']])) {
    $field_map = array_flip(array_shift($import[$get['#csv_plural']]));
    foreach ($import[$get['#csv_plural']] as $entity_id => $entity) {
      if (is_int($entity_id)) {
        // For each entity, copy the definition from $def to a new array
        // element in $data.
        $data[$entity_id] = $def;
        _importexportapi_csv_get($data[$entity_id], $import, $entity_id, $field_map);
      }
    }
  }

  return $data;
}

/**
 * Helper function for importexportapi_csv_get(). Recurses down through the
 * provided CSV data, and adds all the available field values to the structured
 * array.
 *
 * @param $entity
 *   The structured array of the current entity or array field. This field has
 *   the values of its children altered and passed back by reference.
 * @param $import
 *   The CSV import data, organised as an array of flat strings.
 * @param $import_id
 *   The index of the current CSV import string in the $import array to use.
 * @param $field_map
 *   An array of CSV-field to numeric-index mappings, to allow for navigation
 *   through the $import array.
 */
function _importexportapi_csv_get(&$entity, $import, $import_id, $field_map) {
  $get = $entity['#get'];
  $entity_import = $import[$get['#csv_plural']][$import_id];

  foreach (element_children($entity) as $field_index) {
    $field = $entity[$field_index];
    $get_field = $field['#get'];

    if ($field['#type'] != 'array') {
      if (isset($entity_import[$field_map[$get_field['#csv_mapping']]]) && $entity_import[$field_map[$get_field['#csv_mapping']]] !== '') {
        // Assign this field to the data structure.
        $entity[$field_index]['#value'] = $entity_import[$field_map[$get_field['#csv_mapping']]];
        importexportapi_string_typecast($field['#type'], $entity[$field_index]['#value']);
      }
      else {
        $entity[$field_index]['#value'] = $field['#default_value'];
      }
    }
    else {
      $entity_data = $entity[$field_index];

      if (isset($import[$get_field['#csv_plural']]) && is_array($import[$get_field['#csv_plural']])) {
        $child_field_map = array_flip(array_shift($import[$get_field['#csv_plural']]));
        foreach ($import[$get_field['#csv_plural']] as $child_id => $child) {
          $parent_array = array('import' => $entity_import, 'field_map' => $field_map, 'def' => $entity);
          $child_array = array('import' => $import[$get_field['#csv_plural']][$child_id], 'field_map' => $child_field_map, 'def' => $entity_data);
          if (is_int($child_id) && _importexportapi_csv_get_check_key_match($parent_array, $child_array)) {
            // For each array, copy the definition of the field to a new array
            // element nested within $entity.
            $entity[$field_index]['#value'][$child_id] = $entity_data;
            _importexportapi_csv_get($entity[$field_index]['#value'][$child_id], $import, $child_id, $child_field_map);
          }
        }
      }
    }
  }
}

/**
 * Transforms a CSV-formatted string into a nested array of records and fields,
 * where records and fields are character-delimited. This function is based on
 * the custom CSV2Array() function, which can be found at this URL:
 * http://php.net/fgetcsv#53932
 *
 * @param $csv_doc
 *   A CSV-formatted string to be transformed into a nested array of records
 *   and fields.
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
 *   A nested array of records and fields.
 */
function importexportapi_csv_get_parsed($csv_doc, $field_delimiter = ',', $record_delimiter = "\n", $enclosure = "\"", $optional = 1) {
  $line_count = 0;
  $double_enclosure = $enclosure . $enclosure;
  $records = array();
  $csv_extract = array();

  // Clean up line endings.
  if ($record_delimiter == "\n") {
    $csv_doc = str_replace("\r\n", "\n", $csv_doc);
    $csv_doc = str_replace("\n\r", "\n", $csv_doc);
    $csv_doc = str_replace("\r", "\n", $csv_doc);
  }

  // The regular expression requires a newline to be at the end of the string.
  if ($csv_doc[strlen($csv_doc)-1] != $record_delimiter) {
    $csv_doc .= $record_delimiter;
  }

  // The magic CSV parsing regular expression.
  $expr = '/(('. $enclosure .')'. ($optional ? '?(((?(2)' : '(') .'('. $double_enclosure .'|[^'. $enclosure .'])*))'. $enclosure .'?[^'. $field_delimiter . $record_delimiter .']*))('. $field_delimiter .'|'. $record_delimiter .')/smi';

  preg_match_all($expr, $csv_doc, $csv_extract);

  // Loop through, and assign all the fields and records from the CSV string
  // into the flat array.
  for ($i = 0; $i < count($csv_extract[6]); $i++) {
    if ($csv_extract[4][$i] === '' && $csv_extract[3][$i] !== '"') {
      $records[$line_count][] = $csv_extract[3][$i];
    }
    else {
      $records[$line_count][] = str_replace($double_enclosure, $enclosure, $csv_extract[4][$i]);
    }
    if ($csv_extract[6][$i] != $field_delimiter) {
      $line_count++;
    }
  }

  return $records;
}

/**
 * Helper function for _importexportapi_csv_get(). Compares a parent entity or
 * array field to a child array field, and determines whether or not the former
 * is actually the parent of the latter. The two fields have a parent-child
 * relationship if this function can find one field in the child array that
 * references a key or alternate key field in the parent array, and if the
 * values of these two fields are equal.
 *
 * @param $parent_array
 *   An array containing all the information about the parent field that this
 *   function needs, in order to perform its comparison.
 * @param $child_array
 *   An array containing all the information about the child field that this
 *   function needs, in order to perform its comparison.
 *
 * @return
 *   Boolean value indicating whether or not a matching pair of referencing
 *   values from the two fields was found.
 */
function _importexportapi_csv_get_check_key_match($parent_array, $child_array) {
  $parent_import = $parent_array['import'];
  $parent_field_map = $parent_array['field_map'];

  $child_import = $child_array['import'];
  $child_field_map = $child_array['field_map'];

  // Check each field in the child array.
  foreach (element_children($child_array['def']) as $child_field_id) {
    $child_field = $child_array['def'][$child_field_id];
    $child_get_field = $child_field['#get'];

    // Check that this field has a value.
    if (isset($child_import[$child_field_map[$child_get_field['#csv_mapping']]]) && $child_import[$child_field_map[$child_get_field['#csv_mapping']]] !== '') {
      if (isset($child_field['#reference_entity'])) {
        // Create a string representation of the path to this field.
        $child_field_path = implode('.', array_merge(array($child_field['#reference_entity']), $child_field['#reference_field']));

        // Grab the ID of the immediate parent of this field, as this is all we
        // need in order to find the field.
        $child_field_ref = end($child_field['#reference_field']);
        reset($child_field['#reference_field']);

        // Check that the parent field exists in the parent entity or array.
        if (isset($parent_array['def'][$child_field_ref])) {
          $parent_field = $parent_array['def'][$child_field_ref];
          $parent_get_field = $parent_field['#get'];
          // Create another string representation of the path, this time for
          // the parent field.
          $parent_field_path = implode('.', array_merge($parent_field['#parents'], array($parent_field['#id'])));

          // Check that the paths match - if so, we're looking at the right
          // field. Also check that the parent field is a key field or an
          // alternate key field, as they're the only ones that we're
          // interested in.
          if ($child_field_path == $parent_field_path && (!empty($parent_field['#key']) || !empty($parent_field['#alt_key_for']))) {
            // Check that the parent field has a value.
            if (isset($parent_import[$parent_field_map[$parent_get_field['#csv_mapping']]]) && $parent_import[$parent_field_map[$parent_get_field['#csv_mapping']]] !== '') {
              // Finally, check that the value of the parent field matches the
              // value of the child field.
              if ($child_import[$child_field_map[$child_get_field['#csv_mapping']]] == $parent_import[$parent_field_map[$parent_get_field['#csv_mapping']]]) {
                // Once we find a match, stop looking for more matches.
                return TRUE;
              }
            }
          }
        }
      }
    }
  }

  // If we reached this point, no matches were found.
  return FALSE;
}
