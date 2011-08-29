<?php

/**
 * @file
 * Template script used to generate the importexportapi_reference_csv.html
 * document. The code in this file is designed to be run through the
 * PHPTemplate system with the codefilter module in use. If the code in this
 * file is rendered without the help of codefilter, the PHP code examples will
 * not display properly.
 */

require_once('./pages/reference_entities.php');

$properties = array();

$usage = "
<?php
\$def['comment_statistics_nid'] = array(
  '#type' => 'int',
  '#title' => t('Node ID'),
  '#reference_entity' => $type,
  '#reference_field' => array('nid'),
  '#db_table' => 'node_comment_statistics',
  '#db_field_unaliased' => 'nid',
  '#key_component' => TRUE,
  '#csv_hidden' => TRUE
);
?>
";

$properties['csv_hidden'] = array(
  'not_used_by' => array('entity', 'array'),
  'description' => 'When set to TRUE, indicates that this field should not be outputted during a csv \'put\' operation.',
  'values' => 'Boolean.',
  'default_value' => 'NULL',
  'usage_example_files' => array('definitions/importexportapi_comment.inc'),
  'usage_example' => $usage
);

$usage = "
<?php
\$def['path'] = array(
  '#title' => t('System path'),
  '#xml_mapping' => 'system-path',
  '#csv_mapping' => 'system-path',
  '#unique' => TRUE
);
?>
";

$properties['csv_mapping'] = array(
  'not_used_by' => array('entity', 'array'),
  'description' => 'The CSV column heading name for this field. During a \'put\' operation, the values of this field will be outputted in a column underneath this name; and during a \'get\' operation, the system will search for a column with this name. This should NOT be confused with the <a href="#csv_plural">#csv_plural</a> property, which applies to CSV file names.',
  'values' => 'A valid CSV column heading name. <strong>Note:</strong> this value must be unique for all direct child fields of a given entity or array.',
  'default_value' => 'inherited from the <a href="'. IMPORTEXPORTDOCS_BASE_URL . 'docs/developer/topics/importexportapi_reference.html#id">#id</a> property of this field.',
  'usage_example_files' => array('definitions/importexportapi_menu.inc'),
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

$properties['csv_plural'] = array(
  'used_by' => array('entity', 'array'),
  'description' => 'The CSV file name for this field. During a \'put\' operation, the values of the direct children of this field will be outputted to a file (or to a portion of the screen) with this name; and during a \'get\' operation, the system will search for files with this name. This property should NOT be confused with the <a href="#csv_mapping">#csv_mapping</a> property, which applies to the column heading names for fields within files.',
  'values' => 'A valid file name. <strong>Note:</strong> this value must be unique for all fields to which it applies.',
  'default_value' => 'inherited from the <a href="'. IMPORTEXPORTDOCS_BASE_URL . 'docs/developer/topics/importexportapi_reference.html#id">#id</a> property of this field.',
  'usage_example_files' => array('definitions/importexportapi_node.inc'),
  'usage_example' => $usage
);
?>
<p>This document provides a programmer's reference to the <em>csv get</em> and <em>csv put</em> engines of the Drupal Import / Export API. For an explanation of the field types and their core properties, see the main <a href="<?php print IMPORTEXPORTDOCS_BASE_URL .'docs/developer/topics/importexportapi_reference.html'; ?>">Import / Export API reference</a> document.</p>
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
