<?php

/**
 * @file
 * Implements data definitions on behalf of aggregator.module.
 */

/**
 * Implementation of hook_def().
 */
function aggregator_def() {
  $defs = array();

  $def = array(
    '#type' => 'entity',
    '#title' => t('Aggregator category'),
    '#xml_plural' => 'aggregator-categories',
    '#csv_plural' => 'aggregator-categories'
  );

  $def['cid'] = array(
    '#type' => 'int',
    '#title' => t('Aggregator category ID'),
    '#key' => TRUE,
    '#db_uses_sequences' => TRUE
  );
  $def['category_title'] = array(
    '#title' => t('Category title'),
    '#db_field_unaliased' => 'title',
    '#alt_key_for' => 'cid'
  );
  $def['category_description'] = array(
    '#title' => t('Category description'),
    '#db_field_unaliased' => 'description'
  );
  $def['num_items_in_block'] = array(
    '#type' => 'int',
    '#title' => t('Number of items in block'),
    '#db_field_unaliased' => 'block'
  );

  $defs['aggregator_category'] = $def;

  $def = array(
    '#type' => 'entity',
    '#title' => t('Aggregator feed'),
    '#xml_plural' => 'aggregator-feeds',
    '#csv_plural' => 'aggregator-feeds'
  );

  $def['fid'] = array(
    '#type' => 'int',
    '#title' => t('Aggregator feed ID'),
    '#key' => TRUE,
    '#db_uses_sequences' => TRUE
  );
  $def['feed_title'] = array(
    '#title' => t('Feed title'),
    '#db_field_unaliased' => 'title',
    '#alt_key_for' => 'fid'
  );
  $def['feed_source_url'] = array(
    '#title' => t('Feed source URL'),
    '#db_field_unaliased' => 'url'
  );
  $def['refresh_interval'] = array(
    '#type' => 'int',
    '#title' => t('Refresh interval'),
    '#db_field_unaliased' => 'refresh'
  );
  $def['last_checked_date'] = array(
    '#type' => 'datetime',
    '#title' => t('Last checked date'),
    '#db_field_unaliased' => 'checked'
  );
  $def['feed_syndicated_url'] = array(
    '#title' => t('Feed syndicated URL'),
    '#db_field_unaliased' => 'link'
  );
  $def['feed_description'] = array(
    '#title' => t('Feed description'),
    '#db_field_unaliased' => 'description'
  );
  $def['image'] = array(
    '#title' => t('Feed image')
  );
  $def['etag'] = array(
    '#title' => t('E-tag')
  );
  $def['last_modified_date'] = array(
    '#type' => 'datetime',
    '#title' => t('Last modified date'),
    '#db_field_unaliased' => 'modified'
  );
  $def['num_items_in_block'] = array(
    '#type' => 'int',
    '#title' => t('Number of items in block'),
    '#db_field_unaliased' => 'block'
  );
  $def['feed_categories'] = array(
    '#type' => 'array',
    '#title' => t('Feed categories'),
    '#db_default_table' => 'aggregator_category_feed',
    '#xml_plural' => 'feed-categories',
    '#csv_plural' => 'feed-categories',
    '#xml_mapping' => 'feed-category'
  );
  $def['feed_categories']['fid'] = array(
    '#type' => 'int',
    '#title' => t('Aggregator feed ID'),
    '#reference_entity' => 'aggregator_feed'
  );
  $def['feed_categories']['cid'] = array(
    '#type' => 'int',
    '#title' => t('Aggregator category ID'),
    '#reference_entity' => 'aggregator_category',
    '#key_component' => TRUE
  );

  $defs['aggregator_feed'] = $def;

  $def = array(
    '#type' => 'entity',
    '#title' => t('Aggregator item'),
    '#xml_plural' => 'aggregator-items',
    '#csv_plural' => 'aggregator-items'
  );

  $def['iid'] = array(
    '#type' => 'int',
    '#title' => t('Aggregator item ID'),
    '#key' => TRUE,
    '#db_uses_sequences' => TRUE
  );
  $def['fid'] = array(
    '#type' => 'int',
    '#title' => t('Aggregator feed ID'),
    '#reference_entity' => 'aggregator_feed'
  );
  $def['item_title'] = array(
    '#title' => t('Item title'),
    '#db_field_unaliased' => 'title',
    '#alt_key_for' => 'iid'
  );
  $def['source_url'] = array(
    '#title' => t('Source URL'),
    '#db_field_unaliased' => 'link'
  );
  $def['author'] = array(
    '#title' => t('Author')
  );
  $def['item_description'] = array(
    '#title' => t('Item description'),
    '#db_field_unaliased' => 'description'
  );
  $def['created'] = array(
    '#type' => 'datetime',
    '#title' => t('Creation date'),
    '#db_field_unaliased' => 'timestamp'
  );
  $def['guid'] = array(
    '#title' => t('GUID')
  );
  $def['item_categories'] = array(
    '#type' => 'array',
    '#title' => t('Item categories'),
    '#db_default_table' => 'aggregator_category_item',
    '#xml_plural' => 'item-categories',
    '#csv_plural' => 'item-categories',
    '#xml_mapping' => 'item-category'
  );
  $def['item_categories']['iid'] = array(
    '#type' => 'int',
    '#title' => t('Aggregator item ID'),
    '#reference_entity' => 'aggregator_item'
  );
  $def['item_categories']['cid'] = array(
    '#type' => 'int',
    '#title' => t('Aggregator category ID'),
    '#reference_entity' => 'aggregator_category',
    '#key_component' => TRUE
  );

  $defs['aggregator_item'] = $def;

  return $defs;
}

/**
 * Implementation of hook_db_def_tables().
 */
function aggregator_db_def_tables() {
  $tables = array();

  $tables['aggregator_category'] = 'ac';
  $tables['aggregator_category_feed'] = 'acf';
  $tables['aggregator_category_item'] = 'aci';
  $tables['aggregator_feed'] = 'af';
  $tables['aggregator_item'] = 'ai';

  return $tables;
}
