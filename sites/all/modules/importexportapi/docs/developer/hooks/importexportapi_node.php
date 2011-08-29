<?php

/**
 * @file
 * Implements hook documentation on behalf of node.module.
 */

/**
 * @addtogroup hooks
 * @{
 */


/**
 * Alter the data definition of the base node entity and its fields, as defined
 * in importexportapi_node_get_def().
 *
 * If your module needs to alter anything that affects the data definition of
 * all node types, then you should use this hook instead of hook_def_alter(),
 * because this hook allows you to make changes to the base node entity, before
 * its definition is copied to the node type entities.
 *
 * @param &$def
 *   The data definition of the node entity and its fields, as defined through
 *   importexportapi_node_get_def(). This parameter should be modified directly,
 *   as all changes to it are passed back by reference.
 * @param $type
 *   The node type name to use when referencing other fields (default is
 *   'node').
 */
function hook_node_def_alter(&$def, $type) {
  $def['taxonomy'] = array(
    '#type' => 'array',
    '#title' => t('Taxonomy terms'),
    '#db_default_table' => 'term_node',
    '#xml_plural' => 'taxonomy-terms',
    '#csv_plural' => 'taxonomy-terms',
    '#xml_mapping' => 'term'
  );
  $def['taxonomy']['nid'] = array(
    '#type' => 'int',
    '#title' => t('Node ID'),
    '#reference_entity' => $type
  );
  $def['taxonomy']['tid'] = array(
    '#type' => 'int',
    '#title' => t('Term ID'),
    '#reference_entity' => 'term',
    '#key_component' => TRUE
  );
}


/**
 * @} End of "addtogroup hooks".
 */
