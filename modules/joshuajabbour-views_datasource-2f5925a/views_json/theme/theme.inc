<?php
// $Id: views_views_json_style.theme.inc,v 1.1.2.5 2010/07/15 07:34:48 allisterbeharry Exp $

/**
 * @file
 * Views theme to render view fields as JSON.
 *
 * - $vars['view']: The current view.
 * - $vars['rows']: Array of row objects as rendered by _views_json_render_fields
 * - $attachment: Not used currently
 * - $vars['options']: The options for the style passed in from the UI.
 *
 * @ingroup views_templates
 * @see views_json.views.inc
 */

function template_preprocess_views_views_json_style_simple(&$vars) {
  $root_object = $vars['options']['root_object'];
  $top_child_object = $vars['options']['top_child_object'];
  $plaintext_output = $vars['options']['plaintext_output'];
  $objects = array();

  foreach ($vars['rows'] as $row) {
    $object = array();

    // Convert the rows into a hierachial, associative array.
    foreach ($row as $field) {
      if ($vars['options']['field_output'] === 'normal') {
        if ($field->label) {
          $label = $plaintext_output ? strip_tags($field->label) : $field->label;
        }
        else {
          $label = $plaintext_output ? strip_tags($field->id) : $field->id;
        }
        if (!$field->is_multiple) {
          $content = $plaintext_output ? strip_tags($field->content) : $field->content;
        }
        else {
          $content = array();
          foreach ($field->content as $n => $oc) {
            $content[$n] = ($plaintext_output ? strip_tags($oc) : $oc);
          }
        }
      }
      elseif ($vars['options']['field_output'] === 'raw') {
        $label = $plaintext_output ? strip_tags($field->id) : $field->id;
        if (!$field->is_multiple) {
          $content = $plaintext_output ? strip_tags($field->raw) : $field->raw;
        }
        else {
          $content = array();
          foreach ($field->raw as $n => $oc) {
            $content[$n] = $plaintext_output ? strip_tags($oc) : $oc;
          }
        }
      }

      if ($top_child_object !== '') {
        $object[$top_child_object][$label] = $content;
      }
      else {
        $object[$label] = $content;
      }
    }

    $objects[] = $object;
  }

  if ($root_object !== '') {
    $vars['rows'] = array($root_object => $objects);
  }
  else {
    $vars['rows'] = $objects;
  }
}

function template_preprocess_views_views_json_style_exhibit(&$vars) {
  $root_object = 'Items';
  $top_child_object = $vars['options']['top_child_object'];
  $plaintext_output = $vars['options']['plaintext_output'];
  $objects = array();
  foreach ($vars['rows'] as $row) {
    $object = array($top_child_object => array());
    // Convert the rows into a hierachial, associative array.
    foreach ($row as $field) {
      if ($vars['options']['field_output'] === 'normal') {
        if ($field->label) {
          $label = $plaintext_output ? strip_tags($field->label) : $field->label;
        }
        else {
          $label = $plaintext_output ? strip_tags($field->id) : $field->id;
        }
        if (!$field->is_multiple) {
          $content = $plaintext_output ? strip_tags($field->content) : $field->content;
        }
        else {
          $content = array();
          foreach ($field->content as $n => $oc) {
            $content[$n] = ($plaintext_output ? strip_tags($oc) : $oc);
          }
        }
      }
      elseif ($vars['options']['field_output'] === 'raw') {
        $label = $plaintext_output ? strip_tags($field->id) : $field->id;
        if (!$field->is_multiple) {
          $content = $plaintext_output ? strip_tags($field->raw) : $field->raw;
        }
        else {
          $content = array();
          foreach ($field->raw as $n => $oc) {
            $content[$n] = $plaintext_output ? strip_tags($oc) : $oc;
          }
        }
      }

      $object[$top_child_object][$label] = $content;
    }

    if (!array_key_exists('label', $object)) {
      $object['label'] = 'Item';
    }
    if (!array_key_exists('type', $object)) {
      $object['type'] = $top_child_object;
    }

    $objects[] = $object;
  }

  $vars['rows'] = array($root_object => $objects);
}
