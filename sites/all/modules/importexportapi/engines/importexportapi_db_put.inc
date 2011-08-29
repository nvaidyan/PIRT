<?php

/**
 * @file
 * The 'db' 'put' engine for the Import / Export API. This is the default
 * 'source' for all entities, and as such, it is also the default 'put' engine
 * for imports.
 */

require_once(drupal_get_path('module', 'importexportapi') .'/engines/importexportapi_db.inc');

/**
 * Callback for inserting structured data into the database.
 */
function importexportapi_db_put($data, $variables) {
  static $placeholder_map;

  if (!isset($placeholder_map)) {
    $placeholder_map = importexportapi_get_field_type_info(NULL, 'placeholders');
  }

  $put_map = array();
  if (isset($variables['put_id'])) {
    $put_map = _importexportapi_db_put_maps_load($variables['put_id']);
  }

  importexportapi_db_put_generate_key_ids($data, $put_map, $placeholder_map);

  $queries = array();
  foreach ($data as $entity_index => $entity) {
    // Convert the data definition and its values into an array structure that
    // more closely resembles the queries that need to be run. For most of the
    // tasks to follow, we can work with this structure instead of the data
    // definition itself, making life much easier.
    $queries = array_merge($queries, _importexportapi_db_put_queries($entity));
  }

  foreach ($queries as $index => $query) {
    // Build the SQL that lists the tables and fields into which the data
    // should be inserted. The primary table goes in the 'INTO' clause.
    foreach ($query['tables'] as $table => $fields) {
      $insert = array();
      $into = '{'. $table .'}';
      $where = array();
      $args = array();
      $values = array();
      $placeholders = array();
      $unique_fields = array();
      $key_fields = array();

      // Tracks whether or not the query will be run at all.
      $do_query = TRUE;
      // Tracks whether the raw query is an INSERT or an UPDATE.
      $is_insert = $query['is_insert'][$table];
      // Tracks whether or not to try and update an existing database entry.
      $update_existing = $update_existing_temp = FALSE;

      foreach ($fields as $field) {
        $insert[] = $field['field_unaliased'];
        $value = $field['value'];
        $value_old = $field['value_old'];
        $update_existing_temp = $field['update_existing'];

        if (!empty($field['key']) || !empty($field['key_component'])) {
          if (!$is_insert) {
            $where[] = $field['field_unaliased'] .' = '. $placeholder_map[$field['type']];
            $args[] = $value;
          }
        }

        if ($update_existing_temp && isset($query['keys'][$field['field_unaliased']])) {
          $key_fields[$field['field_unaliased']] = $field;
        }

        // Map the old ID value to the new ID value, for fields that are
        // references to key fields.
        if (!empty($field['reference_entity']) && empty($field['inherited_value'])) {
          $reference_entity = $field['reference_entity'];
          $reference_path = implode('.', $field['reference_field']);
          $value_old = $value;

          $put_map_value = importexportapi_db_put_map_get($put_map, $field, $value_old);
          if (isset($put_map_value)) {
            // Override the value of this field with its new mapped value, if
            // one has been defined.
            $value = $put_map_value;
          }
        }

        if (!empty($field['unique'])) {
          $unique_fields['sql'][] = $field['field_unaliased'] .' = '. $placeholder_map[$field['type']];
          $unique_fields['values'][] = $value;
        }

        $values[] = $value;
        $placeholders[] = $placeholder_map[$field['type']];
        $update_existing = $update_existing_temp ? $update_existing_temp : $update_existing;
      }

      // If the 'update_existing' flag is set, then attempt to override the
      // value of an existing entry in the database, if such an entry can be
      // found.
      if ($update_existing && $is_insert) {
        $reference_sql = 'SELECT * FROM '. $into;
        $reference_where = array();
        $reference_args = array();

        foreach ($key_fields as $key_field => $key_values) {
          $reference_where[] = $key_field .' = '. $placeholder_map[$key_values['type']];
          $reference_args[] = $key_values['value'];
        }

        if (!empty($reference_where)) {
          $reference_sql .= ' WHERE '. implode(' AND ', $reference_where);

          $result = db_fetch_object(db_query($reference_sql, $reference_args));
          if (!empty($result)) {
            $is_insert = FALSE;
            $where = array_merge($where, $reference_where);
            $args = array_merge($args, $reference_args);
          }
        }
      }

      // Check that no values already exist, for any of the fields that are
      // marked as 'unique'. If any values do already exist for these fields,
      // then abort this query altogether.
      if ($is_insert && !empty($unique_fields)) {
        $unique_fields_sql = 'SELECT * FROM {'. $into .'} WHERE '. implode(' OR ', $unique_fields['sql']);
        $result = db_fetch_object(db_query($unique_fields_sql, $unique_fields['values']));

        if (!empty($result)) {
          $do_query = FALSE;

          // Don't bother processing any more fields - we're not going to
          // execute the query at all.
          break;
        }
      }

      if ($do_query) {
        $sql = NULL;

        // Concatenate all the SQL together to make the final query string.
        if ($is_insert) {
          $sql = 'INSERT INTO '. $into;
          if (!empty($insert)) {
            $sql .= ' ('. implode(', ', $insert) .')';
          }
          if (!empty($placeholders)) {
            $sql .= ' VALUES ('. implode(', ', $placeholders) .')';
          }
        }
        else {
          $sql = 'UPDATE '. $into;
          $sql_update = array();
          if (!empty($insert)) {
            $sql .= ' SET ';

            foreach ($insert as $key => $field) {
              $sql_update[] = $field .' = '. $placeholders[$key];
            }

            $sql .= implode(', ', $sql_update);
          }
        }

        if (!empty($where)) {
          $sql .= ' WHERE '. implode(' AND ', $where);
        }
        $args = array_merge($values, $args);

        db_query($sql, $args);
      }
    }
  }

  if (isset($variables['put_id'])) {
    _importexportapi_db_put_maps_save($variables['put_id'], $put_map);
  }

  return 0;
}

/**
 * Helper function for importexportapi_db_put(). Generates a unique ID for the
 * specified field, using either Drupal's database sequencing system, or a
 * database-specific sequencing system. Note that if a database-specific
 * sequencing system is used, then a dummy row will be inserted into the
 * database, with the newly generated ID as its key.
 *
 * @param $field
 *   The field for which to generate a unique ID.
 * @param $table
 *   The table that the field belongs in (with curly brackets already added).
 * @param &$is_insert
 *   Whether or not the row for this field will need to be entered into the
 *   database using an INSERT statement at a later stage. If a database-
 *   specific sequencing system is used, then this value gets set to FALSE, and
 *   is passed back by reference.
 *
 * @return
 *   The newly generated ID for the specified field.
 */
function _importexportapi_db_put_sequences($field, $table, &$is_insert, $old_id, $placeholder_map) {
  $new_id = NULL;
  $field_put = $field['#put'];

  if ($field_put['#db_uses_sequences'] && $GLOBALS['db_type'] != 'pgsql') {
    $new_id = db_last_insert_id($table, $field_put['#db_field_unaliased']);
  }
  else {
    switch ($GLOBALS['db_type']) {
      case 'mysql':
      case 'mysqli':
        db_query('INSERT INTO '. $table .' ('. $field_put['#db_field_unaliased'] .') VALUES (NULL)');
        $new_id = db_fetch_object(db_query('SELECT LAST_INSERT_ID() AS last_insert_id'));
        break;
      case 'pgsql':
        db_query('INSERT INTO '. $table .' ('. $field_put['#db_field_unaliased'] .') VALUES (DEFAULT)');
        $last_insert_id = $table .'_'. $field_put['#db_field_unaliased'] .'_seq';
        $new_id = db_fetch_object(db_query('SELECT last_value AS last_insert_id from '. $last_insert_id));
        break;
    }

    $new_id = $new_id->last_insert_id;

    if (!$is_insert[$field_put['#db_table']]) {
      db_query('DELETE FROM '. $table .' WHERE '. $field_put['#db_field_unaliased'] .' = '. $placeholder_map[$field['#type']], $new_id);
    }

    $is_insert[$field_put['#db_table']] = FALSE;
  }

  return $new_id;
}

/**
 * Helper function for importexportapi_db_put(). Converts a data set into an
 * array representation of a set of queries.
 *
 * @param $data
 *   The data set array for an entity, as returned by
 *   importexportapi_get_data().
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
function _importexportapi_db_put_queries($data, $index = NULL, $parent_index = NULL, $depth = NULL, $queries = NULL) {
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

  if ($data['#put']['#db_do_query']) {
    $queries[$index]['primary_table'] = $data['#put']['#db_default_table'];
    $queries[$index]['is_insert'] = $data['#put']['#db_is_insert'];
    $queries[$index]['keys'] = !empty($data['#put']['#db_keys']) ? $data['#put']['#db_keys'] : array();
    $queries[$index]['depth'] = $depth;

    foreach (element_children($data) as $field_id) {
      $field = $data[$field_id];
      $put = $field['#put'];

      // All fields that are not arrays are part of the current query.
      if ($field['#type'] != 'array' && empty($field['#generated'])) {
        $new_field = array('field_unaliased' => $put['#db_field_unaliased'], 'type' => $field['#type'], 'value' => $field['#value'], 'value_old' => (isset($field['#db_value_old']) ? $field['#db_value_old'] : NULL), 'key' => (isset($field['#key']) ? $field['#key'] : NULL), 'reference_entity' => (isset($field['#reference_entity']) ? $field['#reference_entity'] : NULL), 'reference_field' => (isset($field['#reference_field']) ? $field['#reference_field'] : NULL), 'id' => $field['#id'], 'parents' => $field['#parents'], 'unique' => $field['#unique'], 'key_component' => (isset($field['#key_component']) ? $field['#key_component'] : NULL));
        if (!empty($field['#key']) || !empty($field['#key_component']) || !empty($field['#reference_entity'])) {
          $new_field += array('update_existing' => $put['#db_update_existing'], 'inherited_value' => ($put['#db_inherited_value'] ? $put['#db_inherited_value'] : FALSE), 'parent_data' => $data);
        }

        $queries[$index]['tables'][$put['#db_table']][] = $new_field;
      }
    }

    foreach (element_children($data) as $field_id) {
      $field = $data[$field_id];
      $put = $field['#put'];

      if (!empty($field['#generated'])) {
        continue;
      }

      if ($field['#type'] != 'array') {
        if (!empty($put['#db_reference_table'])) {
          $is_child_query = isset($parent_index);
          // Fields that reference the primary table of this query need to
          // be tracked, as they are used to construct the joins.
          if (isset($queries[$index]['tables'][$put['#db_reference_table']])) {
            if ($is_child_query) {
              $queries[$index]['references'][$put['#db_reference_table']][] = array(
                'field' => $put['#db_field'],
                'field_unaliased' => $put['#db_field_unaliased'],
                'reference_table' => $put['#db_table'],
                'reference_field' => $put['#db_reference_field'],
                'reference_field_unaliased' => $put['#db_reference_field_unaliased']
              );
            }
            else {
              $queries[$index]['references'][$put['#db_table']][] = array(
                'field' => $put['#db_field'],
                'field_unaliased' => $put['#db_field_unaliased'],
                'reference_table' => $put['#db_reference_table'],
                'reference_field' => $put['#db_reference_field'],
                'reference_field_unaliased' => $put['#db_reference_field_unaliased']
              );
            }
          }
        }
      }
      // Each array field gets its own query, and all the fields within the array
      // become part of that query.
      else {
        if (!empty($field['#value']) && is_array($field['#value'])) {
          foreach ($field['#value'] as $child => $child_data) {
            if ($child_data['#put']['#db_do_query']) {
              $index_count++;
              $queries[$index_count]['field'] = $field_id;
              $queries[$index_count]['parents'] = $data[$field_id]['#parents'];

              $queries = _importexportapi_db_put_queries($child_data, $index_count, $parent_index_ref, $depth+1, $queries);
            }
          }
        }
      }
    }
  }

  return $queries;
}

/**
 * 'Process' callback for this engine. Examines the set of data provided, and
 * looks for any 'key' fields whose value is NULL. For all such fields, an
 * attempt is made to find an existing key in the database, using any available
 * alternate keys. If alternate keys can be resolved to existing actual keys,
 * then those keys are assigned to the fields.
 *
 * @param data
 *   The set of data for which to try and resolve alternate keys.
 */
function importexportapi_db_put_resolve_alt_keys($data) {
  static $placeholder_map;

  if (!isset($placeholder_map)) {
    $placeholder_map = importexportapi_get_field_type_info(NULL, 'placeholders');
  }

  foreach (array_keys($data) as $data_index) {
    if (is_numeric($data_index)) {
      // Recursively resolve alternate keys.
      _importexportapi_db_put_resolve_alt_keys($data[$data_index], $placeholder_map);
    }
  }

  return $data;
}

/**
 * Helper function for importexportapi_db_put_resolve_alt_keys(). Attempts to
 * resolve alternate keys recursively, for $data and all its children.
 *
 * @param &$data
 *   The field for which to possibly resolve alternate keys. Any values within
 *   this variable may be changed and passed back by reference.
 * @param $placeholder_map
 *   A map of field type placeholders.
 */
function _importexportapi_db_put_resolve_alt_keys(&$data, $placeholder_map) {
  if ($data['#type'] == 'entity' || $data['#type'] == 'array') {
    // Let's look for 'key' fields.
    foreach (element_children($data) as $key_index) {
      // We're only interested in 'key' fields or 'reference' fields whose
      // value is NULL.
      if ((!empty($data[$key_index]['#key']) || !empty($data[$key_index]['#key_component']) || !empty($data[$key_index]['#reference_entity'])) && !isset($data[$key_index]['#value']) && empty($data[$key_index]['#alt_key_for'])) {
        $key_field = $data[$key_index];

        $ref_field = $key_field;
        if (!empty($data[$key_index]['#reference_entity'])) {
          $ref_array = array_merge(array($data[$key_index]['#reference_entity']), $data[$key_index]['#reference_field']);
          $ref_field = importexportapi_get_def($ref_array);
        }

        $ref_put = $ref_field['#put'];
        $ref_db_field = $ref_put['#db_field_unaliased'];

        // Now that we've found a NULL 'key' field, let's look for an alternate
        // key field for it.
        foreach (element_children($data) as $alt_index) {
          // We want an alternate key field whose value is not NULL, so that we
          // can resolve it.
          if (!empty($data[$alt_index]['#alt_key_for']) && $data[$alt_index]['#alt_key_for'] == $key_index && (isset($data[$alt_index]['#value']) && $data[$alt_index]['#value'] !== '')) {
            $alt_field = $data[$alt_index];
            $alt_put = $alt_field['#put'];

            // Find the 'key' value in the database, of a row where the
            // alternate key's value matches the value of the alternate key
            // that we're looking for.
            $query = 'SELECT '. $ref_db_field .' FROM {'. $ref_put['#db_table'] .'} WHERE '. $alt_put['#db_field_unaliased'] .' = '. $placeholder_map[$alt_field['#type']];
            $result = db_fetch_object(db_query($query, $alt_field['#value']));

            if (!empty($result) && isset($result->$ref_db_field) && $result->$ref_db_field !== '') {
              // We managed to resolve the alternate key to an actual 'key'
              // value - now, copy the actual 'key' value to the 'key' field.
              importexportapi_string_typecast($ref_field['#type'], $result->$ref_db_field);
              $data[$key_index]['#value'] = $result->$ref_db_field;

              // Record the alternate key that we used in the
              // '#db_resolved_alt_key' attribute of the 'key' field.
              $alt_field_array = array_merge($alt_field['#parents'], array($alt_field['#id']));
              $data[$key_index]['#db_resolved_alt_key'] = $alt_field_array;

              // Once we resolve an alternate key, we stop searching for more.
              break;
            }
          }
        }
      }
    }
  }

  foreach ($data as $data_index => $data_fields) {
    if ($data_fields['#type'] == 'array' && !empty($data_fields['#value'])) {
      foreach (element_children($data_fields['#value']) as $child_index) {
        // Recurse down through all child fields.
        _importexportapi_db_put_resolve_alt_keys($data[$data_index]['#value'][$child_index], $placeholder_map);
      }
    }
  }
}

/**
 * Generate new IDs for key fields, where applicable.
 * For an explanation of the case numbers marked with comments in the code
 * below, see the ID handling scenario matrix at:
 * http://basement.greenash.net.au/soc2006/IdHandlingScenarioMatrix
 *
 * @param $data
 *   The set of data for which to try and generate IDs for 'key' fields. This
 *   is passed back by reference.
 * @param $put_map
 *   The old-value to new-value mappings of all fields that have their values
 *   changed during this 'put' operation.
 * @param $placeholder_map
 *   A map of field type placeholders.
 */
function importexportapi_db_put_generate_key_ids(&$data, &$put_map, $placeholder_map) {
  foreach (array_keys($data) as $data_index) {
    // Generate IDs for 'key' fields recursively.
    _importexportapi_db_put_generate_key_ids($data[$data_index], $put_map, $placeholder_map);

    if (isset($data[$data_index]['#put']['#db_process_generate_id'])) {
      foreach ($data[$data_index]['#put']['#db_process_generate_id'] as $process => $args) {
        if (function_exists($process)) {
          // Call the 'db_process_generate_id' callback for this field, is one
          // is defined.
          $args = array_merge(array($data[$data_index]), $args);
          $data[$data_index] = call_user_func_array($process, $args);
        }
      }
    }
  }
}

/**
 * Helper function for importexportapi_db_put_generate_key_ids(). Generates new
 * IDs for 'key' fields recursively, for $data and all its children.
 *
 * @param &$data
 *   The field for which to possibly generate a new ID. Any values within
 *   this variable may be changed and passed back by reference.
 * @param $put_map
 *   The old-value to new-value mappings of all fields that have their values
 *   changed during this 'put' operation.
 * @param $placeholder_map
 *   A map of field type placeholders.
 */
function _importexportapi_db_put_generate_key_ids(&$data, &$put_map, $placeholder_map, $child_ref_map = array()) {
  $do_query = FALSE;

  if ($data['#type'] == 'entity' || $data['#type'] == 'array') {
    // Tracks whether or not to run any query at all.
    $do_query = TRUE;
    // Tracks whether the raw query is an INSERT or an UPDATE.
    $is_insert = array();

    // Let's look for 'key' fields.
    foreach (element_children($data) as $key_index) {
      $key_field = $data[$key_index];
      $key_put = $key_field['#put'];
      $value = $key_field['#value'];

      if (empty($key_put['#db_table'])) {
        continue;
      }
      $into = '{'. $key_put['#db_table'] .'}';

      if (!isset($is_insert[$key_put['#db_table']])) {
        $is_insert[$key_put['#db_table']] = TRUE;
      }

      if (!empty($key_field['#key']) && empty($key_field['#alt_key_for'])) {
        $value_old = NULL;

        $key_provided = isset($value);
        $key_already_exists = (bool) db_result(db_query('SELECT COUNT('. $key_put['#db_field_unaliased'] .') FROM {'. $into .'} WHERE '. $key_put['#db_field_unaliased'] .' = '. $placeholder_map[$key_field['#type']], $value));
        $update_existing = $key_put['#db_update_existing'];
        $generate_id = $key_put['#db_generate_id'];
        $force_generate_id = $key_put['#db_force_generate_id'];

        if ($key_provided) {
          if ($key_already_exists) {
            if ($update_existing) {
              // Case 1
              if (!$force_generate_id) {
                $is_insert[$key_put['#db_table']] = FALSE;
              }
              // Case 1a: force ID generation for existing field
              else {
                $new_id = _importexportapi_db_put_sequences($key_field, $into, $is_insert, $value, $placeholder_map);

                $value_old = $value;
                $value = $new_id;
              }
            }
            else {
              if ($generate_id) {
                // Case 2
                $new_id = _importexportapi_db_put_sequences($key_field, $into, $is_insert, $value, $placeholder_map);

                $value_old = $value;
                $value = $new_id;
              }
              else {
                if (!empty($key_field['#key'])) {
                  // Case 3
                  $do_query = FALSE;

                  // Don't bother processing any more fields - we're not going to
                  // execute the query at all.
                  break;
                }
              }
            }
          }
          else {
            if ($force_generate_id) {
              // Case 4a: force ID generation for new field
              $new_id = _importexportapi_db_put_sequences($key_field, $into, $is_insert, $value, $placeholder_map);

              $value_old = $value;
              $value = $new_id;
            }
          }
        }
        else {
          if ($generate_id) {
            // Case 5
            $new_id = _importexportapi_db_put_sequences($key_field, $into, $is_insert, $value, $placeholder_map);

            $value_old = $value;
            $value = $new_id;
          }
          else {
            if (!empty($key_field['#key'])) {
              // Case 6
              $do_query = FALSE;

              // Don't bother processing any more fields - we're not going to
              // execute the query at all.
              break;
            }
          }
        }

        importexportapi_db_put_map_set($put_map, $key_field, $data, $value);
        $data[$key_index]['#value'] = $value;
        $data[$key_index]['#db_value_old'] = $value_old;
      }

      if (!empty($data[$key_index]['#key']) || !empty($data[$key_index]['#unique'])) {
        // Add this key (or unique field) to the child reference mapping, for
        // child fields to use in deeper recursions.
        $child_ref_array = array_merge($data[$key_index]['#parents'], array($key_index));
        $child_ref_index = implode('.', $child_ref_array);
        $child_ref_map[$child_ref_index] = $value;
      }

      if (!empty($key_field['#alt_key_for']) && isset($key_field['#value']) && $key_field['#value'] !== '') {
        $alt_key_index = $key_field['#alt_key_for'];

        if (isset($data[$alt_key_index]) && isset($data[$alt_key_index]['#value']) && $data[$alt_key_index]['#value'] !== '' && empty($data[$alt_key_index]['#db_uses_sequences'])) {
          $alt_key_field = $data[$alt_key_index];
          $alt_key_put = $alt_key_field['#put'];
          $alt_key_db_field = isset($alt_key_put['#db_reference_field_unaliased']) ? $alt_key_put['#db_reference_field_unaliased'] : $alt_key_put['#db_field_unaliased'];

          // Alternate key fields need to be updated in the database
          // immediately, otherwise duplicate key errors can occur.
          $sql = 'SELECT '. $key_put['#db_field_unaliased'] .' FROM {'. $into .'} WHERE '. $alt_key_db_field .' = '. $placeholder_map[$alt_key_field['#type']];
          $result = db_fetch_object(db_query($sql, $alt_key_field['#value']));

          if (!empty($result) && (!isset($result->$alt_key_db_field) || $result->$alt_key_db_field === '')) {
            $sql = 'UPDATE '. $into .' SET '. $key_put['#db_field_unaliased'] .' = '. $placeholder_map[$key_field['#type']] .' WHERE '. $alt_key_db_field .' = '. $placeholder_map[$alt_key_field['#type']];
            db_query($sql, $key_field['#value'], $alt_key_field['#value']);
          }
        }
      }

      if (isset($data[$key_index]['#reference_entity']) && (!isset($data[$key_index]['#reference_parent']) || $data[$key_index]['#reference_parent'])) {
        $child_ref_array = array_merge(array($data[$key_index]['#reference_entity']), $data[$key_index]['#reference_field']);
        $child_ref_index = implode('.', $child_ref_array);

        // Child reference fields automatically inherit the keys of the fields
        // that they reference, through the child reference mapping array.
        if (isset($child_ref_map[$child_ref_index])) {
          $data[$key_index]['#value'] = $child_ref_map[$child_ref_index];
          $data[$key_index]['#put']['#db_inherited_value'] = TRUE;
        }
      }
    }

    $data['#put']['#db_is_insert'] = $is_insert;
  }

  $data['#put']['#db_do_query'] = $do_query;

  if ($do_query) {
    foreach ($data as $data_index => $data_fields) {
      if ($data_fields['#type'] == 'array' && !empty($data_fields['#value'])) {
        foreach (element_children($data_fields['#value']) as $child_index) {
          // Recurse down through all child fields.
          _importexportapi_db_put_generate_key_ids($data[$data_index]['#value'][$child_index], $put_map, $placeholder_map, $child_ref_map);
        }
      }
    }
  }
}

/**
 * Sets the put map values for the specified field and all of its alternate
 * keys.
 *
 *
 * @param &$put_map
 *   A nested array consisting of entity names, field names, key names, and
 *   values.
 * @param $field
 *   The field for which to set put map values.
 * @param $parent
 *   The parent of the field.
 * @param $value_new
 *   The new value to record in the put map.
 */
function importexportapi_db_put_map_set(&$put_map, $field, $parent, $value_new) {
  $entity_index = array_shift($field['#parents']);
  $field_path = implode('.', array_merge($field['#parents'], array($field['#id'])));

  $field_array = importexportapi_get_def(array_merge(array_merge(array($entity_index), $field['#parents']), array($field['#id'])));
  if (!empty($field_array['#abstract_parent'])) {
    $entity_index = $field_array['#abstract_parent'];
  }

  // Set the put map value for the field itself, if one is available.
  if (isset($field['#value']) && $field['#value'] != $value_new) {
    $value_old = $field['#value'];
    $put_map[$entity_index][$field_path]['#key'][$value_old] = $value_new;
  }

  // Set the put map values for all available alternate key values.
  foreach (element_children($parent) as $alt_key_id) {
    $alt_key_field = $parent[$alt_key_id];

    if (isset($alt_key_field['#alt_key_for']) && $alt_key_field['#alt_key_for'] == $field['#id'] && isset($alt_key_field['#value']) && $alt_key_field['#value'] !== '') {
      $value_old = $alt_key_field['#value'];
      $put_map[$entity_index][$field_path][$alt_key_id][$value_old] = $value_new;
    }
  }
}

/**
 * Gets the first available put map value for the specified field. The put map
 * value for the field itself is returned if available; otherwise, the first
 * available alternate key value in the put map is returned.
 *
 * @param $put_map
 *   A nested array consisting of entity names, field names, key names, and
 *   values.
 * @param $field
 *   The field for which to get put values.
 * @param $value_old
 *   The old value to find in the put map.
 *
 * @return
 *   The first available put map value, or NULL if no value can be found.
 */
function importexportapi_db_put_map_get($put_map, $field, $value_old) {
  $reference_entity = $field['reference_entity'];
  $reference_path = implode('.', $field['reference_field']);

  $reference_field = importexportapi_get_def(array_merge(array($field['reference_entity']), $field['reference_field']));
  if (!empty($reference_field['#abstract_parent'])) {
    $reference_entity = $reference_field['#abstract_parent'];
  }

  // Return the put map value of the field itself if available.
  if (isset($value_old) && $value_old !== '' && isset($put_map[$reference_entity][$reference_path]['#key'][$value_old]) && $put_map[$reference_entity][$reference_path]['#key'][$value_old] !== '') {
    return $put_map[$reference_entity][$reference_path]['#key'][$value_old];
  }

  $parent = $field['parent_data'];

  // Otherwise, return the first available alternate key put map value.
  foreach (element_children($parent) as $alt_key_id) {
    $alt_key_field = $parent[$alt_key_id];

    if (isset($alt_key_field['#alt_key_for']) && $alt_key_field['#alt_key_for'] == $field['id'] && isset($alt_key_field['#value']) && $alt_key_field['#value'] !== '') {
      $put_map_index = $alt_key_id;
      if (!empty($alt_key_field['#alt_key_rename'])) {
        $put_map_index = end($alt_key_field['#reference_field']);
        $put_map_index = str_replace($alt_key_field['#alt_key_rename'] .'_', '', $put_map_index);
      }

      if (isset($put_map[$reference_entity][$reference_path][$put_map_index][$alt_key_field['#value']]) && $put_map[$reference_entity][$reference_path][$put_map_index][$alt_key_field['#value']] !== '') {
        $value_old = $alt_key_field['#value'];
        return $put_map[$reference_entity][$reference_path][$put_map_index][$value_old];
      }
    }
  }

  return NULL;
}

/**
 * Returns the old-value to new-value mappings of any fields in the specified
 * 'put' operation, that were saved to their 'put' source with a different
 * value from that returned by their 'get' engine.
 *
 * @param $put_id
 *   The logical grouping ID of the 'put' operation in question.
 * @param $refresh
 *   Whether or not to reset the internal cache of this function (defaults to
 *   FALSE).
 *
 * @return
 *   A nested array consisting of entity names, field names, key names, and
 *   values.
 */
function _importexportapi_db_put_maps_load($put_id, $refresh = FALSE) {
  static $put_map;

  if (!isset($put_map)) {
    $put_map = array();
  }

  if (!isset($put_map[$put_id]) || $refresh) {
    $put_map[$put_id] = array();
    $query = db_query('SELECT * FROM {importexportapi_db_put_map} WHERE put_id = %d', $put_id);

    while ($result = db_fetch_object($query)) {
      if (!isset($put_map[$put_id][$result->entity])) {
        $put_map[$put_id][$result->entity] = array();
      }

      $put_map[$put_id][$result->entity][$result->field]['#key'][$result->value_old] = $result->value_new;
    }

    $query = db_query('SELECT * FROM {importexportapi_db_put_map_alt} WHERE put_id = %d', $put_id);

    while ($result = db_fetch_object($query)) {
      if (!isset($put_map[$put_id][$result->entity])) {
        $put_map[$put_id][$result->entity] = array();
      }

      $put_map[$put_id][$result->entity][$result->field][$result->alt_field][$result->value_alt] = $result->value_key;
    }
  }

  return $put_map[$put_id];
}

/**
 * Saves the specified old-value to new-value mappings of fields to the
 * database, under the given 'put' ID.
 *
 * @param $put_id
 *   The logical grouping ID of the 'put' operation in question.
 * @param $put_map
 *   A nested array consisting of entity names, field names, key names, and
 *   values.
 */
function _importexportapi_db_put_maps_save($put_id, $put_map) {
  foreach ($put_map as $entity_index => $fields) {
    foreach ($fields as $field_path => $keys) {
      foreach ($keys as $key_field => $values) {
        foreach ($values as $value_old => $value_new) {
          if ($key_field == '#key') {
            $args = array($put_id, $entity_index, $field_path, $value_old, $value_new);
            $into = '{importexportapi_db_put_map}';
            $sql = '';

            $check_sql = 'SELECT * FROM {'. $into ."} WHERE put_id = %d AND entity = '%s' AND field = '%s' AND value_old = %d";
            $result = db_fetch_object(db_query($check_sql, $put_id, $entity_index, $field_path, $value_old));
            $is_insert = empty($result);

            if ($is_insert) {
              $sql .= 'INSERT INTO '. $into ." (put_id, entity, field, value_old, value_new) VALUES (%d, '%s', '%s', %d, %d)";
            }
            else {
              $sql .= 'UPDATE '. $into ." SET put_id = %d, entity = '%s', field = '%s', value_old = %d, value_new = %d WHERE put_map_id = %d";
              $args[] = $result->put_map_id;
            }

            db_query($sql, $args);
          }
          else {
            $args = array($put_id, $entity_index, $field_path, $key_field, $value_old, $value_new);
            $into = '{importexportapi_db_put_map_alt}';
            $sql = '';

            $check_sql = 'SELECT * FROM {'. $into ."} WHERE put_id = %d AND entity = '%s' AND field = '%s' AND alt_field = '%s' AND value_alt = '%s'";
            $result = db_fetch_object(db_query($check_sql, $put_id, $entity_index, $field_path, $key_field, $value_old));
            $is_insert = empty($result);

            if ($is_insert) {
              $sql .= 'INSERT INTO '. $into ." (put_id, entity, field, alt_field, value_alt, value_key) VALUES (%d, '%s', '%s', '%s', '%s', '%d')";
            }
            else {
              $sql .= 'UPDATE '. $into ." SET put_id = %d, entity = '%s', field = '%s', alt_field = '%s', value_alt = '%s', value_key = %d WHERE put_map_alt_id = %d";
              $args[] = $result->put_map_id;
            }

            db_query($sql, $args);
          }
        }
      }
    }
  }
}
