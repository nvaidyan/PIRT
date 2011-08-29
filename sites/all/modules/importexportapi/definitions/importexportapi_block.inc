<?php

/**
 * @file
 * Implements data definitions on behalf of block.module.
 */

/**
 * Implementation of hook_def().
 */
function block_def() {
  $defs = array();

  $def = _block_get_def();

  $defs['block'] = $def;

  $defs['block']['module']['#db_filter'] = array(
    'values' => array('block'),
    'operator' => 'neq'
  );

  $def['bid'] = array(
    '#type' => 'int',
    '#title' => t('Box ID'),
    '#db_table' => 'boxes',
    '#key' => TRUE
  );
  $def['body'] = array(
    '#title' => t('Body'),
    '#db_table' => 'boxes'
  );
  $def['info'] = array(
    '#title' => t('Description'),
    '#unique' => TRUE,
    '#db_table' => 'boxes'
  );
  $def['format'] = array(
    '#type' => 'int',
    '#title' => t('Input format ID'),
    '#reference_entity' => 'input_format',
    '#db_table' => 'boxes'
  );

  $def['delta']['#reference_entity'] = 'block_custom';
  $def['delta']['#reference_field'] = array('bid');

  $def['module']['#db_filter'] = array(
    'values' => array('block')
  );

  $def['#title'] = t('Block (custom)');
  $def['#xml_plural'] = $def['#csv_plural'] = 'blocks-custom';

  $defs['block_custom'] = $def;

  return $defs;
}

/**
 * Implementation of hook_db_def_tables().
 */
function block_db_def_tables() {
  $tables = array();

  $tables['blocks'] = 'bl';
  $tables['blocks_roles'] = 'blr';
  $tables['boxes'] = 'bo';

  return $tables;
}

/**
 * Returns the base definition of a block.
 *
 * @return
 *   Array containing the block definition.
 */
function _block_get_def() {
  $def = array(
    '#type' => 'entity',
    '#title' => t('Block'),
    '#db_default_table' => 'blocks',
    '#xml_plural' => 'blocks',
    '#csv_plural' => 'blocks'
  );

  $def['module'] = array(
    '#title' => t('Responsible module'),
    '#key_component' => TRUE
  );
  $def['delta'] = array(
    '#title' => t('Delta'),
    '#key_component' => TRUE
  );
  $def['theme'] = array(
    '#title' => t('Theme'),
    '#key_component' => TRUE
  );
  $def['status'] = array(
    '#type' => 'int',
    '#title' => t('Enabled')
  );
  $def['weight'] = array(
    '#type' => 'int',
    '#title' => t('Weight')
  );
  $def['region'] = array(
    '#title' => t('Region')
  );
  $def['custom'] = array(
    '#type' => 'int',
    '#title' => t('Custom')
  );
  $def['throttle'] = array(
    '#type' => 'int',
    '#title' => t('Throttle')
  );
  $def['visibility'] = array(
    '#type' => 'int',
    '#title' => t('Visibility')
  );
  $def['pages'] = array(
    '#title' => t('Pages')
  );
  $def['title'] = array(
    '#title' => t('Title')
  );
  $def['roles'] = array(
    '#type' => 'array',
    '#title' => t('Roles'),
    '#db_default_table' => 'blocks_roles',
    '#xml_plural' => 'roles',
    '#csv_plural' => 'blocks-roles',
    '#xml_mapping' => 'role'
  );
  $def['roles']['module'] = array(
    '#title' => t('Responsible module'),
    '#reference_entity' => 'block',
    '#key_component' => TRUE
  );
  $def['roles']['delta'] = array(
    '#title' => t('Delta'),
    '#reference_entity' => 'block',
    '#key_component' => TRUE
  );
  $def['roles']['rid'] = array(
    '#type' => 'int',
    '#title' => t('Role ID'),
    '#reference_entity' => 'role',
    '#key_component' => TRUE
  );

  return $def;
}
