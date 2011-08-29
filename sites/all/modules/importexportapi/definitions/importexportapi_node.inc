<?php

/**
 * @file
 * Implements data definitions on behalf of node.module.
 */

/**
 * Implementation of hook_def().
 */
function node_def() {
  $defs = array();

  $def = importexportapi_node_get_def();
  $def['#abstract'] = TRUE;

  $defs['node'] = $def;

  $def = array(
    '#type' => 'entity',
    '#title' => t('Node access rule'),
    '#xml_plural' => 'node-access',
    '#xml_mapping' => 'node-access-rule',
    '#csv_plural' => 'node-access'
  );

  $def['nid'] = array(
    '#type' => 'int',
    '#title' => t('Node ID'),
    '#key_component' => TRUE,
    '#reference_entity' => 'node'
  );
  $def['gid'] = array(
    '#type' => 'int',
    '#title' => t('Group ID'),
    '#key_component' => TRUE
  );
  $def['realm'] = array(
    '#title' => t('Realm'),
    '#key_component' => TRUE
  );
  $def['grant_view'] = array(
    '#type' => 'int',
    '#title' => t('Grant view')
  );
  $def['grant_update'] = array(
    '#type' => 'int',
    '#title' => t('Grant update')
  );
  $def['grant_delete'] = array(
    '#type' => 'int',
    '#title' => t('Grant delete')
  );

  $defs['node_access'] = $def;
  
  $def = array(
    '#type' => 'entity',
    '#title' => t('Node type'),
    '#xml_plural' => 'node-types',
    '#xml_mapping' => 'node-type',
    '#csv_plural' => 'node-types'
  );
  
  $def['type'] = array(
    '#title' => t('Name (machine-readable)'),
    '#key_component' => TRUE
  );
  $def['name'] = array(
    '#title' => t('Name (human-readable)'),
    '#alt_key_for' => 'type'
  );
  $def['module'] = array(
    '#title' => t('Responsible module')
  );
  $def['description'] = array(
    '#title' => t('Description')
  );
  $def['help'] = array(
    '#title' => t('Explanation or submission guidelines')
  );
  $def['has_title'] = array(
    '#type' => 'int',
    '#title' => t('Has title field')
  );
  $def['title_label'] = array(
    '#title' => t('Title field label')
  );
  $def['has_body'] = array(
    '#type' => 'int',
    '#title' => t('Has body field')
  );
  $def['body_label'] = array(
    '#title' => t('Body field label')
  );
  $def['min_word_count'] = array(
    '#type' => 'int',
    '#title' => t('Minimum word count')
  );
  $def['custom'] = array(
    '#type' => 'int',
    '#title' => t('Is user-defined node type')
  );
  $def['modified'] = array(
    '#type' => 'int',
    '#title' => t('Has been modified')
  );
  $def['locked'] = array(
    '#type' => 'int',
    '#title' => t('Has locked machine-readable name')
  );
  $def['orig_type'] = array(
    '#title' => t('Original name (machine-readable)')
  );

  $defs['node_type'] = $def;

  $node_types = node_get_types();
  foreach ($node_types as $type => $info) {
    if ($info->module == 'node') {
      $def = importexportapi_node_get_def($type);

      $def['#title'] = $info->name;
      $def['#title'][0] = strtoupper($def['#title'][0]);

      $def['#xml_plural'] = $def['#csv_plural'] = $type .'-collection';
      $def['revisions']['#csv_plural'] = $type .'-revisions';

      $def['type']['#db_filter'] = array(
        'values' => array($type)
      );

      $defs[$type] = $def;
    }
  }

  return $defs;
}

/**
 * Implementation of hook_db_def_tables().
 */
function node_db_def_tables() {
  $tables = array();

  $tables['node'] = 'n';
  $tables['node_revisions'] = 'nr';
  $tables['node_access'] = 'na';
  $tables['node_type'] = 'nt';

  return $tables;
}

/**
 * Provides the data definition for the base node entity. This entity should
 * not be used directly (e.g. as part of a 'get' or a 'put' operation), except
 * when defining references to its fields. Instead, use the entities provided
 * for each individual node type. Modules should extend and modify this
 * definition in order to build the definitions for individual node types.
 *
 * @param $type
 *   The node type name to use when referencing other fields (default is
 *   'node').
 *
 * @return
 *   An array representing the data definition for the base node entity.
 */
function importexportapi_node_get_def($type = 'node') {
  $def = array(
    '#type' => 'entity',
    '#title' => t('Node'),
    '#db_default_table' => 'node',
    '#xml_plural' => 'nodes',
    '#csv_plural' => 'nodes',
    '#db_process_generate_id' => array('importexportapi_node_process_generate_id' => array())
  );

  $def['nid'] = array(
    '#type' => 'int',
    '#title' => t('Node ID'),
    '#key' => TRUE,
    '#db_uses_sequences' => TRUE
  );
  $def['vid'] = array(
    '#type' => 'int',
    '#title' => t('Revision ID'),
    '#reference_entity' => $type,
    '#reference_field' => array('revisions', 'vid'),
    '#key_component' => TRUE
  );
  $def['type'] = array(
    '#title' => t('Type'),
    '#reference_entity' => 'node_type'
  );
  $def['title'] = array(
    '#title' => t('Title'),
    '#alt_key_for' => 'nid'
  );
  $def['uid'] = array(
    '#type' => 'int',
    '#title' => t('User ID'),
    '#reference_entity' => 'user'
  );
  $def['status'] = array(
    '#type' => 'int',
    '#title' => t('Published'),
    '#default_value' => 1
  );
  $def['created'] = array(
    '#type' => 'datetime',
    '#title' => t('Creation date')
  );
  $def['changed'] = array(
    '#type' => 'datetime',
    '#title' => t('Last changed date')
  );
  $def['promote'] = array(
    '#type' => 'int',
    '#title' => t('Promoted to front page')
  );
  $def['moderate'] = array(
    '#type' => 'int',
    '#title' => t('In moderation queue')
  );
  $def['sticky'] = array(
    '#type' => 'int',
    '#title' => t('Sticky at top of lists')
  );

  $def['revisions'] = array(
    '#type' => 'array',
    '#title' => t('Revisions'),
    '#db_default_table' => 'node_revisions',
    '#xml_plural' => 'revisions',
    '#xml_mapping' => 'revision',
    '#csv_plural' => 'node-revisions'
  );
  $def['revisions']['nid'] = array(
    '#type' => 'int',
    '#title' => t('Node ID'),
    '#reference_entity' => $type
  );
  $def['revisions']['vid'] = array(
    '#type' => 'int',
    '#title' => t('Revision ID'),
    '#key' => TRUE,
    '#db_uses_sequences' => TRUE
  );
  $def['revisions']['uid'] = array(
    '#type' => 'int',
    '#title' => t('User ID'),
    '#reference_entity' => 'user'
  );
  $def['revisions']['title_revision'] = array(
    '#title' => t('Title'),
    '#alt_key_for' => 'vid',
    '#db_field_unaliased' => 'title'
  );
  $def['revisions']['body'] = array(
    '#title' => t('Body')
  );
  $def['revisions']['teaser'] = array(
    '#title' => t('Teaser')
  );
  $def['revisions']['log'] = array(
    '#title' => t('Log message')
  );
  $def['revisions']['timestamp'] = array(
    '#type' => 'datetime',
    '#title' => t('Last changed date')
  );

  // Allow modules to alter the base node .
  foreach (module_implements('node_def_alter') as $module) {
    $function = $module .'_node_def_alter';
    $function($def, $type);
  }

  _importexportapi_node_set_abstract($def);

  return $def;
}

function importexportapi_node_process_generate_id($data) {
  if (isset($data['revisions']['#value'])) {
    $max_revision = count($data['revisions']['#value']) - 1;
    if (isset($data['revisions']['#value'][$max_revision]['vid']['#value']) && !isset($data['vid']['#value'])) {
      $data['vid']['#value'] = $data['revisions']['#value'][$max_revision]['vid']['#value'];
    }
  }

  return $data;
}

function _importexportapi_node_set_abstract(&$def) {
  foreach (element_children($def) as $child_index) {
    $def[$child_index]['#abstract_parent'] = 'node';

    if (isset($def[$child_index]['#type']) && $def[$child_index]['#type'] == 'array') {
      _importexportapi_node_set_abstract($def[$child_index]);
    }
  }
}
