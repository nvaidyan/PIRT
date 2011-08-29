<?php

/**
 * @file
 * Implements data definitions on behalf of forum.module.
 */

/**
 * Implementation of hook_def().
 */
function forum_def() {
  $defs = array();

  $type = 'forum';
  $def = importexportapi_node_get_def($type);

  $type_info = module_invoke($type, 'node_info');
  $type_info = $type_info[$type];
  $def['#title'] = $type_info['name'];
  $def['#title'][0] = strtoupper($def['#title'][0]);

  $def['#xml_plural'] = $def['#csv_plural'] = 'forum-topics';
  $def['revisions']['#csv_plural'] = 'forum-revisions';

  $def['type']['#db_filter'] = array(
    'values' => array($type)
  );

  $def['revisions']['forum_vid'] = array(
    '#type' => 'int',
    '#title' => t('Revision ID'),
    '#reference_entity' => $type,
    '#reference_field' => array('revisions', 'vid'),
    '#db_table' => 'forum',
    '#db_field_unaliased' => 'vid',
    '#key_component' => TRUE,
    '#csv_hidden' => TRUE
  );
  $def['revisions']['forum_nid'] = array(
    '#type' => 'int',
    '#title' => t('Node ID'),
    '#reference_entity' => $type,
    '#reference_field' => array('nid'),
    '#db_table' => 'forum',
    '#db_field_unaliased' => 'nid',
    '#csv_hidden' => TRUE
  );
  $def['revisions']['forum_tid'] = array(
    '#type' => 'int',
    '#title' => t('Forum topic term ID'),
    '#reference_entity' => 'term',
    '#reference_field' => array('tid'),
    '#db_table' => 'forum',
    '#db_field_unaliased' => 'tid'
  );

  $defs['forum'] = $def;

  return $defs;
}

/**
 * Implementation of hook_db_def_tables().
 */
function forum_db_def_tables() {
  $tables = array();

  $tables['forum'] = 'fo';

  return $tables;
}
