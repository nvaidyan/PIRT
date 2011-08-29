<?php

/**
 * @file
 * Template script used to generate the importexportapi_reference.html document.
 * The code in this file is designed to be run through the PHPTemplate system
 * with the codefilter module in use. If the code in this file is rendered
 * without the help of codefilter, the PHP code examples will not display
 * properly.
 */

require_once('./pages/reference_entities.php');

$properties = array();

$usage = "
<?php
\$def['name'] = array(
  '#title' => t('Username'),
  '#alt_key_for' => 'uid'
);
?>
";

$properties['alt_key_for'] = array(
  'used_by' => array('int', 'string'),
  'description' => 'The name of the field for which this field is an alternate key. The field being named must be either a <a href="#key">#key</a> or a <a href="#key_component">#key_component</a> field. All fields with this property set are implicitly <a href="#unique">#unique</a>.',
  'values' => 'A field name. The parents of the alternate key field cannot be specfied; instead, it is assumed that the alternate key field is a sibling of this field.',
  'default_value' => 'NULL',
  'usage_example_files' => array('definitions/importexportapi_user.inc'),
  'usage_example' => $usage
);

$usage = "
<?php
\$def['auth']['uid'] = array(
  '#type' => 'int',
  '#title' => t('User ID'),
  '#reference_entity' => 'user',
  '#alt_key_ignore' => array('name')
);
\$def['auth']['authname'] = array(
  '#title' => t('Auth name'),
  '#reference_entity' => 'user',
  '#reference_field' => array('name'),
  '#alt_key_ignore' => TRUE
);
?>
";

$properties['alt_key_ignore'] = array(
  'used_by' => array('int', 'string'),
  'description' => 'The names of alternate key fields (of the field that this field is referencing) that should NOT be copied and generated as siblings of this field. Should only be used by fields that specify a <a href="#reference_entity">#reference_entity</a>.',
  'values' => 'Either an array of alternate key field names, or boolean TRUE to indicate all alternate key fields.',
  'default_value' => 'NULL',
  'usage_example_files' => array('definitions/importexportapi_user.inc'),
  'usage_example' => $usage
);

$usage = "
<?php
\$def['status'] = array(
  '#type' => 'int',
  '#title' => t('Published'),
  '#default_value' => 1
);
?>
";

$properties['default_value'] = array(
  'not_used_by' => array('entity', 'array'),
  'description' => 'The value of the field that will be used if <a href="#value">#value</a> is NULL or undefined. Should NOT be confused with <a href="#value">#value</a>, which is the actual value that can be explicitly provided.',
  'values' => 'Mixed.',
  'default_value' => 'Different for each field type.',
  'usage_example_files' => array('definitions/importexportapi_node.inc'),
  'usage_example' => $usage
);

$properties['dependencies'] = array(
  'used_by' => array('entity'),
  'description' => 'INTERNAL. The entities that this entity depends on. This is determined automatically, by looking for fields that reference other fields in other entities.',
  'values' => 'Array of entity field names.',
  'default_value' => 'empty array'
);

$properties['generated'] = array(
  'used_by' => array('int', 'string'),
  'description' => 'INTERNAL. Indicates that this field is a computer-generated copy of an alternate key field.',
  'values' => 'Boolean.',
  'default_value' => 'NULL'
);

$properties['get'] = array(
  'used_by' => 'ALL',
  'description' => 'INTERNAL. Contains engine-specific properties that have been configured for use in a \'get\' operation. The properties inside are generally inherited from their equivalent top-level properties, unless they have been explicitly set. Although this property is internally generated, the properties inside it are considered public, and can (and should) be changed by other modules that utilise import / export API functionality.',
  'values' => 'Mixed.',
  'default_value' => 'NULL'
);

$properties['id'] = array(
  'used_by' => 'ALL',
  'description' => 'INTERNAL. The field\'s id property, which is determined by the key of the field as declared in a data definition.',
  'values' => 'String.',
  'default_value' => 'n/a'
);

$usage = "
<?php
\$def['nid'] = array(
  '#type' => 'int',
  '#title' => t('Node ID'),
  '#key' => TRUE,
  '#db_uses_sequences' => TRUE
);
?>
";

$properties['key'] = array(
  'used_by' => array('int'),
  'description' => 'Specifies that this field is the unique, database-generated, integer identifier for the entity or array of which it is a part. A maximum of one field per entity or array can be a key field For example, if "x" is an entity, "y" is an array that is the child of "x", and "z" is an array that is the child of "y", then "x", "y", and "z" can each have a maximum of one key field. Should NOT be used by fields that specify a <a href="#reference_entity">#reference_entity</a>.',
  'values' => 'Boolean.',
  'default_value' => 'NULL',
  'usage_example_files' => array('definitions/importexportapi_node.inc'),
  'usage_example' => $usage
);

$usage = "
<?php
\$def['parents']['tid'] = array(
  '#type' => 'int',
  '#title' => t('Term ID'),
  '#reference_entity' => 'term',
  '#alt_key_ignore' => TRUE,
  '#key_component' => TRUE
);
?>
";

$properties['key_component'] = array(
  'used_by' => array('int', 'string'),
  'description' => 'Specifies that this field is one of the fields that makes up the unique identifier for the entity or array of which it is a part. The unique identifier for each entity or array consists of the <a href="#key">#key</a> field (if any), plus all key_component fields. If no <a href="#key">#key</a> field is specified for an entity or array, then at least one field must be a key_component.',
  'values' => 'Boolean.',
  'default_value' => 'NULL',
  'usage_example_files' => array('definitions/importexportapi_taxonomy.inc'),
  'usage_example' => $usage
);

$properties['keys'] = array(
  'used_by' => array('entity', 'array'),
  'description' => 'INTERNAL. The list of fields that make up the unique identifier for this entity or array field. This list consists of all <a href="#key">#key</a> or <a href="#key_component">#key_component</a> fields that are direct children of this field.',
  'values' => 'Array of field names.',
  'default_value' => 'n/a'
);

$properties['parents'] = array(
  'used_by' => 'ALL',
  'description' => 'INTERNAL. The parents of the current field. This field is always an empty array for entities.',
  'values' => 'Array of field names, in order from most distant parent to least distant (i.e. direct) parent.',
  'default_value' => 'empty array'
);

$usage = "
<?php
\$def = array(
  '#type' => 'entity',
  '#title' => t('User role'),
  '#xml_plural' => 'roles',
  '#csv_plural' => 'roles',
  '#process' => array('_importexportapi_user_process_role' => array())
);
?>
";

$properties['process'] = array(
  'used_by' => 'ALL',
  'description' => 'The callback functions to use when executing the \'process\' hook on this field. The callback functions are executed in the order in which they are listed. Note: if this field is of a type that specifies a default set of callback functions, then all callback functions specified here will be <em>appended</em> to the default list, and will not <em>replace</em> the default list.',
  'values' => 'Array of callback functions, in the form \'function_name\' => array(<em>\'arguments\'</em>).',
  'default_value' => 'Different for each field type.',
  'usage_example_files' => array('definitions/importexportapi_user.inc'),
  'usage_example' => $usage
);

$properties['processed_get'] = array(
  'used_by' => 'ALL',
  'description' => 'INTERNAL. Indicates whether or not the \'process\' hook has been executed for for this field during the current \'get\' operation.',
  'values' => 'Boolean.',
  'default_value' => 'NULL'
);

$properties['processed_put'] = array(
  'used_by' => 'ALL',
  'description' => 'INTERNAL. Indicates whether or not the \'process\' hook has been executed for for this field during the current \'put\' operation.',
  'values' => 'Boolean.',
  'default_value' => 'NULL'
);

$properties['put'] = array(
  'used_by' => 'ALL',
  'description' => 'INTERNAL. Contains engine-specific properties that have been configured for use in a \'put\' operation. The properties inside are generally inherited from their equivalent top-level properties, unless they have been explicitly set. Although this property is internally generated, the properties inside it are considered public, and can (and should) be changed by other modules that utilise import / export API functionality.',
  'values' => 'Mixed.',
  'default_value' => 'NULL'
);

$usage = "
<?php
\$def['parents']['parent_tid'] = array(
  '#type' => 'int',
  '#title' => t('Parent term ID'),
  '#reference_entity' => 'term',
  '#reference_field' => array('tid'),
  '#db_field_unaliased' => 'parent',
  '#key_component' => TRUE,
  '#reference_delta' => 1,
  '#reference_parent' => FALSE,
  '#alt_key_ignore' => TRUE
);
?>
";

$properties['reference_delta'] = array(
  'used_by' => array('int', 'string'),
  'description' => 'If separate instances of a particular entity or array are referenced by multiple fields that are children of the same entity or array, then each referencing field will need to set a different delta value. This allows for a separate join to be performed on each instance, when executing database queries on the entity or array that is the parent of this field.',
  'values' => 'Integer (must be > 0).',
  'default_value' => 'NULL',
  'usage_example_files' => array('definitions/importexportapi_taxonomy.inc'),
  'usage_example' => $usage
);

$usage = "
<?php
\$def['uid'] = array(
  '#type' => 'int',
  '#title' => t('User ID'),
  '#reference_entity' => 'user'
);
?>
";

$properties['reference_entity'] = array(
  'used_by' => array('int', 'string'),
  'description' => 'The name of the entity that is the parent (direct or non-direct) of the field that this field is referencing. This is the only field that is required in order to define a field as a reference to another field.',
  'values' => 'An entity field name.',
  'default_value' => 'NULL',
  'usage_example_files' => array('definitions/importexportapi_node.inc'),
  'usage_example' => $usage
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

$properties['reference_field'] = array(
  'used_by' => array('int', 'string'),
  'description' => 'The field that this field is referencing. Should only be used by fields that specify a <a href="#reference_entity">#reference_entity</a>. Defaults to the unique identifying name of this field. This property only needs to be specified if the unique identifying name of this field is different to the unique identifying name of the field being referenced, or if the field being referenced is not the direct child of an entity (i.e. if it is the direct child of an array).',
  'values' => 'An array of field names, that consists of the names of all the parents of the referencing field (except for its parent entity), and the name of the referencing field itself, in order from the most distant parent to the actual referenced field.',
  'default_value' => 'NULL',
  'usage_example_files' => array('definitions/importexportapi_user.inc'),
  'usage_example' => $usage
);

$usage = "
<?php
\$def['parents']['parent_tid'] = array(
  '#type' => 'int',
  '#title' => t('Parent term ID'),
  '#reference_entity' => 'term',
  '#reference_field' => array('tid'),
  '#db_field_unaliased' => 'parent',
  '#key_component' => TRUE,
  '#reference_delta' => 1,
  '#reference_parent' => FALSE,
  '#alt_key_ignore' => TRUE
);
?>
";

$properties['reference_parent'] = array(
  'used_by' => array('int', 'string'),
  'description' => 'Allows this field to NOT be a reference to a field within its parent\'s parent. By default, all fields that reference a field within the parent entity or array of their parent are used for parent-child linking. Setting this value to FALSE will reverse this behaviour, and will stop this field from being used for parent-child linking. This property should generally be used in conjunction with <a href="#reference_delta">#reference_delta</a>.',
  'values' => 'Boolean.',
  'default_value' => 'NULL',
  'usage_example_files' => array('definitions/importexportapi_taxonomy.inc'),
  'usage_example' => $usage
);

$usage = "
<?php
\$def['auth']['module'] = array(
  '#title' => t('Auth module'),
  '#required' => TRUE
);
?>
";

$properties['required'] = array(
  'not_used_by' => array('entity', 'array'),
  'description' => 'Specifies that a field is required to have a value (other than its <a href="#default_value">#default_value</a>). Note: this is not currently enforced or validated in the API.',
  'values' => 'Boolean.',
  'default_value' => 'FALSE',
  'usage_example_files' => array('definitions/importexportapi_user.inc'),
  'usage_example' => $usage
);

$properties['source'] = array(
  'used_by' => array('entity'),
  'description' => 'The engine to be used for accessing the source of data for this entity.',
  'values' => 'String.',
  'default_value' => 'db'
);

$usage = "
<?php
\$def['pass'] = array(
  '#title' => t('Password')
);
?>
";

$properties['title'] = array(
  'used_by' => 'ALL',
  'description' => 'The title of the field. Make sure to enclose inside the <a href="http://api.drupal.org/t">t()</a> function so this property can be translated. Note: this is the only property that is required explicitly and universally, for all field types.',
  'values' => 'String.',
  'default_value' => 'n/a',
  'usage_example_files' => array('definitions/importexportapi_user.inc'),
  'usage_example' => $usage
);

$usage = "
<?php
\$def['created'] = array(
  '#type' => 'int',
  '#title' => t('Member since')
);
?>
";

$properties['type'] = array(
  'used_by' => 'ALL',
  'description' => 'Used to determine the type of field.',
  'values' => 'String.',
  'default_value' => 'string',
  'usage_example_files' => array('definitions/importexportapi_user.inc'),
  'usage_example' => $usage
);

$usage = "
<?php
\$def['vid'] = array(
  '#type' => 'int',
  '#title' => t('Revision ID'),
  '#reference_entity' => 'node',
  '#reference_field' => array('revisions', 'vid'),
  '#unique' => TRUE
);
?>
";

$properties['unique'] = array(
  'not_used_by' => array('entity', 'array'),
  'description' => 'Used to determine if all instances of this field must be unique.',
  'values' => 'Boolean.',
  'default_value' => 'NULL',
  'usage_example_files' => array('definitions/importexportapi_node.inc'),
  'usage_example' => $usage
);

$properties['value'] = array(
  'not_used_by' => array('entity'),
  'description' => 'INTERNAL. This is where the actual values of a field are placed during a \'get\' operation, and it is where they are retrieved from during a \'put\' operation.',
  'values' => 'Mixed.',
  'default_value' => 'NULL'
);
?>
<p>This document provides a programmer's reference to the Drupal Import / Export API. If you're interested in step-by-step documentation to help you utilise the API, please see the <a href="#link-to-quickstart-goes-here">Import / Export API QuickStart guide</a>.</p>

<p>See also the reference guides for the bundled engines:</p>

<ul>
<li><a href="<?php print IMPORTEXPORTDOCS_BASE_URL; ?>docs/developer/topics/importexportapi_reference_db.html">db engines</a></li>
<li><a href="<?php print IMPORTEXPORTDOCS_BASE_URL; ?>docs/developer/topics/importexportapi_reference_csv.html">csv engines</a></li>
<li><a href="<?php print IMPORTEXPORTDOCS_BASE_URL; ?>docs/developer/topics/importexportapi_reference_xml.html">xml engines</a></li>
</ul>

<p>Skip to: <a href="#properties">Properties</a> | <a href="#fields">Field types</a></p>

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
    <th scope="col"><a href="#<?php print $field_id; ?>"><?php print $field_id; ?></a></th>
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

<h2><a name="fields" id="fields"></a>Field types</h2>

<?php
foreach ($fields as $field_id => $field) {
?>
<h3><a name="<?php print $field_id; ?>" id="<?php print $field_id; ?>"></a><?php print $field_id; ?></h3>

<?php
  $properties_array = array();
  foreach ($properties as $property_id => $property) {
    if (importexportdocs_is_property_used_by_field($property, $field_id)) {
      $properties_array[] = '<a href="#'. $property_id .'">#'. $property_id .'</a>';
    }
  }
?>
<p><strong>Description</strong>: <?php print !empty($field['description']) ? $field['description'] : 'NONE'; ?></p>
<p><strong>Properties</strong>: <?php print !empty($properties_array) ? (is_array($properties_array) ? implode(', ', $properties_array) .'.' : $properties_array) : 'NONE'; ?></p>
<?php
  $usage_example_files = $field['usage_example_files'];
  $usage_example_files_empty = empty($usage_example_files);
  $usage_example_files_array = array();
  $usage_example_text = $field['usage_example_text'];
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
<p><strong>Usage example</strong>: <?php print ($usage_example_files_or_text ? (!$usage_example_files_empty ? '('. implode(', ', $usage_example_files_array) .')' : '') . (!empty($field['usage_example']) ? ':' : '') . (!$usage_example_text_empty ? ' '. $usage_example_text : '') : 'NONE'); ?></p>
<?php print !empty($field['usage_example']) ? $field['usage_example'] : ''; ?>
<?php
}
?>

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
        $used_by_array[] = '<a href="#'. $used_by .'">'. $used_by .'</a>';
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
        $not_used_by_array[] = '<a href="#'. $not_used_by .'">'. $not_used_by .'</a>';
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
