<?php

/**
 * @file
 * Implements hook documentation on behalf of content.module.
 */

/**
 * @addtogroup hooks
 * @{
 */


/**
 * Alter the data definition of a CCK field, as defined by content_def().
 *
 * Modules that define CCK reference fields should use this hook instead of
 * hook_def_alter(), because it allows you to easily perform standard checks
 * and alterations on all CCK fields that exist on a site, and also because it
 * provides useful information about a CCK field, such as its CCK field type.
 *
 * @param &$field_def
 *   The data definition of a CCK field, as defined through content_def().
 *   This parameter should be modified directly, as all changes to it are passed
 *   back by reference.
 * @param $field_info
 *   An array containing information about the CCK field, such as its CCK field
 *   type.
 */
function hook_content_def_references(&$field_def, $field_info) {
  static $reference_delta = 1;

  if ($field_info['type'] == 'nodereference') {
    $field_def['#reference_entity'] = 'node';
    $field_def['#reference_field'] = array('nid');
    $field_def['#reference_delta'] = $reference_delta;

    $reference_delta++;
  }
}


/**
 * @} End of "addtogroup hooks".
 */
