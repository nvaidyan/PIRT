<?php

/**
 * @file
 * Implements data definitions on behalf of statistics.module.
 */

/**
 * Implementation of hook_def().
 */
function statistics_def() {
  $defs = array();

  $def = array(
    '#type' => 'entity',
    '#title' => t('Access log message'),
    '#db_default_table' => 'accesslog',
    '#xml_plural' => 'access-log',
    '#csv_plural' => 'access-log'
  );

  $def['aid'] = array(
    '#type' => 'int',
    '#title' => t('Access log message ID'),
    '#key' => TRUE
  );
  $def['sid'] = array(
    '#title' => t('Session ID')
  );
  $def['title'] = array(
    '#title' => t('Page title')
  );
  $def['path'] = array(
    '#title' => t('System path')
  );
  $def['url'] = array(
    '#title' => t('Page URL')
  );
  $def['hostname'] = array(
    '#title' => t('Hostname')
  );
  $def['uid'] = array(
    '#type' => 'int',
    '#title' => t('User ID'),
    '#reference_entity' => 'user'
  );
  $def['page_generation_time'] = array(
    '#type' => 'int',
    '#title' => t('Page generation time'),
    '#db_field_unaliased' => 'timer'
  );
  $def['date_accessed'] = array(
    '#type' => 'datetime',
    '#title' => t('Date accessed'),
    '#db_field_unaliased' => 'timestamp'
  );

  $defs['access_log_message'] = $def;

  return $defs;
}

/**
 * Implementation of hook_node_def_alter().
 */
function statistics_node_def_alter(&$def, $type) {
  $def['counter_nid'] = array(
    '#type' => 'int',
    '#title' => t('Node ID'),
    '#reference_entity' => $type,
    '#reference_field' => array('nid'),
    '#db_table' => 'node_counter',
    '#db_field_unaliased' => 'nid',
    '#key_component' => TRUE,
    '#csv_hidden' => TRUE
  );
  $def['totalcount'] = array(
    '#type' => 'int',
    '#title' => t('Total page views'),
    '#db_table' => 'node_counter'
  );
  $def['daycount'] = array(
    '#type' => 'int',
    '#title' => t('Today\'s page views'),
    '#db_table' => 'node_counter'
  );
  $def['viewed'] = array(
    '#type' => 'int',
    '#title' => t('Last viewed date'),
    '#db_table' => 'node_counter',
    '#db_field_unaliased' => 'timestamp'
  );
}

/**
 * Implementation of hook_db_def_tables().
 */
function statistics_db_def_tables() {
  $tables = array();

  $tables['accesslog'] = 'al';
  $tables['node_counter'] = 'nc';

  return $tables;
}
