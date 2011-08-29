<?php

/**
 * @file
 * Implements data definitions on behalf of locale.module.
 */

/**
 * Implementation of hook_def().
 */
function locale_def() {
  $defs = array();

  $def = array(
    '#type' => 'entity',
    '#title' => t('Locale source string'),
    '#db_default_table' => 'locales_source',
    '#xml_plural' => 'locale-source-strings',
    '#csv_plural' => 'locale-source-strings'
  );
  $def['lid'] = array(
    '#type' => 'int',
    '#title' => t('Source string ID'),
    '#key' => TRUE
  );
  $def['location'] = array(
    '#title' => t('Location')
  );
  $def['source_string'] = array(
    '#title' => t('Source string'),
    '#db_field_unaliased' => 'source'
  );

  $defs['locale_source'] = $def;

  $def = array(
    '#type' => 'entity',
    '#title' => t('Locale'),
    '#db_default_table' => 'locales_meta',
    '#xml_plural' => 'locales',
    '#csv_plural' => 'locales'
  );

  $def['locale_code'] = array(
    '#title' => t('Locale language code'),
    '#key_component' => TRUE,
    '#db_field_unaliased' => 'locale'
  );
  $def['name'] = array(
    '#title' => t('Locale name'),
    '#unique' => TRUE
  );
  $def['enabled'] = array(
    '#type' => 'int',
    '#title' => t('Enabled')
  );
  $def['isdefault'] = array(
    '#type' => 'int',
    '#title' => t('Is default')
  );
  $def['plurals'] = array(
    '#type' => 'int',
    '#title' => t('Plurals')
  );

  $def['localized_strings'] = array(
    '#type' => 'array',
    '#title' => t('Localized strings'),
    '#db_default_table' => 'locales_target',
    '#xml_plural' => 'localized-strings',
    '#csv_plural' => 'localized-strings',
    '#xml_mapping' => 'localized-string'
  );
  $def['localized_strings']['lid'] = array(
    '#type' => 'int',
    '#title' => t('Source string ID'),
    '#reference_entity' => 'locale_source',
    '#key_component' => TRUE
  );
  $def['localized_strings']['translation'] = array(
    '#title' => t('Localized string')
  );
  $def['localized_strings']['locale_code'] = array(
    '#title' => t('Locale language code'),
    '#db_field_unaliased' => 'locale',
    '#reference_entity' => 'locale',
    '#key_component' => TRUE
  );
  $def['localized_strings']['plid'] = array(
    '#type' => 'int',
    '#title' => t('Plural source string ID'),
    '#reference_entity' => 'locale',
    '#reference_field' => array('localized_strings', 'lid'),
    '#reference_parent' => FALSE,
    '#key_component' => TRUE
  );
  $def['localized_strings']['plural'] = array(
    '#type' => 'int',
    '#title' => t('Is plural')
  );

  $defs['locale'] = $def;

  return $defs;
}

/**
 * Implementation of hook_def_alter().
 */
function locale_def_alter(&$def) {
  if (isset($def['user'])) {
    $def['user']['language'] = array(
      '#title' => t('Language')
    );
  }
}

/**
 * Implementation of hook_db_def_tables().
 */
function locale_db_def_tables() {
  $tables = array();

  $tables['locales_meta'] = 'lm';
  $tables['locales_source'] = 'ls';
  $tables['locales_target'] = 'lt';

  return $tables;
}
