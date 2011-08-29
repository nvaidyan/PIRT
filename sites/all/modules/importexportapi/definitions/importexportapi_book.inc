<?php

/**
 * @file
 * Implements data definitions on behalf of book.module.
 */

/**
 * Implementation of hook_def().
 */
function book_def() {
  $defs = array();

  $type = 'book';
  $def = importexportapi_node_get_def($type);

  $type_info = module_invoke($type, 'node_info');

  if (empty($type_info)) {
    return array();
  }

  $type_info = $type_info[$type];
  $def['#title'] = $type_info['name'];
  $def['#title'][0] = strtoupper($def['#title'][0]);

  $def['#xml_plural'] = $def['#csv_plural'] = 'book-pages';
  $def['revisions']['#csv_plural'] = 'book-revisions';

  $def['type']['#db_filter'] = array(
    'values' => array($type)
  );

  $def['revisions']['book_vid'] = array(
    '#type' => 'int',
    '#title' => t('Revision ID'),
    '#reference_entity' => $type,
    '#reference_field' => array('revisions', 'vid'),
    '#db_table' => 'book',
    '#db_field_unaliased' => 'vid',
    '#key_component' => TRUE,
    '#csv_hidden' => TRUE
  );
  $def['revisions']['book_nid'] = array(
    '#type' => 'int',
    '#title' => t('Node ID'),
    '#reference_entity' => $type,
    '#db_table' => 'book',
    '#db_field_unaliased' => 'nid',
    '#csv_hidden' => TRUE
  );
  $def['revisions']['parent'] = array(
    '#type' => 'int',
    '#title' => t('Parent'),
    '#reference_entity' => $type,
    '#reference_field' => array('nid'),
    '#reference_parent' => FALSE,
    '#reference_delta' => 1,
    '#db_table' => 'book'
  );
  $def['revisions']['weight'] = array(
    '#type' => 'int',
    '#title' => t('Weight'),
    '#db_table' => 'book'
  );

  $defs['book'] = $def;

  return $defs;
}

/**
 * Implementation of hook_db_def_tables().
 */
function book_db_def_tables() {
  $tables = array();

  $tables['book'] = 'b';

  return $tables;
}
