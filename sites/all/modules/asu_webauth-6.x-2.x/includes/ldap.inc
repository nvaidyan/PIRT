<?php
// $Id$


/**
 * @file
 * Optional LDAP-related functionality for the ASU Webauth module.
 */

define('ASU_WEBAUTH_LDAP_BASEDN', 'ou=people,o=asu.edu');
define('ASU_WEBAUTH_LDAP_HOST', 'ds.asu.edu');
define('ASU_WEBAUTH_LDAP_DN', '');
define('ASU_WEBAUTH_LDAP_PASS', '');
define('ASU_WEBAUTH_LDAP_PORT', 389);


/**
 * Helper function for generating the display name.
 */
function asu_webauth_ldap_generate_name($account) {
  $username = $account->name;
  if (variable_get('asu_webauth_use_domain', FALSE)) {
    $username = str_ireplace('@asu.edu', '', $username);
  }
  $ldap_data = asu_webauth_ldap_user_by_attribute('asuriteid', $username);
  if ($ldap_data) {
    $name = $ldap_data['cn'];
  }
  else {
    $name = $account->name;
  }
  return $name;
}

/**
 * Override of theme_username().
 *
 * @see theme_username()
 */
function asu_webauth_ldap_username($object) {
  if ($object->uid && $object->name) {
    $name = asu_webauth_ldap_generate_name($object);

    // Shorten the name when it is too long or it will break many tables.
    if (drupal_strlen($name) > 20) {
      $name = drupal_substr($name, 0, 15) .'...';
    }

    if (user_access('access user profiles')) {
      $output = l($name, 'user/'. $object->uid, array('attributes' => array('title' => t('View user profile.'))));
    }
    else {
      $output = check_plain($name);
    }
  }
  elseif ($object->name) {
    // Sometimes modules display content composed by people who are
    // not registered members of the site (e.g. mailing list or news
    // aggregator modules). This clause enables modules to display
    // the true author of the content.
    if (!empty($object->homepage)) {
      $output = l($object->name, $object->homepage, array('attributes' => array('rel' => 'nofollow')));
    }
    else {
      $output = check_plain($object->name);
    }

    $output .= ' ('. t('not verified') .')';
  }
  else {
    $output = check_plain(variable_get('anonymous', t('Anonymous')));
  }

  return $output;
}


/**
 * Search LDAP based on an attribute / value pair. This is not intended to be used for
 * wildcard or partial match searches.
 *
 * @param $attribute
 *   i.e. asuriteid, affiliateid, etc.
 * @return
 *   A keyed array for one, and only one, user result from the LDAP search.
 */
function asu_webauth_ldap_user_by_attribute($attribute, $value) {
  $cid = 'ldap:'. $value;
  if ($cache = cache_get($cid, 'cache_asuwebauth')) {
    $data = $cache->data;
  }
  else {
    $ds = asu_webauth_ldap_connect();
    $sr = ldap_search($ds, ASU_WEBAUTH_LDAP_BASEDN, $attribute .'='. $value);
    $entries = ldap_get_entries($ds, $sr);
    asu_webauth_ldap_close($ds);
    $data = asu_webauth_ldap_filter_result_single($entries);
    cache_set($cid, $data, 'cache_asuwebauth');
  }
  return $data;
}


/**
 * Filter a list of LDAP results down to a single entry.
 */
function asu_webauth_ldap_filter_result_single($entries) {
  $result = array();
  if ($entries['count']) {
    foreach ($entries[0] as $key => $value) {
      if (!is_numeric($key)) {
        $result[$key] = $value[0];
      }
    }
  }
  return $result;
}


/**
 * Helper method to connect to LDAP.
 */
function asu_webauth_ldap_connect() {
  $ds = ldap_connect(ASU_WEBAUTH_LDAP_HOST);
  if ($ds) {
    $r = ldap_bind($ds, ASU_WEBAUTH_LDAP_DN, ASU_WEBAUTH_LDAP_PASS);
  }
  return $ds;
}


/**
 * Helper method to close LDAP connection.
 */
function asu_webauth_ldap_close($ds) {
  ldap_close($ds);
  return $ds;
}