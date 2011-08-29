<?php

/**
 * @file
 * Implements data definitions on behalf of comment.module.
 */

/**
 * Implementation of hook_def().
 */
function comment_def() {
  $defs = array();

  $def = array(
    '#type' => 'entity',
    '#title' => t('Comment'),
    '#db_default_table' => 'comments',
    '#xml_plural' => 'comments',
    '#csv_plural' => 'comments',
    '#process' => array('_importexportapi_comment_process_thread' => array())
  );

  $def['cid'] = array(
    '#type' => 'int',
    '#title' => t('Comment ID'),
    '#key' => TRUE,
    '#db_uses_sequences' => TRUE,
    '#db_force_generate_id' => TRUE
  );
  $def['parent'] = array(
    '#type' => 'int',
    '#title' => t('Parent comment ID'),
    '#reference_entity' => 'comment',
    '#reference_field' => array('cid'),
    '#reference_delta' => 1,
    '#reference_parent' => FALSE,
    '#db_field_unaliased' => 'pid'
  );
  $def['nid'] = array(
    '#type' => 'int',
    '#title' => t('Node ID'),
    '#reference_entity' => 'node'
  );
  $def['uid'] = array(
    '#type' => 'int',
    '#title' => t('User ID'),
    '#reference_entity' => 'user'
  );
  $def['subject'] = array(
    '#title' => t('Subject')
  );
  $def['body'] = array(
    '#title' => t('Comment text'),
    '#db_field_unaliased' => 'comment'
  );
  $def['hostname'] = array(
    '#title' => t('Hostname')
  );
  $def['timestamp'] = array(
    '#type' => 'datetime',
    '#title' => t('Last modified date')
  );
  $def['status'] = array(
    '#type' => 'int',
    '#title' => t('Published')
  );
  $def['format'] = array(
    '#type' => 'int',
    '#title' => t('Input format'),
    '#reference_entity' => 'input_format'
  );
  $def['thread'] = array(
    '#title' => t('Thread position')
  );
  $def['commenter_name'] = array(
    '#title' => t('Commenter name'),
    '#db_field_unaliased' => 'name'
  );
  $def['commenter_mail'] = array(
    '#title' => t('Commenter e-mail address'),
    '#db_field_unaliased' => 'mail'
  );
  $def['commenter_homepage'] = array(
    '#title' => t('Commenter home page'),
    '#db_field_unaliased' => 'homepage'
  );

  $defs['comment'] = $def;

  return $defs;
}

/**
 * Implementation of hook_def_alter().
 */
function comment_def_alter(&$def) {
  if (isset($def['user'])) {
    $def['user']['mode'] = array(
      '#type' => 'int',
      '#title' => t('Comment viewing mode')
    );
    $def['user']['sort'] = array(
      '#type' => 'int',
      '#title' => t('Comment viewing order')
    );
    $def['user']['signature'] = array(
      '#title' => t('Signature')
    );
  }
}

/**
 * Implementation of hook_node_def_alter().
 */
function comment_node_def_alter(&$def, $type) {
  $def['comment'] = array(
    '#type' => 'int',
    '#title' => 'Comment setting'
  );

  $def['comment_statistics_nid'] = array(
    '#type' => 'int',
    '#title' => t('Node ID'),
    '#reference_entity' => $type,
    '#reference_field' => array('nid'),
    '#db_table' => 'node_comment_statistics',
    '#db_field_unaliased' => 'nid',
    '#key_component' => TRUE,
    '#csv_hidden' => TRUE
  );
  $def['last_comment_timestamp'] = array(
    '#type' => 'datetime',
    '#title' => 'Last comment date',
    '#db_table' => 'node_comment_statistics'
  );
  $def['last_comment_name'] = array(
    '#title' => 'Last comment username',
    '#default_value' => NULL,
    '#db_table' => 'node_comment_statistics',
    '#reference_entity' => 'user',
    '#reference_field' => array('uid'),
    '#alt_key_ignore' => TRUE
  );
  $def['last_comment_uid'] = array(
    '#type' => 'int',
    '#title' => 'Last comment user ID',
    '#db_table' => 'node_comment_statistics',
    '#reference_entity' => 'user',
    '#reference_field' => array('uid')
  );
  $def['comment_count'] = array(
    '#type' => 'int',
    '#title' => 'Comment count',
    '#db_table' => 'node_comment_statistics'
  );
}

/**
 * Implementation of hook_db_def_tables().
 */
function comment_db_def_tables() {
  $tables = array();

  $tables['comments'] = 'c';
  $tables['node_comment_statistics'] = 'ncs';

  return $tables;
}

/**
 * Callback for processing of the 'thread' field in the 'comment' entity
 * definition.
 */
function _importexportapi_comment_process_thread($field_id, $field, $format, $op, $source_get) {
  static $thread_cache = array(), $thread_parents = array();

  if ($op == 'put' && $format == 'db') {
    if (empty($field['thread']['#value']) && !empty($field['cid']['#value'])) {
      $thread = '';

      // Here we are building the thread field.  See the comment
      // in comment_render(). Copied from comment_save().
      if ($field['parent']['#value'] == 0) {
        // This is a comment with no parent comment (depth 0): we start
        // by retrieving the maximum thread level.
        $thread = db_result(db_query('SELECT MAX(thread) FROM {comments} WHERE nid = %d', $field['nid']['#value']));

        if (isset($thread_cache[$field['nid']['#value']][0]) && $thread_cache[$field['nid']['#value']][0] > $thread) {
          $thread = $thread_cache[$field['nid']['#value']][0];
        }

        // Strip the "/" from the end of the thread.
        $thread = rtrim($thread, '/');

        // Finally, build the thread field for this new comment.
        $thread = int2vancode(vancode2int($thread) + 1) .'/';

        $thread_cache[$field['nid']['#value']][0] = $thread;
      }
      else {
        // This is comment with a parent comment: we increase
        // the part of the thread value at the proper depth.
        $thread = isset($thread_cache[$field['nid']['#value']][$field['parent']['#value']]) ? $thread_cache[$field['nid']['#value']][$field['parent']['#value']] : '';

        // Strip the "/" from the end of the parent thread.
        $thread = (string) rtrim((string) $thread, '/');

        $parent_thread = $thread_cache[$field['nid']['#value']][0];
        if (isset($thread_parents[$field['parent']['#value']])) {
          $parent_thread = $thread_cache[$field['nid']['#value']][$thread_parents[$field['parent']['#value']]];
        }
        $parent_thread = (string) rtrim((string) $parent_thread, '/');

        if ($thread == '') {
          // First child of this parent.
          $thread = $parent_thread .'.'. int2vancode(1) .'/';
        }
        else {
          // We need to get the value at the correct depth.
          $parts = explode('.', $thread);
          $depth = count($parts);
          $last = $parts[$depth-1];

          // Finally, build the thread field for this new comment.
          $thread = $parent_thread .'.'. int2vancode(vancode2int($last) + 1) .'/';
        }

        $thread_cache[$field['nid']['#value']][$field['parent']['#value']] = $thread;
        $thread_parents[$field['cid']['#value']] = $field['parent']['#value'];
      }

      $field['thread']['#value'] = $thread;
    }
  }

  return $field;
}
