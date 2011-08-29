<?php

/**
 * @file
 * Template script used to generate the importexportapi_reference_db.html
 * document. The code in this file is designed to be run through the
 * PHPTemplate system with the codefilter module in use. If the code in this
 * file is rendered without the help of codefilter, the PHP code examples will
 * not display properly.
 */

require_once('./pages/reference_entities.php');

$properties = array();

$usage = "
<?php
\$def = array(
  '#type' => 'entity',
  '#title' => t('User'),
  '#db_default_table' => 'users',
  '#xml_plural' => 'users',
  '#csv_plural' => 'users'
);
?>
";

$properties['db_default_table'] = array(
  'used_by' => array('entity', 'array'),
  'description' => 'The default value to assign to the <a href="#db_table">#db_table</a> property for all direct children of this field. This value is not assigned to child fields if they explicitly define their <a href="#db_table">#db_table</a> property.',
  'values' => 'An unprefixed database table name. The table name must be one that is defined through <code>hook_db_def_tables()</code>.',
  'default_value' => 'inherited from the <a href="'. IMPORTEXPORTDOCS_BASE_URL . 'docs/developer/topics/importexportapi_reference.html#id">#id</a> property of this field.',
  'usage_example_files' => array('definitions/importexportapi_user.inc'),
  'usage_example' => $usage
);

$properties['db_do_query'] = array(
  'used_by' => array('entity', 'array'),
  'description' => 'INTERNAL. Indicates whether or not the query for this entity or array should be prepared and executed.',
  'values' => 'Boolean.',
  'default_value' => 'TRUE'
);

$properties['db_field'] = array(
  'not_used_by' => array('entity', 'array'),
  'description' => 'The field name that should be used when retrieving this field in database queries. This is different to <a href="#db_field_unaliased">#db_field_unaliased</a>, in that it does not have to correspond to the actual name of the field in the database. Every direct child of an entity or array field should have a unique value for this property and for the <a href="#db_reference_field">#db_reference_field</a> property.',
  'values' => 'A database field querying name.',
  'default_value' => 'inherited from the <a href="'. IMPORTEXPORTDOCS_BASE_URL . 'docs/developer/topics/importexportapi_reference.html#id">#id</a> property of this field.'
);

$usage = "
<?php
\$def['perm_rid'] = array(
  '#type' => 'int',
  '#title' => t('Role ID'),
  '#reference_entity' => 'role',
  '#reference_field' => array('rid'),
  '#db_table' => 'permission',
  '#db_field_unaliased' => 'rid',
  '#key_component' => TRUE,
  '#csv_hidden' => TRUE
);
?>
";

$properties['db_field_unaliased'] = array(
  'not_used_by' => array('entity', 'array'),
  'description' => 'The name of this field in the database. Should NOT be confused with <a href="#db_field">#db_field</a>, which is the name used to query this field in the database.',
  'values' => 'A database field name.',
  'default_value' => 'inherited from the <a href="'. IMPORTEXPORTDOCS_BASE_URL . 'docs/developer/topics/importexportapi_reference.html#id">#id</a> property of this field.',
  'usage_example_files' => array('definitions/importexportapi_user.inc'),
  'usage_example' => $usage
);

$usage = "
<?php
\$defs['block']['module']['#db_filter'] = array(
  'values' => array('block'),
  'operator' => 'neq'
);
?>
";

$properties['db_filter'] = array(
  'not_used_by' => array('entity', 'array'),
  'description' => 'A filtering rule to be applied to this field when querying it from the database. The rule has a special syntax which is represented as a simple array.',
  'values' => 'An optional \'operator\' property (defaults to \'eq\'), which can be \'eq\', \'neq\', or any valid SQL comparison operator; and a \'values\' property, which can be an array of values (each of the correct type) when \'operator\' is \'eq\' or \'neq\', or else whatever value is appropriate for the \'operator\' value that is being used.',
  'default_value' => 'NULL',
  'usage_example_files' => array('definitions/importexportapi_block.inc'),
  'usage_example' => $usage
);

$properties['db_force_generate_id'] = array(
  'used_by' => array('int'),
  'description' => 'When set to TRUE, forces a new value to be generated for this field (from the database) during a db \'put\' operation, even if one was already provided. Should generally be used in combination with the <a href="#db_generate_id">#db_generate_id</a> property. Only applies to fields with a <a href="'. IMPORTEXPORTDOCS_BASE_URL . 'docs/developer/topics/importexportapi_reference.html#key">#key</a> value of TRUE.',
  'values' => 'Boolean.',
  'default_value' => 'NULL'
);

$properties['db_generate_id'] = array(
  'used_by' => array('int'),
  'description' => 'When set to TRUE, indicates that a new value is to be generated for this field (from the database) during a db \'put\' operation, if none has been provided. Only applies to fields with a <a href="'. IMPORTEXPORTDOCS_BASE_URL . 'docs/developer/topics/importexportapi_reference.html#key">#key</a> value of TRUE.',
  'values' => 'Boolean.',
  'default_value' => 'TRUE'
);

$properties['db_is_insert'] = array(
  'used_by' => array('entity', 'array'),
  'description' => 'INTERNAL. Indicates whether the query for this entity or array is to be an SQL INSERT or UPDATE query.',
  'values' => 'Boolean.',
  'default_value' => 'TRUE'
);

$properties['db_keys'] = array(
  'used_by' => array('entity', 'array'),
  'description' => 'INTERNAL. The list of database fields that make up the unique identifier for this entity or array field. This list always consists of the <a href="#db_field_unaliased">#db_field_unaliased</a> values for each field in the <a href="#keys">#keys</a> property of this field.',
  'values' => 'Array of database field names.',
  'default_value' => 'n/a'
);

$properties['db_reference_delta'] = array(
  'used_by' => array('int', 'string'),
  'description' => 'If separate instances of a particular entity or array are referenced by multiple fields that are children of the same entity or array, then each referencing field will need to set a different delta value. This allows for a separate join to be performed on each instance, when executing database queries on the entity or array that is the parent of this field.',
  'values' => 'Integer (must be > 0).',
  'default_value' => 'inherited from the <a href="'. IMPORTEXPORTDOCS_BASE_URL . 'docs/developer/topics/importexportapi_reference.html#reference_delta">#reference_delta</a> value of this field.'
);

$properties['db_reference_field'] = array(
  'used_by' => array('int', 'string'),
  'description' => 'The field name that should be used when retrieving the field that this field is referencing in database queries. This is different to <a href="#db_reference_field_unaliased">#db_reference_field_unaliased</a>, in that it does not have to correspond to the actual name of the referenced field in the database. Every direct child of an entity or array field should have a unique value for this property and for the <a href="#db_field">#db_field</a> property. Only applies to fields with a non-empty <a href="'. IMPORTEXPORTDOCS_BASE_URL . 'docs/developer/topics/importexportapi_reference.html#reference_entity">#reference_entity</a> value.',
  'values' => 'A database field querying name.',
  'default_value' => 'inherited from the <a href="#db_field">#db_field</a> property of the field that this field is referencing.'
);

$properties['db_reference_field_unaliased'] = array(
  'used_by' => array('int', 'string'),
  'description' => 'The name of the field that this field is referencing in the database. Should NOT be confused with <a href="#db_reference_field">#db_reference_field</a>, which is the name used to query the referenced field in the database. Only applies to fields with a non-empty <a href="'. IMPORTEXPORTDOCS_BASE_URL . 'docs/developer/topics/importexportapi_reference.html#reference_entity">#reference_entity</a> value.',
  'values' => 'A database field name.',
  'default_value' => 'inherited from the <a href="#db_field_unaliased">#db_field_unaliased</a> property of the field that this field is referencing.'
);

$properties['db_reference_parent'] = array(
  'used_by' => array('int', 'string'),
  'description' => 'Allows this field to NOT be a reference to a field within its parent\'s parent. By default, all fields that reference a field within the parent entity or array of their parent are used for parent-child linking. Setting this value to FALSE will reverse this behaviour, and will stop this field from being used for parent-child linking. This property should generally be used in conjunction with <a href="#db_reference_delta">#db_reference_delta</a>.',
  'values' => 'Boolean.',
  'default_value' => 'inherited from the <a href="'. IMPORTEXPORTDOCS_BASE_URL . 'docs/developer/topics/importexportapi_reference.html#reference_parent">#reference_parent</a> value of this field.'
);

$properties['db_reference_table'] = array(
  'used_by' => array('int', 'string'),
  'description' => 'The name of the database table for the field that this field is referencing. Only applies to fields with a non-empty <a href="'. IMPORTEXPORTDOCS_BASE_URL . 'docs/developer/topics/importexportapi_reference.html#reference_entity">#reference_entity</a> value.',
  'values' => 'An unprefixed database table name. The table name must be one that is defined through <code>hook_db_def_tables()</code>.',
  'default_value' => 'inherited from the <a href="#db_table">#db_table</a> property of the field that this field is referencing.'
);

$properties['db_resolved_alt_key'] = array(
  'used_by' => array('int'),
  'description' => 'INTERNAL. Stores the database name of the alternate key field that was used to resolve this field during a db \'put\' operation (if any).',
  'values' => 'A database field querying name.',
  'default_value' => 'NULL'
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

$properties['db_table'] = array(
  'not_used_by' => array('entity', 'array'),
  'description' => 'The name of the database table for this field. Should NOT be confused with <a href="#db_field">#db_field</a>, which is the name used to query this field in the database.',
  'values' => 'An unprefixed database table name. The table name must be one that is defined through <code>hook_db_def_tables()</code>.',
  'default_value' => 'inherited from the <a href="#db_default_table">#db_default_table</a> property of this field\'s direct parent entity or array.',
  'usage_example_files' => array('definitions/importexportapi_user.inc'),
  'usage_example' => $usage
);

$properties['db_update_existing'] = array(
  'used_by' => array('int'),
  'description' => 'When set to TRUE, indicates that existing database entries that match this field should be updated (rather than new entries being inserted, and old entries left as-is) during a db \'put\' operation, if existing entries can be found. Only applies to fields with a <a href="'. IMPORTEXPORTDOCS_BASE_URL . 'docs/developer/topics/importexportapi_reference.html#key">#key</a> or <a href="'. IMPORTEXPORTDOCS_BASE_URL . 'docs/developer/topics/importexportapi_reference.html#key_component">#key_component</a> value of TRUE, or with a non-empty <a href="'. IMPORTEXPORTDOCS_BASE_URL . 'docs/developer/topics/importexportapi_reference.html#reference_entity">#reference_entity</a> value.',
  'values' => 'Boolean.',
  'default_value' => 'TRUE'
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

$properties['db_uses_sequences'] = array(
  'used_by' => array('int'),
  'description' => 'When set to TRUE, indicates that Drupal\'s custom database sequencing system (i.e. the \'sequences table\' system) should be used when generating a new value for this field, instead of relying on in-built database sequencing / incrementing. Only applies to fields with a <a href="'. IMPORTEXPORTDOCS_BASE_URL . 'docs/developer/topics/importexportapi_reference.html#key">#key</a> value of TRUE.',
  'values' => 'Boolean.',
  'default_value' => 'NULL',
  'usage_example_files' => array('definitions/importexportapi_user.inc'),
  'usage_example' => $usage
);

$properties['db_value_old'] = array(
  'used_by' => array('int'),
  'description' => 'INTERNAL. Stores the original value of this field during a db \'put\' operation, in case a new value was database-generated for this field.',
  'values' => 'Integer.',
  'default_value' => 'inherited from the <a href="'. IMPORTEXPORTDOCS_BASE_URL . 'docs/developer/topics/importexportapi_reference.html#value">#value</a> property of this field. Only applies to fields with a <a href="'. IMPORTEXPORTDOCS_BASE_URL . 'docs/developer/topics/importexportapi_reference.html#key">#key</a> value of TRUE.'
);
?>
<p>This document provides a programmer's reference to the <em>db get</em> and <em>db put</em> engines of the Drupal Import / Export API. For an explanation of the field types and their core properties, see the main <a href="<?php print IMPORTEXPORTDOCS_BASE_URL .'docs/developer/topics/importexportapi_reference.html'; ?>">Import / Export API reference</a> document.</p>
<p>Skip to: <a href="#properties">Properties</a></p>

<h2>Fields and properties matrix</h2>

<p><strong>Legend:</strong><br />
<strong>X</strong> = property can be used with this field type<br />
<strong>-</strong> = this property is not applicable to this field type</p>

<table border="1">
  <tr>
    <th scope="col"><a href="#type"><strong>#type</strong></a></th>
<?php
foreach (array_keys($fields) as $field_id) {
?>
    <th scope="col"><a href="<?php print IMPORTEXPORTDOCS_BASE_URL . 'docs/developer/topics/importexportapi_reference.html#'. $field_id; ?>"><?php print $field_id; ?></a></th>
<?php
}
?>
  </tr>
<?php
foreach ($properties as $property_id => $property) {
?>
  <tr>
    <th scope="row"><a href="#<?php print $property_id; ?>">#<?php print $property_id; ?></a></th>
<?php
  foreach (array_keys($fields) as $field_id) {
    if (importexportdocs_is_property_used_by_field($property, $field_id)) {
?>
    <td class="x"><strong>X</strong></td>
<?php
    }
    else {
?>
    <td>-</td>
<?php
    }
  }
?>
  </tr>
<?php
}
?>
</table>

<h2><a name="properties" id="properties"></a>Properties</h2>

<?php
foreach ($properties as $property_id => $property) {
?>
<h3><a name="<?php print $property_id; ?>" id="<?php print $property_id; ?>"></a>#<?php print $property_id; ?></h3>

<?php
  $used_by_array = array();
  if (!empty($property['used_by'])) {
    if (is_array($property['used_by'])) {
      foreach ($property['used_by'] as $used_by) {
        $used_by_array[] = '<a href="'. IMPORTEXPORTDOCS_BASE_URL .'docs/developer/topics/importexportapi_reference.html#'. $used_by .'">'. $used_by .'</a>';
      }
    }
    else {
      $used_by_array = $property['used_by'];
    }
  }

  $not_used_by_array = array();
  if (!empty($property['not_used_by'])) {
    if (is_array($property['not_used_by'])) {
      foreach ($property['not_used_by'] as $not_used_by) {
        $not_used_by_array[] = '<a href="'. IMPORTEXPORTDOCS_BASE_URL .'docs/developer/topics/importexportapi_reference.html#'. $not_used_by .'">'. $not_used_by .'</a>';
      }
    }
  }
?>
<p><strong>Used by</strong>: <?php print !empty($used_by_array) ? (is_array($used_by_array) ? implode(', ', $used_by_array) .'.' : $used_by_array) : (!empty($not_used_by_array) ? 'ALL types except '. implode(', ', $not_used_by_array) .'.' : 'NONE'); ?></p>
<p><strong>Description</strong>: <?php print !empty($property['description']) ? $property['description'] : 'NONE'; ?></p>
<p><strong>Values</strong>: <?php print !empty($property['values']) ? $property['values'] : 'NONE'; ?></p>
<p><strong>Default value</strong>: <?php print !empty($property['default_value']) ? $property['default_value'] : 'NONE'; ?></p>
<?php
  $usage_example_files = $property['usage_example_files'];
  $usage_example_files_empty = empty($usage_example_files);
  $usage_example_files_array = array();
  $usage_example_text = $property['usage_example_text'];
  $usage_example_text_empty = empty($usage_example_text);
  $usage_example_files_or_text = !($usage_example_files_empty && $usage_example_text_empty);

  if (!$usage_example_files_empty) {
    foreach ($usage_example_files as $usage_example_file) {
      $usage_example_file_end = explode('/', $usage_example_file);
      $usage_example_file_end = end($usage_example_file_end);
      $usage_example_files_array[] = '<a href="'. IMPORTEXPORTDOCS_BASE_URL . $usage_example_file .'">'. $usage_example_file_end .'</a>';
    }
  }
?>
<p><strong>Usage example</strong>: <?php print ($usage_example_files_or_text ? (!$usage_example_files_empty ? '('. implode(', ', $usage_example_files_array) .')' : '') . (!empty($property['usage_example']) ? ':' : '') . (!$usage_example_text_empty ? ' '. $usage_example_text : '') : 'NONE'); ?></p>
<?php print !empty($property['usage_example']) ? $property['usage_example'] : ''; ?>
<?php
}
?>
