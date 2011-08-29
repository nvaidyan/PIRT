<?php

/**
 * @file
 * Implements data definitions on behalf of blog.module.
 */

/**
 * Implementation of hook_def().
 */
function blog_def() {
  $defs = array();

  $type = 'blog';
  $def = importexportapi_node_get_def($type);

  $type_info = module_invoke($type, 'node_info');
  $type_info = $type_info[$type];
  $def['#title'] = $type_info['name'];
  $def['#title'][0] = strtoupper($def['#title'][0]);

  $def['#xml_plural'] = $def['#csv_plural'] = 'blog-entries';
  $def['revisions']['#csv_plural'] = 'blog-revisions';

  $def['type']['#db_filter'] = array(
    'values' => array($type)
  );

  $defs['blog'] = $def;

  return $defs;
}
