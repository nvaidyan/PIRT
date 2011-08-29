<?php

/**
 * @file
 * Implements data definitions on behalf of content.module.
 */

/**
 * Implementation of hook_def().
 */
function content_def() {
  $defs = array();

  $def = array(
    '#type' => 'entity',
    '#title' => t('Content field'),
    '#db_default_table' => 'node_field',
    '#xml_plural' => 'content-fields',
    '#csv_plural' => 'content-fields'
  );

  $def['field_name'] = array(
    '#title' => t('Field name'),
    '#key_component' => TRUE
  );
  $def['field_type'] = array(
    '#title' => t('Field type'),
    '#db_field_unaliased' => 'type'
  );
  $def['global_settings'] = array(
    '#type' => 'serialized',
    '#title' => t('Global settings')
  );
  $def['required'] = array(
    '#type' => 'int',
    '#title' => t('Required')
  );
  $def['multiple'] = array(
    '#type' => 'int',
    '#title' => t('Multiple values')
  );
  $def['db_storage'] = array(
    '#type' => 'int',
    '#title' => t('Uses database storage')
  );

  $defs['content_field'] = $def;

  $def = array(
    '#type' => 'entity',
    '#title' => t('Content type'),
    '#db_default_table' => 'node_type_content',
    '#xml_plural' => 'content-types',
    '#csv_plural' => 'content-types'
  );

  $def['type_name'] = array(
    '#title' => t('Type name'),
    '#key_component' => TRUE
  );
  $def['type_label'] = array(
    '#title' => t('Label'),
    '#db_field_unaliased' => 'label',
    '#alt_key_for' => 'type_name'
  );
  $def['description'] = array(
    '#title' => t('Description')
  );
  $def['help_text'] = array(
    '#title' => t('Help text'),
    '#db_field_unaliased' => 'help'
  );
  $def['title_label'] = array(
    '#title' => t('Title label')
  );

  $def['fields'] = array(
    '#type' => 'array',
    '#title' => t('Fields'),
    '#db_default_table' => 'node_field_instance',
    '#xml_plural' => 'fields',
    '#csv_plural' => 'content-field-instances',
    '#xml_mapping' => 'field'
  );
  $def['fields']['field_name'] = array(
    '#title' => t('Field name'),
    '#key_component' => TRUE,
    '#reference_entity' => 'content_field'
  );
  $def['fields']['type_name'] = array(
    '#title' => t('Type name'),
    '#key_component' => TRUE,
    '#reference_entity' => 'content_type'
  );
  $def['fields']['weight'] = array(
    '#type' => 'int',
    '#title' => t('Weight')
  );
  $def['fields']['label'] = array(
    '#title' => t('Label')
  );
  $def['fields']['widget_type'] = array(
    '#title' => t('Widget type')
  );
  $def['fields']['widget_settings'] = array(
    '#type' => 'serialized',
    '#title' => t('Widget settings')
  );
  $def['fields']['description'] = array(
    '#title' => t('Description')
  );

  $defs['content_type'] = $def;

  $content_types = content_types();

  foreach ($content_types as $content_type_name => $content_type_info) {
    // In 5.x everythings cck type, so exclude the basics
    if (isset($content_type_info['orig_type'])) {
      continue;
    }

    $content_type_name_system = str_replace('-', '_', $content_type_name);
    $content_type_name_mapping = str_replace('_', '-', $content_type_name);

    $def = importexportapi_node_get_def($content_type_name_system);
    $table_info = array();

    $def['#title'] = $content_type_info['name'];

    $def['#xml_plural'] = $def['#csv_plural'] = $content_type_name_mapping .'-collection';
    $def['revisions']['#csv_plural'] = $content_type_name_mapping .'-revisions';

    $def['type']['#db_filter'] = array(
      'values' => array($content_type_name)
    );

    foreach ($content_type_info['fields'] as $field_name => $field_info) {
      $table_info += _importexportapi_content_generate_fields($def, $field_info);
    }

    foreach ($table_info as $table => $table_info) {
      _importexportapi_content_add_parent_references($def, $table, $table_info, $content_type_name_system);
    }

    $defs[$content_type_name_system] = $def;
  }

  return $defs;
}

/**
 * Implementation of hook_db_def_tables().
 */
function content_db_def_tables() {
  $tables = array();

  $tables['node_type_content'] = 'nt';
  $tables['node_field'] = 'nf';
  $tables['node_field_instance'] = 'nfi';

  $content_fields = content_fields();

  foreach ($content_fields as $field_info) {
    $database_info = content_database_info($field_info);
    $db_table = $database_info['table'];

    if (!isset($tables[$db_table])) {
      $tables[$db_table] = $db_table;
    }
  }

  return $tables;
}

function _importexportapi_content_generate_fields(&$def, $field_info) {
  $database_info = content_database_info($field_info);

  // Boolean flags.
  $multiple_columns = (count($database_info['columns']) > 1);
  $storage_per_field = ($field_info['db_storage'] == CONTENT_DB_STORAGE_PER_FIELD);
  $multiple_values = ($field_info['multiple']);

  $base_title = !empty($field_info['widget']['label']) ? $field_info['widget']['label'] : $field_info['field_name'];
  $field_name = NULL;
  $ret_field_name = NULL;

  foreach ($database_info['columns'] as $column) {
    $sub_field_array = explode('_', $column['column']);
    $sub_field = end($sub_field_array);

    $name = substr($column['column'], 6);
    $field_title = $title = $base_title;
    $field_type = $type = _importexportapi_content_map_cck_db_type($column['type']);
    $field_already_set = TRUE;

    if ($multiple_columns && !$multiple_values) {
      $field_title .= ' ('. $sub_field .')';
    }

    if ($multiple_values) {
      $field_name = substr($name, 0, ((strlen($name) - strlen($sub_field)) - 1));
      $field_type = 'array';
      $field_title .= ' (collection)';
      $title .= ' ('. $sub_field .')';
    }
    else {
      $field_name = $name;
      $ret_field_name = substr($name, 0, ((strlen($name) - strlen($sub_field)) - 1));
    }

    if (!isset($def['revisions'][$field_name])) {
      $def['revisions'][$field_name] = array(
        '#type' => $field_type,
        '#title' => $field_title
      );

      $field_already_set = FALSE;
    }

    if ($multiple_values) {
      if (!$field_already_set) {
        $field_name_mapping = str_replace('_', '-', $field_name);
        $def['revisions'][$field_name]['#db_default_table'] = $database_info['table'];
        $def['revisions'][$field_name]['#xml_plural'] = $def['revisions'][$field_name]['#csv_plural'] = $field_name_mapping .'-collection';
        $def['revisions'][$field_name]['#xml_mapping'] = $field_name_mapping;
      }

      $def['revisions'][$field_name][$name] = array(
        '#type' => $type,
        '#title' => $title,
        '#db_field_unaliased' => $column['column']
      );

      _importexportapi_content_invoke_def_references($def['revisions'][$field_name][$name], $field_info);
    }
    else {
      $def['revisions'][$field_name]['#db_field_unaliased'] = $column['column'];
      $def['revisions'][$field_name]['#db_table'] = $database_info['table'];

      _importexportapi_content_invoke_def_references($def['revisions'][$field_name], $field_info);
    }
  }

  $table_info = array(
    'field_name' => (isset($ret_field_name) ? $ret_field_name : $field_name),
    'multiple_values' => $multiple_values,
    'storage_per_field' => $storage_per_field,
    'db_table' => $database_info['table']
  );

  return array($database_info['table'] => $table_info);
}

function _importexportapi_content_map_cck_db_type($type) {
  $new_type = $type;

  switch ($type) {
    case 'varchar':
    case 'longtext':
      $new_type = 'string';
      break;
    default:
      break;
  }

  return $new_type;
}

function _importexportapi_content_add_parent_references(&$def, $table, $table_info, $content_type_name_system) {
  $field_name = $table_info['field_name'];

  if ($table_info['multiple_values']) {
    $def['revisions'][$field_name]['vid'] = array(
      '#type' => 'int',
      '#title' => t('Revision ID'),
      '#reference_entity' => $content_type_name_system,
      '#reference_field' => array('revisions', 'vid'),
      '#key_component' => TRUE
    );
    $def['revisions'][$field_name]['nid'] = array(
      '#type' => 'int',
      '#title' => t('Node ID'),
      '#reference_entity' => $content_type_name_system
    );
    $def['revisions'][$field_name]['delta'] = array(
      '#type' => 'int',
      '#title' => t('Delta'),
      '#key_component' => TRUE
    );
  }
  else {
    $def['revisions'][$field_name .'_vid'] = array(
      '#type' => 'int',
      '#title' => t('Revision ID'),
      '#reference_entity' => $content_type_name_system,
      '#reference_field' => array('revisions', 'vid'),
      '#db_table' => $table_info['db_table'],
      '#db_field_unaliased' => 'vid',
      '#key_component' => TRUE,
      '#csv_hidden' => TRUE
    );
    $def['revisions'][$field_name .'_nid'] = array(
      '#type' => 'int',
      '#title' => t('Node ID'),
      '#reference_entity' => $content_type_name_system,
      '#db_table' => $table_info['db_table'],
      '#db_field_unaliased' => 'nid',
      '#csv_hidden' => TRUE
    );
  }
}

function _importexportapi_content_invoke_def_references(&$field_def, $field_info) {
  foreach (module_implements('content_def_references') as $module) {
    $function = $module .'_content_def_references';
    $function($field_def, $field_info);
  }
}
