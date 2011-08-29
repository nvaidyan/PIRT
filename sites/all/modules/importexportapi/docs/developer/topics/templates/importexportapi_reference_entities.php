<?php

/**
 * @file
 * Helper for generating the reference documentation that involves covering
 * entities. This file is included by other template scripts.
 */

define('IMPORTEXPORTDOCS_BASE_URL', 'http://cvs.drupal.org/viewcvs/*checkout*/drupal/contributions/modules/importexportapi/');

/**
 * Determines or not the specified property is used by the specified field.
 *
 * @param $property
 *   The property as an array of attributes.
 * @param $field_id
 *   The unique identifier of the field in question.
 *
 * @return
 *   Boolean evaluation.
 */
function importexportdocs_is_property_used_by_field($property, $field_id) {
  return (!empty($property['used_by']) && ($property['used_by'] == 'ALL' || (is_array($property['used_by']) && array_search($field_id, $property['used_by']) !== FALSE)) || (isset($property['not_used_by']) && (empty($property['used_by']) && array_search($field_id, $property['not_used_by']) === FALSE)));
}

$fields = array();

$usage = "
<?php
\$def['revisions'] = array(
  '#type' => 'array',
  '#title' => t('Revisions'),
  '#db_default_table' => 'node_revisions',
  '#xml_plural' => 'revisions',
  '#xml_mapping' => 'revision',
  '#csv_plural' => 'node-revisions'
);
?>
";

$fields['array'] = array(
  'description' => 'Container for one or more child fields. Array fields are used for grouping together fields that may have more than one instance, and that have a parent-child relationship with their parent entity or array. An array field can have either an entity field or another array field as its parent. An array field can have fields of any type, except for <a href="#entity">entity</a>, as its children.',
  'usage_example_files' => array('definitions/importexportapi_node.inc'),
  'usage_example' => $usage
);

$usage = "
<?php
\$def = array(
  '#type' => 'entity',
  '#title' => t('Node'),
  '#db_default_table' => 'node',
  '#xml_plural' => 'nodes',
  '#csv_plural' => 'nodes'
);
?>
";

$usage = "
<?php
\$def['created'] = array(
  '#type' => 'datetime',
  '#title' => t('Member since')
);
?>
";

$fields['datetime'] = array(
  'description' => 'A timestamp in standard UNIX timestamp format. The timestamp is converted to and from an RFC-2822 string representation when using most \'get\' and \'put\' formats (e.g. XML).',
  'usage_example_files' => array('definitions/importexportapi_user.inc'),
  'usage_example' => $usage
);

$fields['entity'] = array(
  'description' => 'Top-level container for all other fields. Entity fields are used for grouping together fields that describe a single entity, and that may have more than one instance. An entity field cannot have a parent, and it can have fields of any type as its children.',
  'usage_example_files' => array('definitions/importexportapi_node.inc'),
  'usage_example' => $usage
);

$usage = "
<?php
\$def['file_path'] = array(
  '#type' => 'file',
  '#title' => t('File path'),
  '#db_field_unaliased' => 'filepath'
);
?>
";

$fields['file'] = array(
  'description' => 'A string specifying the path to a file. Currently, this field has no special behaviour to differentiate it from a regular string field. In future, this field will be used for assisting in automated file imports.',
  'usage_example_files' => array('definitions/importexportapi_system.inc'),
  'usage_example' => $usage
);

$fields['float'] = array(
  'description' => 'A real number (also known as a floating-point number).'
);

$usage = "
<?php
\$def['perm'] = array(
  '#type' => 'freeform',
  '#title' => t('Permission list'),
  '#default_value' => NULL,
  '#xml_plural' => 'permissions',
  '#csv_plural' => 'permissions',
  '#db_table' => 'permission'
);
?>
";

$fields['freeform'] = array(
  'description' => 'A list in delimited string format. The string can be either line-delimited or comma-delimited. The list is converted to and from a tree structure when using some \'get\' and \'put\' formats (e.g. XML).',
  'usage_example_files' => array('definitions/importexportapi_user.inc'),
  'usage_example' => $usage
);

$usage = "
<?php
\$def['uid'] = array(
  '#type' => 'int',
  '#title' => t('User ID'),
  '#key' => TRUE,
  '#db_uses_sequences' => TRUE
);
?>
";

$fields['int'] = array(
  'description' => 'An integral number (also known as a whole number).',
  'usage_example_files' => array('definitions/importexportapi_user.inc'),
  'usage_example' => $usage
);

$usage = "
<?php
\$def['value'] = array(
  '#type' => 'serialized',
  '#title' => t('Variable value')
);
?>
";

$fields['serialized'] = array(
  'description' => 'A PHP variable in serialized string format. The string is converted to and from a tree structure when using some \'get\' and \'put\' formats (e.g. XML).',
  'usage_example_files' => array('definitions/importexportapi_system.inc'),
  'usage_example' => $usage
);

$usage = "
<?php
\$def['pass'] = array(
  '#title' => t('Password')
);
?>
";

$fields['string'] = array(
  'description' => 'A string of text. Note: this is the default field type if none is specified.',
  'usage_example_files' => array('definitions/importexportapi_user.inc'),
  'usage_example' => $usage
);
