<?php

/**
 * @file
 * The 'db' 'get' engine for the Import / Export API. This is the default
 * 'source' for all entities, and as such, it is also the default 'get' engine
 * for exports.
 */

require_once(drupal_get_path('module', 'importexportapi') .'/engines/importexportapi_db.inc');

/**
 * Callback for querying the database, and returning the results in a
 * structured array.
 */
function importexportapi_db_get($def) {
  static $placeholder_map, $tables;

  $data = array();

  if (!isset($placeholder_map)) {
    $placeholder_map = importexportapi_get_field_type_info(NULL, 'placeholders');
  }

  if (!isset($tables)) {
    $tables = importexportapi_db_get_def_tables();
  }

  // Convert the data definition into an array structure that more closely
  // resembles the queries that need to be run. For most of the tasks to
  // follow, we can work with this structure instead of the data definition
  // itself, making life much easier.
  $queries = _importexportapi_db_get_queries($def);
  foreach ($queries as $index => $query) {
    $select = array();
    $from = NULL;
    $join = array();
    $where = array();
    $args = array();
    $primary_table_alias = NULL;

    // Build the SQL that lists the tables and fields from which to get the
    // data. The primary table goes in the 'FROM' clause, and all other tables
    // go in the 'JOIN' clause.
    foreach ($query['tables'] as $table => $fields) {
      $table_alias = $tables[$table];
      if ($table == $query['primary_table']) {
        $from = '{'. $table .'} '. $table_alias;
        $primary_table_alias = $table_alias;
      }
      // Only construct joins on non-primary tables that have a reference field
      // defined, otherwise there is no way to link these tables to the primary
      // table.
      if (isset($query['references'][$table])) {
        $reference_joins = array();
        foreach ($query['references'][$table] as $reference) {
          $table_alias_temp = $table_alias;
          $reference_table_alias = $tables[$reference['reference_table']];
          $index_alias = $table_alias_temp;
          if (!empty($reference['reference_delta'])) {
            if ($reference['reference_table'] != $table && empty($reference['self_join'])) {
              $reference_table_alias .= $reference['reference_delta'];
              $index_alias = $reference_table_alias;
            }
            else {
              $table_alias_temp .= $reference['reference_delta'];
              $index_alias = $table_alias_temp;
            }
          }

          $reference_join_left = $reference_table_alias .'.'. $reference['field_unaliased'];
          $reference_join_right = $table_alias_temp .'.'. $reference['reference_field_unaliased'];

          if ($index_alias != $primary_table_alias) {
            $reference_joins[$index_alias][] = $reference_join_left .' = '. $reference_join_right;
          }
        }

        foreach ($reference_joins as $curr_alias => $reference_join_array) {
          $join[] = 'LEFT JOIN {'. $table .'} '. $curr_alias .' ON '. implode(' AND ', $reference_join_array);
        }
      }

      foreach ($fields as $field) {
        $select_stmt = $table_alias . (isset($field['delta']) ? $field['delta'] : '') .'.'. $field['field_unaliased'];
        if ($field['field_unaliased'] != $field['field']) {
          $select_stmt .= ' AS '. $field['field'];
        }

        $select[] = $select_stmt;

        // Perform filtering on selected fields.
        if (!empty($field['filter']) && !empty($field['filter']['values'])) {
          $filter_sql = $table_alias .'.'. $field['field_unaliased'] .' ';
          $value_placeholder = $placeholder_map[$field['type']];

          if (count($field['filter']['values']) > 1) {
            $value_placeholders = implode(', ', $value_placeholder);
            $in_sql = 'IN ('. $value_placeholders .')';

            if ($field['filter']['operator'] == 'neq') {
              $in_sql = 'NOT '. $in_sql;
            }

            $filter_sql .= $in_sql;
          }
          else {
            $operator = '=';
            $no_placeholder_ops = array('between', 'BETWEEN', 'not between', 'NOT BETWEEN', 'is', 'IS', 'is not', 'IS NOT');

            if (!empty($field['filter']['operator']) && $field['filter']['operator'] != 'eq' && $field['filter']['operator'] != 'neq') {
              $operator = $field['filter']['operator'];
            }

            $eq_sql = $operator .' '. (in_array($field['filter']['operator'], $no_placeholder_ops) ? $field['filter']['values'] : $value_placeholder);

            if (isset($field['filter']['operator']) && $field['filter']['operator'] == 'neq') {
              $eq_sql = '!'. $eq_sql;
            }

            $filter_sql .= $eq_sql;
          }

          $where[] = $filter_sql;
          if (!in_array($field['filter']['operator'], $no_placeholder_ops)) {
            $args = array_merge($args, $field['filter']['values']);
          }
        }
      }
    }

    // Concatenate all the SQL together to make the final query string.
    $sql = 'SELECT '. implode(', ', $select) .' FROM '. $from;
    if (!empty($join)) {
      $sql .= ' '. implode(' ', $join);
    }
    if (!empty($where)) {
      $sql .= ' WHERE '. implode(' AND ', $where);
    }

    $count = 0;
    $field_count = array();
    $query_resource = db_query($sql, $args);
    while ($result = db_fetch_object($query_resource)) {
      if ($index == 0) {
        $data[$count] = $def;

        foreach (element_children($data[$count]) as $field) {
          if ($data[$count][$field]['#type'] == 'array') {
            // Initialise all array fields to an empty array, which will get
            // populated when the query for each array gets run.
            $data[$count][$field]['#value'] = array();
          }
          else {
            // Populate this top-level field with its value, or with the
            // default value for that field type if none was found.
            $data[$count][$field]['#value'] = isset($result->$field) ? $result->$field : $data[$count][$field]['#default_value'];
          }
        }

        $count++;
      }
      else {
        foreach ($data as $data_index => $data_fields) {
          // Because we're doing everything on a per-query-result basis, but
          // we're dealing with multiple array fields that need to be
          // populated, keep a global-ish counter going for each array field
          // within each entity instance.
          if (!isset($field_count[$data_index])) {
            $field_count[$data_index] = array();
          }

          // Result values for array fields only get included if at least one
          // sub-field of the array references a field in the parent entity.
          // Otherwise, we have no way of linking array fields to the correct
          // entity.
          // TODO: review performance of this reference searching system. May
          // need to introduce lookup arrays for indexing referenced values.
          if (isset($query['parent_references'])) {
            $parent_match = TRUE;
            foreach ($query['parent_references'] as $reference_field => $reference) {
              $result_key = $def[$query['field']][$reference['field']]['#get']['#db_field'];
              if (isset($result->$result_key) && $data[$data_index][$reference_field]['#value'] != $result->$result_key) {
                $parent_match = FALSE;
                break;
              }
            }

            if ($parent_match) {
              // Recurse down to the correct level of nested arrays, and set
              // values within those arrays.
              $array_field = isset($query['parents'][0]) ? $query['parents'][0] : $query['field'];
              _importexportapi_db_get_array_values($data[$data_index][$array_field], $field_count[$data_index], $query, $result);
            }
          }
        }
      }
    }
  }

  return $data;
}

/**
 * Helper function for importexportapi_db_get(). Converts a data
 * definition into an array representation of a set of queries.
 *
 * @param $def
 *   The data definition array for an entity, as returned by
 *   importexportapi_get_def().
 * @param $index
 *   The index (key) of the current query within the $queries array. Internal
 *   use only: leave at default value (NULL) when calling from other functions.
 * @param $parent_index
 *   The index (key) of the current query's parent query within the $queries
 *   array. Internal use only: leave at default value (NULL) when calling from
 *   other functions.
 * @param $depth
 *   The number of ancestor queries that this query has. Internal use only:
 *   leave at default value (NULL) when calling from other functions.
 * @param $queries
 *   An array in which each element is an array structure representing a
 *   database query. Internal use only: leave at default value (NULL) when
 *   calling from other functions.
 *
 * @return
 *   A structured array that represents a set of queries.
 */
function _importexportapi_db_get_queries($def, $index = NULL, $parent_index = NULL, $depth = NULL, $queries = NULL) {
  // Variable used to keep track of the key of the newest query in the
  // $queries array. It is remembered throughout recursive calls, but it is
  // reset when this function is called from other functions.
  static $index_count;

  $parent_index_ref = NULL;
  if (!isset($parent_index)) {
    // Query 0 is always the parent of all other queries
    $parent_index_ref = $index_count = $index = $depth = 0;
    $queries = array();
  }
  else {
    // If this function calls itself again, the current query becomes the
    // parent of the next query.
    $parent_index_ref = $index;
  }

  $queries[$index]['primary_table'] = $def['#get']['#db_default_table'];
  $queries[$index]['depth'] = $depth;

  foreach (element_children($def) as $field_id) {
    $field = $def[$field_id];
    $get = $field['#get'];

    // All fields that are not arrays are part of the current query.
    if ($field['#type'] != 'array') {
      $queries[$index]['tables'][$get['#db_table']][$get['#db_field']] = array('field' => $get['#db_field'], 'field_unaliased' => $get['#db_field_unaliased'], 'type' => $field['#type'], 'reference_delta' => (isset($get['#db_reference_delta']) ? $get['#db_reference_delta'] : NULL), 'filter' => (isset($get['#db_filter']) ? $get['#db_filter'] : NULL), 'delta' => (isset($get['#db_delta']) ? $get['#db_delta'] : NULL));
    }
  }

  foreach (element_children($def) as $field_id) {
    $field = $def[$field_id];
    $get = $field['#get'];

    if ($field['#type'] != 'array') {
      if (!empty($get['#db_reference_table'])) {
        // Fields that reference the primary table of the parent
        // query need to be tracked, as they are used for constructing
        // some of the filters.
        $is_child_query = isset($parent_index) && (!isset($get['#db_reference_parent']) || $get['#db_reference_parent']);
        if ($is_child_query && $get['#db_reference_table'] == $queries[$parent_index]['primary_table']) {
          $queries[$index]['parent_references'][$get['#db_reference_field']] = array(
            'table' => $get['#db_table'],
            'field' => $get['#db_field'],
            'field_unaliased' => $get['#db_field_unaliased'],
            'reference_field_orig' => $field['#reference_field'],
            'parents' => $field['#parents'],
            'id' => $field['#id']
          );
        }
        // Fields that reference the primary table of this query also need to
        // be tracked, as they are used to construct the joins.
        $do_join = 0;
        if (isset($queries[$index]['tables'][$get['#db_reference_table']][$get['#db_reference_field']])) {
          $do_join = 1;
        }
        elseif (isset($parent_index) && isset($queries[$parent_index]['tables'][$get['#db_reference_table']][$get['#db_reference_field']])) {
          $do_join = 2;
        }
        if ($do_join) {
          if ($do_join == 2 && !empty($get['#db_reference_delta'])) {
            $queries[$index]['references'][$get['#db_reference_table']][] = array(
              'field' => $get['#db_field'],
              'field_unaliased' => $get['#db_field_unaliased'],
              'reference_table' => $get['#db_table'],
              'reference_field' => $get['#db_reference_field'],
              'reference_field_unaliased' => $get['#db_reference_field_unaliased'],
              'reference_delta' => $get['#db_reference_delta'],
              'self_join' => TRUE
            );
          }
          elseif ($get['#db_table'] != $queries[$index]['primary_table']) {
            $queries[$index]['references'][$get['#db_table']][] = array(
              'field' => $get['#db_field'],
              'field_unaliased' => $get['#db_reference_field_unaliased'],
              'reference_table' => $get['#db_reference_table'],
              'reference_field' => $get['#db_reference_field'],
              'reference_field_unaliased' => $get['#db_field_unaliased'],
              'reference_delta' => $get['#db_reference_delta']
            );
          }
          else {
            $queries[$index]['references'][$get['#db_reference_table']][] = array(
              'field' => $get['#db_field'],
              'field_unaliased' => $get['#db_field_unaliased'],
              'reference_table' => $get['#db_table'],
              'reference_field' => $get['#db_reference_field'],
              'reference_field_unaliased' => $get['#db_reference_field_unaliased'],
              'reference_delta' => $get['#db_reference_delta']
            );
          }
        }
      }
      // Generated alternate key fields do not define any references of their
      // own, since they share the reference of their master key. But we still
      // need to construct joins for them.
      elseif (isset($field['#generated'])) {
        $key_field = $def[$field['#alt_key_for']];
        $key_get = $key_field['#get'];
        if (!isset($queries[$index]['references'][$key_get['#db_reference_table']])) {
          $queries[$index]['references'][$key_get['#db_reference_table']][] = array(
            'reference_table' => $key_get['#db_table'],
            'field' => $key_get['#db_field'],
            'field_unaliased' => $key_get['#db_field_unaliased'],
            'reference_field' => $key_get['#db_reference_field'],
            'reference_field_unaliased' => $key_get['#db_reference_field_unaliased'],
            'reference_delta' => (isset($get['#db_reference_delta']) ? $get['#db_reference_delta'] : NULL)
          );
        }
      }
    }
    // Each array field gets its own query, and all the fields within the array
    // become part of that query.
    else {
      $index_count++;
      $queries[$index_count]['field'] = $field_id;
      $queries[$index_count]['parents'] = $field['#parents'];
      array_shift($queries[$index_count]['parents']);

      $queries = _importexportapi_db_get_queries($field, $index_count, $parent_index_ref, $depth+1, $queries);
    }
  }

  return $queries;
}

/**
 * Helper function for importexportapi_db_get(). Populates a field, and
 * all of its children (recursively), with the values found in the specified
 * database query result set.
 *
 * @param &$data
 *   A field from a data definition, and its children. When called by other
 *   functions, this is generally a top-level field of an entity. This field is
 *   passed back by reference in its populated state.
 * @param &$field_count
 *   An array of counters, to track the number of instances of a particular
 *   field that is encountered, as a child of $data.
 * @param $query
 *   An array representing the query that is currently being processed.
 * @param $result
 *   A database query result set, as returned by db_fetch_object(). Used to
 *   populate the value of $data and its children.
 * @param $parent_data
 *   The parent field of $data. Internal use only: leave at default value
 *   (NULL) when calling from other functions.
 * @param $parent_index
 *   The key of the current field ($data) within its parent array field.
 *   Internal use only: leave at default value (NULL) when calling from other
 *   functions.
 */
function _importexportapi_db_get_array_values(&$data, &$field_count, $query, $result, $parent_data = NULL, $parent_index = NULL) {
  // Each time that this function calls itself, we check the top-level parent,
  // And pop it off the beginning of the array. By doing this, we recurse down
  // through the parents, until we reach the array itself.
  $parent_field = !empty($query['parents']) ? array_shift($query['parents']) : NULL;

  // With nested arrays, it's only a matter of time before $parent_field is
  // NULL, at which point this variable gets set to the array itself.
  $array_field = isset($parent_field) ? $parent_field : $query['field'];

  // If $parent_field is set, then we still need to recurse down through more
  // parent arrays.
  $next_parent = isset($query['parents'][0]) ? $query['parents'][0] : $query['field'];
  if (isset($parent_field) && !empty($data['#value'])) {
    foreach (element_children($data['#value']) as $index) {
      _importexportapi_db_get_array_values($data['#value'][$index][$next_parent], $field_count, $query, $result, $data, $index);
    }
  }
  else {
    // Result values for array fields only get included if at least one
    // sub-field of the array references a field in the parent entity.
    // Otherwise, we have no way of linking array fields to the correct
    // entity.
    // TODO: review performance of this reference searching system. May
    // need to introduce lookup arrays for indexing referenced values.
    if (isset($query['parent_references'])) {
      $parent_match = TRUE;
      if ($query['depth'] > 1 && isset($parent_data)) {
        foreach ($query['parent_references'] as $reference_field => $reference) {
          $result_key = $parent_data['#value'][$parent_index][$reference['field']]['#get']['#db_field'];
          if ($parent_data['#value'][$parent_index][$reference_field]['#value'] != $result->$result_key) {
            $parent_match = FALSE;
            break;
          }
        }
      }

      if ($parent_match) {
        // Initialise the counter for this field afresh, whenever it is
        // the first instance to get appended to this parent.
        if (empty($data['#value'])) {
          $field_count[$array_field] = 0;
        }

        if (!isset($data['#value'][$field_count[$array_field]])) {
          $data['#value'][$field_count[$array_field]] = array();
        }

        // Copy the definition for the array's child fields into the
        // '#values' array, for each result set found.
        foreach ($data as $child_id => $child) {
          if ($child_id != '#value') {
            $data['#value'][$field_count[$array_field]][$child_id] = $child;
          }
        }

        // Populate this field with its value, within the corresponding
        // parent entity.
        foreach (element_children($data) as $child) {
          $child_info = $data['#value'][$field_count[$array_field]][$child];
          $child_info_db_field = $child_info['#get']['#db_field'];
          $child_info['#value'] = isset($result->$child_info_db_field) ? $result->$child_info_db_field : $child_info['#default_value'];

          $data['#value'][$field_count[$array_field]][$child] = $child_info;
        }

        $field_count[$array_field]++;
      }
    }
  }
}
