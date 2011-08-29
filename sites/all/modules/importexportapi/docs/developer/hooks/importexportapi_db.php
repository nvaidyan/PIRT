<?php

/**
 * @file
 * Implements hook documentation on behalf of the 'db' engines of the Import /
 * Export API module.
 */

/**
 * @addtogroup hooks
 * @{
 */


/**
 * Define the aliases of database tables that are used in data definitions.
 *
 * @return
 *   An array of database tables and their aliases, where each key is a database
 *   table name, and where each value is a database table alias.
 */
function hook_db_def_tables() {
  $tables = array();

  $tables['node'] = 'n';
  $tables['node_revisions'] = 'nr';
  $tables['node_access'] = 'na';

  return $tables;
}


/**
 * @} End of "addtogroup hooks".
 */
