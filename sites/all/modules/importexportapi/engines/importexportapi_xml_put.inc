<?php

/**
 * @file
 * The 'xml' 'put' engine for the Import / Export API.
 */

require_once(drupal_get_path('module', 'importexportapi') .'/engines/importexportapi_xml.inc');

/**
 * Callback for rendering data in XML export format.
 */
function importexportapi_xml_put($data, $variables) {
  $export = array();
  $entity_count = array();
  $use_cdata = isset($variables['use_cdata']) ? $variables['use_cdata'] : TRUE;

  // Convert the structured data into an array in MiniXML format.
  foreach ($data as $entity) {
    $put = $entity['#put'];
    if (!isset($entity_count[$entity['#id']])) {
      $entity_count[$entity['#id']] = 0;
    }

    if (!isset($export[$put['#xml_plural']])) {
      $export[$put['#xml_plural']] = array();
      $export[$put['#xml_plural']][$put['#xml_mapping']] = array();
    }
    $export[$put['#xml_plural']][$put['#xml_mapping']][$entity_count[$entity['#id']]] = array();

    foreach (element_children($entity) as $field_index) {
      // Recurse down through children, and add all child values to the MiniXML
      // array.
      _importexportapi_xml_put($export[$put['#xml_plural']][$put['#xml_mapping']][$entity_count[$entity['#id']]], $entity[$field_index], $use_cdata);
    }

    if (empty($export[$put['#xml_plural']][$put['#xml_mapping']])) {
      unset($export[$put['#xml_plural']]);
    }

    $entity_count[$entity['#id']]++;
  }

  $export = array('drupal' => $export);

  // Let MiniXML do the hard work.
  $xml_doc = _importexportapi_include_minixml();
  $xml_doc->fromArray($export);

  $return = $xml_doc->toString();
  unset($xml_doc);

  return $return;
}

/**
 * Helper function for importexportapi_xml_put(). Converts the specified
 * field and its children into a MiniXML structured array fragment.
 *
 * @param &$export
 *   The MiniXML structured array fragment to populate with values. These
 *   values are passed back by reference.
 * @param $field
 *   The field from which to assign values to the MiniXML structured array.
 *   The children of this field are recursively added as well.
 */
function _importexportapi_xml_put(&$export, $field, $use_cdata) {
  $put = $field['#put'];

  if ($field['#type'] != 'array') {
    if (isset($field['#value']) && $field['#value'] !== '' && empty($put['#xml_hidden'])) {
      if ($use_cdata && _importexportapi_xml_put_check_specialchars($field['#value'])) {
        _importexportapi_xml_put_wrap_cdata($field['#value']);
      }

      // Assign this field to the MiniXML array.
      if ($field['#type'] == 'freeform') {
        $export[$put['#xml_plural']] = array($put['#xml_mapping'] => $field['#value']);
      }
      else {
        $export[$put['#xml_mapping']] = $field['#value'];
      }
    }
  }
  else {
    $export[$put['#xml_plural']] = array();
    $export[$put['#xml_plural']][$put['#xml_mapping']] = array();

    foreach (element_children($field['#value']) as $child) {
      $export[$put['#xml_plural']][$put['#xml_mapping']][$child] = array();
      foreach (element_children($field['#value'][$child]) as $child_field) {
        $child_field_data = $field['#value'][$child][$child_field];
        $put_child = $child_field_data['#put'];
        if ($child_field_data['#type'] == 'array') {
          // Recurse down through the children of this field.
          _importexportapi_xml_put($export[$put['#xml_plural']][$put['#xml_mapping']][$child], $child_field_data, $use_cdata);
        }
        elseif (isset($child_field_data['#value']) && $child_field_data['#value'] !== '' && empty($put_child['#xml_hidden'])) {
          if ($use_cdata && _importexportapi_xml_put_check_specialchars($child_field_data['#value'])) {
            _importexportapi_xml_put_wrap_cdata($child_field_data['#value']);
          }

          // Assign children of array fields to the MiniXML array.
          if ($child_field_data['#type'] == 'freeform') {
            $export[$put['#xml_plural']][$put['#xml_mapping']][$child][$put_child['#xml_plural']] = array($put_child['#xml_mapping'] => $child_field_data['#value']);
          }
          else {
            $export[$put['#xml_plural']][$put['#xml_mapping']][$child][$put_child['#xml_mapping']] = $child_field_data['#value'];
          }
        }
      }
    }

    if (empty($export[$put['#xml_plural']][$put['#xml_mapping']])) {
      unset($export[$put['#xml_plural']]);
    }
  }
}

/**
 * Wraps a value inside an XML CDATA structure. Works with flat values, and
 * works recursively on array values.
 *
 * @param &$value
 *   The array or flat value to wrap in a CDATA structure. This is passed back
 *   by reference.
 */
function _importexportapi_xml_put_wrap_cdata(&$value) {
  if (isset($value)) {
    if (is_array($value)) {
      foreach (array_keys($value) as $value_key) {
        _importexportapi_xml_put_wrap_cdata($value[$value_key]);
      }
    }
    else {
      $value = '<![CDATA['. $value .']]>';
    }
  }
}

/**
 * Checks for special XML characters within the specified value, which would
 * indicate that the value needs to be wrapped in a CDATA structure.
 *
 * @param $value
 *   The array or flat value to check for special characters.
 * @param $specialchars
 *   Internal use only - tracks whether or not special characters have been
 *   found inside the specified value.
 * @return
 *   Boolean TRUE if special characters were found, FALSE otherwise.
 */
function _importexportapi_xml_put_check_specialchars($value, $specialchars = FALSE) {
  if (isset($value)) {
    if (is_array($value)) {
      foreach (array_keys($value) as $value_key) {
        $specialchars = _importexportapi_xml_put_check_specialchars($value_key, $specialchars);

        if ($specialchars) {
          break;
        }
      }
    }
    else {
      return preg_match('/[&<>\'"]/', $value);
    }
  }
  else {
    return FALSE;
  }

  return $specialchars;
}
