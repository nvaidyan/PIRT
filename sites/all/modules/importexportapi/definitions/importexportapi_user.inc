<?php

/**
 * @file
 * Implements data definitions on behalf of user.module.
 */

/**
 * Implementation of hook_def().
 */
function user_def() {
  $defs = array();

  $def = array(
    '#type' => 'entity',
    '#title' => t('User role'),
    '#xml_plural' => 'roles',
    '#csv_plural' => 'roles',
    '#process' => array('_importexportapi_user_process_role' => array())
  );

  $def['rid'] = array(
    '#type' => 'int',
    '#title' => t('Role ID'),
    '#key' => TRUE
  );
  $def['role_name'] = array(
    '#title' => t('Role name'),
    '#alt_key_for' => 'rid',
    '#db_field_unaliased' => 'name'
  );
  $def['perm_rid'] = array(
    '#type' => 'int',
    '#title' => t('Role ID'),
    '#reference_entity' => 'role',
    '#reference_field' => array('rid'),
    '#db_table' => 'permission',
    '#db_field_unaliased' => 'rid',
    '#key_component' => TRUE,
    '#csv_hidden' => TRUE
  );
  $def['perm'] = array(
    '#type' => 'freeform',
    '#title' => t('Permission list'),
    '#default_value' => NULL,
    '#xml_plural' => 'permissions',
    '#csv_plural' => 'permissions',
    '#db_table' => 'permission'
  );

  $defs['role'] = $def;

  $def = array(
    '#type' => 'entity',
    '#title' => t('User'),
    '#db_default_table' => 'users',
    '#xml_plural' => 'users',
    '#csv_plural' => 'users'
  );

  $def['uid'] = array(
    '#type' => 'int',
    '#title' => t('User ID'),
    '#key' => TRUE,
    '#db_uses_sequences' => TRUE
  );
  $def['name'] = array(
    '#title' => t('Username'),
    '#alt_key_for' => 'uid'
  );
  $def['pass'] = array(
    '#title' => t('Password')
  );
  $def['mail'] = array(
    '#title' => t('E-mail address'),
    '#alt_key_for' => 'uid'
  );
  $def['theme'] = array(
    '#title' => t('Theme configuration')
  );
  $def['created'] = array(
    '#type' => 'datetime',
    '#title' => t('Member since')
  );
  $def['access'] = array(
    '#type' => 'datetime',
    '#title' => t('Last access')
  );
  $def['login'] = array(
    '#type' => 'datetime',
    '#title' => t('Last login')
  );
  $def['status'] = array(
    '#type' => 'int',
    '#title' => t('Status')
  );
  $def['timezone'] = array(
    '#type' => 'int',
    '#title' => t('Time zone'),
    '#default_value' => NULL
  );
  $def['picture'] = array(
    '#type' => 'file',
    '#title' => t('Picture')
  );

  $def['roles'] = array(
    '#type' => 'array',
    '#title' => t('Roles'),
    '#db_default_table' => 'users_roles',
    '#xml_plural' => 'roles',
    '#csv_plural' => 'users-roles',
    '#xml_mapping' => 'role'
  );
  $def['roles']['uid'] = array(
    '#type' => 'int',
    '#title' => t('User ID'),
    '#reference_entity' => 'user',
    '#key_component' => TRUE
  );
  $def['roles']['rid'] = array(
    '#type' => 'int',
    '#title' => t('Role ID'),
    '#reference_entity' => 'role',
    '#key_component' => TRUE
  );

  $def['auth'] = array(
    '#type' => 'array',
    '#title' => t('Remote auth'),
    '#db_default_table' => 'authmap',
    '#xml_plural' => 'authmaps',
    '#csv_plural' => 'authmaps'
  );
  $def['auth']['aid'] = array(
    '#type' => 'int',
    '#title' => t('Auth ID'),
    '#key' => TRUE
  );
  $def['auth']['uid'] = array(
    '#type' => 'int',
    '#title' => t('User ID'),
    '#reference_entity' => 'user',
    '#alt_key_ignore' => array('name')
  );
  $def['auth']['authname'] = array(
    '#title' => t('Auth name'),
    '#reference_entity' => 'user',
    '#reference_field' => array('name'),
    '#alt_key_ignore' => TRUE
  );
  $def['auth']['module'] = array(
    '#title' => t('Auth module'),
    '#required' => TRUE
  );

  $defs['user'] = $def;

  return $defs;
}

/**
 * Implementation of hook_db_def_tables().
 */
function user_db_def_tables() {
  $tables = array();

  $tables['users'] = 'u';
  $tables['users_roles'] = 'ur';
  $tables['authmap'] = 'am';
  $tables['role'] = 'r';
  $tables['permission'] = 'p';

  return $tables;
}

/**
 * Callback for processing of the 'perm_rid' field in the 'role' entity
 * definition.
 */
function _importexportapi_user_process_role($field_id, $field, $format, $op, $source_get) {
  if ($op == 'put' && $format == 'db') {
    if (empty($field['perm']['#value'])) {
      unset($field['perm_rid']);
      unset($field['perm']);
    }
    elseif (empty($field['perm_rid']['#value'])) {
      $field['perm_rid']['#value'] = $field['rid']['#value'];
    }
  }

  return $field;
}
