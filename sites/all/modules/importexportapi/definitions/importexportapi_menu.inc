<?php

/**
 * @file
 * Implements data definitions on behalf of menu.module.
 */

/**
 * Implementation of hook_def().
 */
function menu_def() {
  $defs = array();

  $def = array(
    '#type' => 'entity',
    '#title' => t('Menu custom'),
    '#db_default_table' => 'menu_custom',
    '#xml_plural' => 'menu_custom_items',
    '#cvs_plural' => 'menu_custom_items',
  );

  $def['menu_name'] = array(
    '#title' => t('Menu name'),
    '#key' => TRUE,
    '#db_uses_sequences' => FALSE,
    '#cvs_mapping' => 'menu_name',
    '#xml_mapping' => 'menu_name',
    '#xml_plural' => 'menu_name',
  );

  $def['title'] = array(
    '#title' => t('Title'),
  );

  $def['description'] = array(
    '#title' => t('Description'),
  );

  $defs['menu_custom'] = $def;

  $def = array(
    '#type' => 'entity',
    '#title' => t('Menu links'),
    '#db_default_table' => 'menu_links',
    '#xml_plural' => 'menu_link_items',
    '#cvs_plural' => 'menu_link_items',
  );

  $def['menu_name'] = array(
    '#title' => t('Menu name'),
  );

  $def['mlid'] = array(
    '#type' => 'int',
    '#title' => t('Menu link id'),
    '#key' => TRUE,
    '#db_uses_sequences' => TRUE,
  );

  $def['plid'] = array(
    '#type' => 'int',
    '#title' => t('Parent link id'),
    '#reference_entity' => 'menu_links',
    '#reference_field' => array('mlid'),
    '#reference_parent' => FALSE,
  );

  $def['link_path'] = array(
    '#title' => t('Link path'),
  );

  $def['router_path'] = array(
    '#title' => t('Router path'),
  );

  $def['link_title'] = array(
    '#title' => t('Link title'),
  );

  $def['options'] = array(
    '#title' => t('Options'),
  );

  $def['module'] = array(
    '#title' => t('Module'),
  );

  $def['hidden'] = array(
    '#type' => 'int',
    '#title' => t('Hidden'),
  );

  $def['external'] = array(
    '#type' => 'int',
    '#title' => t('External'),
  );

  $def['has_children'] = array(
    '#type' => 'int',
    '#title' => t('Has children'),
  );

  $def['expanded'] = array(
    '#type' => 'int',
    '#title' => t('Expanded'),
  );

  $def['weight'] = array(
    '#type' => 'int',
    '#title' => t('Weight'),
  );

  $def['depth'] = array(
    '#type' => 'int',
    '#title' => t('Depth'),
  );

  $def['customized'] = array(
    '#type' => 'int',
    '#title' => t('Customized'),
  );

  $def['p1'] = array(
    '#type' => 'int',
    '#title' => t('P1'),
  );

  $def['p2'] = array(
    '#type' => 'int',
    '#title' => t('P2'),
  );

  $def['p3'] = array(
    '#type' => 'int',
    '#title' => t('P3'),
  );

  $def['p4'] = array(
    '#type' => 'int',
    '#title' => t('P4'),
  );

  $def['p5'] = array(
    '#type' => 'int',
    '#title' => t('P5'),
  );

  $def['p6'] = array(
    '#type' => 'int',
    '#title' => t('P6'),
  );

  $def['p7'] = array(
    '#type' => 'int',
    '#title' => t('P7'),
  );

  $def['p8'] = array(
    '#type' => 'int',
    '#title' => t('P8'),
  );

  $def['p9'] = array(
    '#type' => 'int',
    '#title' => t('P9'),
  );

  $def['updated'] = array(
    '#type' => 'int',
    '#title' => t('Updated'),
  );

  $defs['menu_links'] = $def;

  $def = array(
    '#type' => 'entity',
    '#title' => t('Menu router'),
    '#db_default_table' => 'menu_router',
    '#xml_plural' => 'menu_router_items',
    '#cvs_plural' => 'menu_router_items',
  );

  $def['path'] = array(
    '#title' => 'Path',
    '#key' => TRUE,
    '#db_uses_sequences' => FALSE,
  );

  $def['load_functions'] = array(
    '#title' => 'Load functions',
  );

  $def['to_arg_functions'] = array(
    '#title' => 'To arg functions',
  );

  $def['access_callback'] = array(
    '#title' => 'Access callback',
  );

  $def['access_arguments'] = array(
    '#type' => 'serialized',
    '#title' => 'Access arguments',
  );

  $def['fit'] = array(
    '#type' => 'int',
    '#title' => 'FIT',
  );

  $def['number_parts'] = array(
    '#type' => 'int',
    '#title' => 'Number parts',
  );

  $def['tab_parent'] = array(
    '#title' => 'Tab parent',
  );

  $def['tab_root'] = array(
    '#title' => 'Tab root',
  );

  $def['title'] = array(
    '#title' => 'Title',
  );

  $def['title_callback'] = array(
    '#title' => 'Title callback',
  );

  $def['title_arguments'] = array(
    '#title' => 'Title arguments',
  );

  $def['type'] = array(
    '#title' => 'Type',
  );

  $def['block_callback'] = array(
    '#title' => 'Block callback',
  );

  $def['description'] = array(
    '#title' => 'Description',
  );

  $def['position'] = array(
    '#title' => 'Position',
  );

  $def['weight'] = array(
    '#type' => 'int',
    '#title' => 'Weight',
  );

  $def['file'] = array(
    '#title' => 'File',
  );

  $defs['menu_router'] = $def;

  return $defs;
}

/**
 * Implementation of hook_db_def_tables().
 */
function menu_db_def_tables() {
  $tables = array();

  $tables['menu_custom'] = 'mc';
  $tables['menu_links'] = 'ml';
  $tables['menu_router'] = 'mr';

  return $tables;
}
