<?php

/**
 * @file
 * Implements data definitions on behalf of poll.module.
 */

/**
 * Implementation of hook_def().
 */
function poll_def() {
  $defs = array();

  $type = 'poll';
  $def = importexportapi_node_get_def($type);

  $type_info = module_invoke($type, 'node_info');
  $type_info = $type_info[$type];
  $def['#title'] = $type_info['name'];
  $def['#title'][0] = strtoupper($def['#title'][0]);

  $def['#xml_plural'] = $def['#csv_plural'] = 'polls';
  $def['revisions']['#csv_plural'] = 'poll-revisions';

  $def['type']['#db_filter'] = array(
    'values' => array($type)
  );

  $def['poll_nid'] = array(
    '#type' => 'int',
    '#title' => t('Node ID'),
    '#reference_entity' => $type,
    '#reference_field' => array('nid'),
    '#db_table' => 'poll',
    '#db_field_unaliased' => 'nid',
    '#key_component' => TRUE,
    '#csv_hidden' => TRUE
  );
  $def['poll_runtime'] = array(
    '#type' => 'int',
    '#title' => t('Poll duration'),
    '#db_table' => 'poll',
    '#db_field_unaliased' => 'runtime'
  );
  $def['poll_active'] = array(
    '#type' => 'int',
    '#title' => t('Poll currently active'),
    '#db_table' => 'poll',
    '#db_field_unaliased' => 'active'
  );

  $def['poll_choices'] = array(
    '#type' => 'array',
    '#title' => t('Poll choices'),
    '#xml_plural' => 'poll-choices',
    '#csv_plural' => 'poll-choices',
    '#xml_mapping' => 'choice'
  );
  $def['poll_choices']['chid'] = array(
    '#type' => 'int',
    '#title' => t('Choice ID'),
    '#key' => TRUE
  );
  $def['poll_choices']['nid'] = array(
    '#type' => 'int',
    '#title' => t('Node ID'),
    '#reference_entity' => $type
  );
  $def['poll_choices']['chtext'] = array(
    '#title' => t('Choice description')
  );
  $def['poll_choices']['chvotes'] = array(
    '#type' => 'int',
    '#title' => t('Choice votes')
  );
  $def['poll_choices']['chorder'] = array(
    '#type' => 'int',
    '#title' => t('Choice order')
  );

  $def['poll_votes'] = array(
    '#type' => 'array',
    '#title' => t('Poll votes'),
    '#xml_plural' => 'poll-votes',
    '#csv_plural' => 'poll-votes',
    '#xml_mapping' => 'vote'
  );
  $def['poll_votes']['nid'] = array(
    '#type' => 'int',
    '#title' => t('Node ID'),
    '#reference_entity' => $type
  );
  $def['poll_votes']['uid'] = array(
    '#type' => 'int',
    '#title' => t('User ID'),
    '#reference_entity' => 'user'
  );
  $def['poll_votes']['hostname'] = array(
    '#type' => 'int',
    '#title' => t('Hostname')
  );

  $defs['poll'] = $def;

  return $defs;
}

/**
 * Implementation of hook_db_def_tables().
 */
function poll_db_def_tables() {
  $tables = array();

  $tables['poll'] = 'po';
  $tables['poll_choices'] = 'poc';
  $tables['poll_votes'] = 'pov';

  return $tables;
}
