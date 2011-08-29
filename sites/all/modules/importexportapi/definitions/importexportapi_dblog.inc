<?php

/**
 * @file
 * Implements data definitions on behalf of watchdog.module.
 */

/**
 * Implementation of hook_def().
 */
function dblog_def() {
  $defs = array();

  $def = array(
    '#type' => 'entity',
    '#title' => t('Watchdog log message'),
    '#db_default_table' => 'watchdog',
    '#xml_plural' => 'watchdog-log',
    '#csv_plural' => 'watchdog-log'
  );

  $def['wid'] = array(
    '#type' => 'int',
    '#title' => t('Watchdog message ID'),
    '#key' => TRUE
  );
  $def['uid'] = array(
    '#type' => 'int',
    '#title' => t('User ID'),
    '#reference_entity' => 'user'
  );
  $def['message_type'] = array(
    '#title' => t('Message type'),
    '#db_field_unaliased' => 'type'
  );
  $def['message'] = array(
    '#title' => t('Message text'),
    '#xml_mapping' => 'message-text',
    '#csv_mapping' => 'message-text'
  );
  $def['severity'] = array(
    '#type' => 'int',
    '#title' => t('Severity')
  );
  $def['link'] = array(
    '#title' => t('Link')
  );
  $def['location'] = array(
    '#title' => t('Location URL')
  );
  $def['referer'] = array(
    '#title' => t('Referer URL')
  );
  $def['hostname'] = array(
    '#title' => t('Hostname')
  );
  $def['timestamp'] = array(
    '#type' => 'datetime',
    '#title' => t('Timestamp')
  );

  $defs['watchdog_message'] = $def;

  return $defs;
}

/**
 * Implementation of hook_db_def_tables().
 */
function dblog_db_def_tables() {
  $tables = array();

  $tables['watchdog'] = 'w';

  return $tables;
}
