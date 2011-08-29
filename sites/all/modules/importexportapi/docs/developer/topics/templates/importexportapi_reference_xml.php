<?php

/**
 * @file
 * Template script used to generate the importexportapi_reference_xml.html
 * document. The code in this file is designed to be run through the
 * PHPTemplate system with the codefilter module in use. If the code in this
 * file is rendered without the help of codefilter, the PHP code examples will
 * not display properly.
 */

require_once('./pages/reference_entities.php');

$properties = array();

$properties['xml_hidden'] = array(
  'not_used_by' => array('entity', 'array'),
  'description' => 'When set to TRUE, indicates that this field should not be outputted during an xml \'put\' operation.',
  'values' => 'Boolean.',
  'default_value' => 'NULL'
);

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

$properties['xml_mapping'] = array(
  'used_by' => 'ALL',
  'description' => 'The XML tag name for this field. During a \'put\' operation, the value of this field will be wrapped in a tag with this name; and during a \'get\' operation, the system will search for tags with this name. For field types that contain multiple values, this name applies to the tags that enclose each individual value; this should NOT be confused with the <a href="#xml_plural">#xml_plural</a> property, which applies to tags that enclose multiple values.',
  'values' => 'A valid XML tag name. <strong>Note:</strong> this value must be different to the value of the XML tags that will become this field\'s respective direct parent and direct child (as well as siblings). For example, if this field is an array, then both the <a href="#xml_plural">#xml_plural</a> value of this field, and the <a href="#xml_mapping">#xml_mapping</a> value (or, where applicable, the <a href="#xml_plural">#xml_plural</a> value) of this field\'s direct children, must be different to the value of this property for this field.',
  'default_value' => 'inherited from the <a href="'. IMPORTEXPORTDOCS_BASE_URL . 'docs/developer/topics/importexportapi_reference.html#id">#id</a> property of this field.',
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

$properties['xml_plural'] = array(
  'used_by' => array('entity', 'array', 'freeform'),
  'description' => 'The XML tag name for a collection of values for this field. During a \'put\' operation, the collection of values for this field will be wrapped in a tag with this name; and during a \'get\' operation, the system will search for tags with this name. This property should NOT be confused with the <a href="#xml_mapping">#xml_mapping</a> property, which applies to the tags that enclose each individual value, and which also applies to all field types.',
  'values' => 'A valid XML tag name. <strong>Note:</strong> this value must always be different to the value of <a href="#xml_mapping">#xml_mapping</a>, in any field for which both of these properties apply. If this field has a parent field, then the <a href="#xml_mapping">#xml_mapping</a> value for the parent field should also be different to this property for this field. Additionally, this property should be unique for all sibling fields.',
  'default_value' => 'inherited from the <a href="'. IMPORTEXPORTDOCS_BASE_URL . 'docs/developer/topics/importexportapi_reference.html#id">#id</a> property of this field.',
  'usage_example_files' => array('definitions/importexportapi_node.inc'),
  'usage_example' => $usage
);
?>
<p>This document provides a programmer's reference to the <em>xml get</em> and <em>xml put</em> engines of the Drupal Import / Export API. For an explanation of the field types and their core properties, see the main <a href="<?php print IMPORTEXPORTDOCS_BASE_URL .'docs/developer/topics/importexportapi_reference.html'; ?>">Import / Export API reference</a> document.</p>
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
