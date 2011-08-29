<?php

/**
 * @file
 * The 'xml' 'get' engine for the Import / Export API.
 */

require_once(drupal_get_path('module', 'importexportapi') .'/engines/importexportapi_xml.inc');

/**
 * Callback for parsing XML data into a structured array.
 */
function importexportapi_xml_get($def, $variables) {
  if (empty($variables['raw'])) {
    return NULL;
  }

  // Let MiniXML do the hard work.
  $xml_doc = _importexportapi_include_minixml();
  $xml_doc->fromString($variables['raw']);

  $import = $xml_doc->toArray();
  unset($xml_doc);
  $import = $import['drupal'];
  $data = array();
  $get = $def['#get'];

  // Check that the entity is available in the XML.
  if (isset($import[$get['#xml_plural']]) && is_array($import[$get['#xml_plural']])) {
    if (isset($import[$get['#xml_plural']][$get['#xml_mapping']]) && is_array($import[$get['#xml_plural']][$get['#xml_mapping']])) {
      // MiniXML has an annoying habit of treating elements with a single child
      // differently to elements with multiple children. This little hack makes
      // the MiniXML array consistent, regardless of number of child elements.
      if (!is_array($import[$get['#xml_plural']][$get['#xml_mapping']]) || !isset($import[$get['#xml_plural']][$get['#xml_mapping']]['_num'])) {
        $entity = $import[$get['#xml_plural']][$get['#xml_mapping']];
        $import[$get['#xml_plural']][$get['#xml_mapping']] = array(0 => $entity);
      }
      foreach ($import[$get['#xml_plural']][$get['#xml_mapping']] as $entity_id => $entity) {
        if (is_int($entity_id)) {
          // For each entity, copy the definition from $def to a new array
          // element in $data.
          $data[$entity_id] = $def;
          foreach (element_children($def) as $field_index) {
            $get_field = $def[$field_index]['#get'];
            if ($data[$entity_id][$field_index]['#type'] == 'array') {
              // Initialise all array fields to an empty array, which will get
              // populated when we recurse into the array.
              $data[$entity_id][$field_index]['#value'] = array();
              _importexportapi_xml_get($data[$entity_id][$field_index], $entity);
            }
            else {
              // Populate this top-level field with its value, or with the
              // default value for that field type if none was found.
              if (isset($get_field['#xml_plural']) && isset($entity[$get_field['#xml_plural']][$get_field['#xml_mapping']])) {
                $data[$entity_id][$field_index]['#value'] = $entity[$get_field['#xml_plural']][$get_field['#xml_mapping']];
                if (!isset($data[$entity_id][$field_index]['#value']['_num'])) {
                  $child = $data[$entity_id][$field_index]['#value'];
                  $data[$entity_id][$field_index]['#value'] = array(0 => $child);
                }
                else {
                  unset($data[$entity_id][$field_index]['#value']['_num']);
                }

                foreach (array_keys($data[$entity_id][$field_index]['#value']) as $value_key) {
                  if (is_array($data[$entity_id][$field_index]['#value'][$value_key]) && isset($data[$entity_id][$field_index]['#value'][$value_key]['CDATA'])) {
                    $data[$entity_id][$field_index]['#value'][$value_key] = $data[$entity_id][$field_index]['#value'][$value_key]['CDATA'];
                  }
                }
              }
              elseif (isset($entity[$get_field['#xml_mapping']]) && $entity[$get_field['#xml_mapping']] !== '') {
                if (is_array($entity[$get_field['#xml_mapping']]) && isset($entity[$get_field['#xml_mapping']]['CDATA'])) {
                  $entity[$get_field['#xml_mapping']] = $entity[$get_field['#xml_mapping']]['CDATA'];
                }
                $data[$entity_id][$field_index]['#value'] = $entity[$get_field['#xml_mapping']];
                importexportapi_string_typecast($data[$entity_id][$field_index]['#type'], $data[$entity_id][$field_index]['#value']);
              }
              else {
                $data[$entity_id][$field_index]['#value'] = $def[$field_index]['#default_value'];
              }
            }
          }
        }
      }
    }
  }

  return $data;
}

/**
 * Helper function for importexportapi_xml_get(). Recurses down through the
 * provided XML data, and adds all the available field values to the structured
 * array.
 *
 * @param $field
 *   The structured array of the current field. This field has its value
 *   altered and passed back by reference.
 * @param $import
 *   The XML import data, as provided by MiniXML.
 */
function _importexportapi_xml_get(&$field, $import) {
  $get = $field['#get'];

  if ($field['#type'] != 'array') {
    if (isset($get['#xml_plural']) && isset($import[$get['#xml_plural']][$get['#xml_mapping']])) {
      $field['#value'] = $import[$get['#xml_plural']][$get['#xml_mapping']];
      if (!is_array($field['#value']) || !isset($field['#value']['_num'])) {
        $child = $field['#value'];
        $field['#value'] = array(0 => $child);
      }
      else {
        unset($field['#value']['_num']);
      }

      foreach (array_keys($field['#value']) as $value_key) {
        if (is_array($field['#value'][$value_key]) && isset($field['#value'][$value_key]['CDATA'])) {
          $field['#value'][$value_key] = $field['#value'][$value_key]['CDATA'];
        }
      }
    }
    elseif (isset($import[$get['#xml_mapping']]) && $import[$get['#xml_mapping']] !== '') {
      if (is_array($import[$get['#xml_mapping']]) && isset($import[$get['#xml_mapping']]['CDATA'])) {
        $import[$get['#xml_mapping']] = $import[$get['#xml_mapping']]['CDATA'];
      }
      // Assign this field to the data structure.
      $field['#value'] = $import[$get['#xml_mapping']];
      importexportapi_string_typecast($field['#type'], $field['#value']);
    }
    else {
      $field['#value'] = $field['#default_value'];
    }
  }
  else {
    if (isset($import[$get['#xml_plural']]) && is_array($import[$get['#xml_plural']])) {
      if (isset($import[$get['#xml_plural']][$get['#xml_mapping']]) && is_array($import[$get['#xml_plural']][$get['#xml_mapping']])) {
        // MiniXML has an annoying habit of treating elements with a single
        // child differently to elements with multiple children. This little
        // hack makes the MiniXML array consistent, regardless of number of
        // child elements.
        if (!is_array($import[$get['#xml_plural']][$get['#xml_mapping']]) || !isset($import[$get['#xml_plural']][$get['#xml_mapping']]['_num'])) {
          $child = $import[$get['#xml_plural']][$get['#xml_mapping']];
          $import[$get['#xml_plural']][$get['#xml_mapping']] = array(0 => $child);
        }
        foreach ($import[$get['#xml_plural']][$get['#xml_mapping']] as $child_id => $child) {
          if (is_int($child_id)) {
            $field['#value'][$child_id] = array();

            foreach ($field as $child_index => $child) {
              if ($child_index != '#value') {
                $field['#value'][$child_id][$child_index] = $child;
              }
            }

            foreach (element_children($field) as $child_index) {
              $child_info = $field['#value'][$child_id][$child_index];
              $import_info = $import[$get['#xml_plural']][$get['#xml_mapping']][$child_id];

              // Recurse down through the children of this field.
              _importexportapi_xml_get($child_info, $import_info);

              $field['#value'][$child_id][$child_index] = $child_info;
            }
          }
        }
      }
    }
  }
}
