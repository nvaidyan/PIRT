<?php

/**
 * @file
 * Implements data definitions on behalf of taxonomy.module.
 */

/**
 * Implementation of hook_def().
 */
function taxonomy_def() {
  $defs = array();

  $def = array(
    '#type' => 'entity',
    '#title' => t('Vocabulary'),
    '#xml_plural' => 'vocabularies',
    '#csv_plural' => 'vocabularies'
  );

  $def['vid'] = array(
    '#type' => 'int',
    '#title' => t('Vocabulary ID'),
    '#key' => TRUE,
    '#db_uses_sequences' => TRUE
  );
  $def['vocabulary_name'] = array(
    '#title' => t('Vocabulary name'),
    '#alt_key_for' => 'vid',
    '#db_field_unaliased' => 'name'
  );
  $def['description'] = array(
    '#title' => t('Description')
  );
  $def['help'] = array(
    '#title' => t('Help text')
  );
  $def['relations'] = array(
    '#type' => 'int',
    '#title' => t('Related terms')
  );
  $def['hierarchy'] = array(
    '#type' => 'int',
    '#title' => t('Hierarchy')
  );
  $def['multiple'] = array(
    '#type' => 'int',
    '#title' => t('Multiple select')
  );
  $def['required'] = array(
    '#type' => 'int',
    '#title' => t('Required')
  );
  $def['tags'] = array(
    '#type' => 'int',
    '#title' => t('Free tagging')
  );
  $def['module'] = array(
    '#title' => t('Responsible module')
  );
  $def['weight'] = array(
    '#type' => 'int',
    '#title' => t('Weight')
  );

  $def['types'] = array(
    '#type' => 'array',
    '#title' => t('Node types'),
    '#db_default_table' => 'vocabulary_node_types',
    '#xml_plural' => 'types',
    '#csv_plural' => 'vocabulary-types',
    '#xml_mapping' => 'type'
  );
  $def['types']['vid'] = array(
    '#type' => 'int',
    '#title' => t('Vocabulary ID'),
    '#reference_entity' => 'vocabulary',
    '#key_component' => TRUE
  );
  $def['types']['type_name'] = array(
    '#title' => t('Node type'),
    '#db_field_unaliased' => 'type',
    '#key_component' => TRUE,
    '#reference_entity' => 'node_type',
    '#reference_field' => array('type')
  );

  $defs['vocabulary'] = $def;

  $def = array(
    '#type' => 'entity',
    '#title' => t('Term'),
    '#db_default_table' => 'term_data',
    '#xml_plural' => 'terms',
    '#csv_plural' => 'terms',
    '#process' => array(
      '_importexportapi_taxonomy_process_term_parents' => array()
    )
  );

  $def['tid'] = array(
    '#type' => 'int',
    '#title' => t('Term ID'),
    '#key' => TRUE,
    '#db_uses_sequences' => TRUE
  );
  $def['vid'] = array(
    '#type' => 'int',
    '#title' => t('Vocabulary ID'),
    '#reference_entity' => 'vocabulary'
  );
  $def['term_name'] = array(
    '#title' => t('Term name'),
    '#alt_key_for' => 'tid',
    '#db_field_unaliased' => 'name'
  );
  $def['description'] = array(
    '#title' => t('Description')
  );
  $def['weight'] = array(
    '#type' => 'int',
    '#title' => t('Weight')
  );

  $def['parents'] = array(
    '#type' => 'array',
    '#title' => t('Parents'),
    '#db_default_table' => 'term_hierarchy',
    '#xml_plural' => 'parents',
    '#csv_plural' => 'term-parents',
    '#xml_mapping' => 'parent-instance'
  );
  $def['parents']['tid'] = array(
    '#type' => 'int',
    '#title' => t('Term ID'),
    '#reference_entity' => 'term',
    '#key_component' => TRUE
  );
  $def['parents']['parent'] = array(
    '#type' => 'int',
    '#title' => t('Parent term ID'),
    '#reference_entity' => 'term',
    '#reference_field' => array('tid'),
    '#key_component' => TRUE,
    '#reference_delta' => 1,
    '#reference_parent' => FALSE
  );

  $def['relations'] = array(
    '#type' => 'array',
    '#title' => t('Related terms'),
    '#db_default_table' => 'term_relation',
    '#xml_plural' => 'relations',
    '#csv_plural' => 'term-relations',
    '#xml_mapping' => 'relation-instance'
  );
  $def['relations']['tid'] = array(
    '#type' => 'int',
    '#title' => t('Term ID'),
    '#db_field_unaliased' => 'tid1',
    '#reference_entity' => 'term'
  );
  $def['relations']['relation'] = array(
    '#type' => 'int',
    '#title' => t('Related term ID'),
    '#reference_entity' => 'term',
    '#reference_field' => array('tid'),
    '#db_field_unaliased' => 'tid2',
    '#reference_delta' => 1,
    '#reference_parent' => FALSE
  );

  $def['synonyms'] = array(
    '#type' => 'array',
    '#title' => t('Synonyms'),
    '#db_default_table' => 'term_synonym',
    '#xml_plural' => 'synonyms',
    '#csv_plural' => 'term-synonyms',
    '#xml_mapping' => 'synonym'
  );
  $def['synonyms']['tid'] = array(
    '#type' => 'int',
    '#title' => t('Term ID'),
    '#reference_entity' => 'term'
  );
  $def['synonyms']['synonym_name'] = array(
    '#title' => t('Synonym name'),
    '#reference_entity' => 'term',
    '#reference_field' => array('term_name'),
    '#alt_key_ignore' => TRUE,
    '#db_field_unaliased' => 'name',
    '#reference_parent' => FALSE
  );

  $defs['term'] = $def;

  return $defs;
}

/**
 * Implementation of hook_node_def_alter().
 */
function taxonomy_node_def_alter(&$def, $type) {
  $def['taxonomy'] = array(
    '#type' => 'array',
    '#title' => t('Taxonomy terms'),
    '#db_default_table' => 'term_node',
    '#xml_plural' => 'taxonomy-terms',
    '#csv_plural' => 'taxonomy-terms',
    '#xml_mapping' => 'term',
  );
  $def['taxonomy']['nid'] = array(
    '#type' => 'int',
    '#title' => t('Node ID'),
    '#reference_entity' => $type,
    '#key_component' => TRUE,
  );
  $def['taxonomy']['vid'] = array(
    '#type' => 'int',
    '#title' => t('Node Revision'),
    '#reference_entity' => 'vid',
    '#key_component' => TRUE,
  );
  $def['taxonomy']['tid'] = array(
    '#type' => 'int',
    '#title' => t('Term ID'),
    '#reference_entity' => 'term',
    '#key_component' => TRUE,
  );
}

/**
 * Implementation of hook_db_def_tables().
 */
function taxonomy_db_def_tables() {
  $tables = array();

  $tables['term_data'] = 'td';
  $tables['term_hierarchy'] = 'th';
  $tables['term_node'] = 'tn';
  $tables['term_relation'] = 'tr';
  $tables['term_synonym'] = 'ts';
  $tables['vocabulary'] = 'v';
  $tables['vocabulary_node_types'] = 'vnt';

  return $tables;
}

/**
 * Callback for processing of a term's parents.
 */
function _importexportapi_taxonomy_process_term_parents($field_id, $field, $format, $op, $source_get) {
  if ($op == 'put' && $format == 'db' && empty($field['parents']['#value'])) {
    $field['parents']['#value'][0] = importexportapi_get_def(array('term', 'parents'));
    unset($field['parents']['#value'][0]['#value']);

    $field['parents']['#value'][0]['tid']['#value'] = isset($field['tid']['#value']) ? $field['tid']['#value'] : NULL;
    $field['parents']['#value'][0]['parent']['#value'] = 0;
  }

  return $field;
}
