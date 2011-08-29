<?php

/**
 * @file
 * Implements data definitions on behalf of nodereference.module.
 */

/**
 * Implementation of hook_content_def_references().
 */
function nodereference_content_def_references(&$field_def, $field_info) {
  static $reference_delta = 1;

  if ($field_info['type'] == 'nodereference') {
    $field_def['#reference_entity'] = 'node';
    $field_def['#reference_field'] = array('nid');
    $field_def['#reference_delta'] = $reference_delta;

    $reference_delta++;
  }
}
