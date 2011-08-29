<?php
// $Id$


/**
 * @file
 * Page callbacks for ASU Webauth module
 */


/**
 * Menu callback.
 *
 * Negotiate whether to redirect to webauth for login.
 */
function asu_webauth_attempt() {
  $authenticator = isset($_GET['authenticator']) ? $_GET['authenticator'] : NULL;
  $destination = isset($_GET['webauthGoto']) ? $_GET['webauthGoto'] : NULL;
  $destination = urldecode($destination);

  if ($authenticator) {

    if (ASU_WEBAUTH_DEBUG) {
      drupal_set_message(t('ASU Webauth: Attempting to login with authenticator: %code', array('%code' => $authenticator)));
    }

    global $user;
    $ip = $_SERVER['REMOTE_ADDR'];
    $result = asu_webauth_verify($authenticator, $ip);
    $user_with_domain = $result;
    if (!$result) {
      if (isset($_SESSION['asu_webauth_goto']) && $_SESSION['asu_webauth_goto'] == $destination) {
        return drupal_access_denied();
      }
      else {
        asu_webauth_goto($destination);
      }
    }

    // Store the authenticator
    $_SESSION['asu_webauth_authenticator'] = $authenticator;

    // Negotiate the username of the returned account
    $username = asu_webauth_process_verify_result($result);

    if (ASU_WEBAUTH_DEBUG) {
      drupal_set_message(t('ASU Webauth: Processed verify result. Username: %n', array('%n' => $username)));
    }

    // Check if user is blocked or the username is blocked
    if (user_is_blocked($username)) {
      drupal_set_message(t('The username %username has been blocked.', array('%username' => $username)), 'error');
      return '';
    }
    if (drupal_is_denied('user', $username)) {
      drupal_set_message(t('The username %username is unavailable.', array('%username' => $username)), 'error');
      return '';
    }

    // Setup some variables we'll need for the rest of this
    $asu_webauth_auto_register = variable_get('asu_webauth_auto_register', FALSE);
    $asu_webauth_roles = variable_get('asu_webauth_auto_assigned_role', array(DRUPAL_AUTHENTICATED_RID));
    $all_roles = user_roles();
    $asu_webauth_roles = array();
    foreach ($all_roles as $key => $value) {
      if (array_key_exists($key, variable_get('asu_webauth_auto_assigned_role', array(DRUPAL_AUTHENTICATED_RID)))) {
        $asu_webauth_roles[$key] = $key;
      }
    }

    // Does a user exist with the given username?
    $account = user_load(array('name' => $username));

    // If account exists, set the current user to be it
    if ($account && $account->uid) {
      $user = $account;
    }

    // If the account doesn't exist, make one (if set to do so).
    if (!$account || !$account->uid) {
      if ($asu_webauth_auto_register) {
        $user_info = array(
          'name' => $username,
          'pass' => user_password(),
          'mail' => drupal_strtolower($user_with_domain),
          'init' => db_escape_string($username),
          'status' => 1,
          'roles' => $asu_webauth_roles,
          'access' => time(),
        );
        $new_user = user_save(NULL, $user_info);
        if (!$new_user) {
          drupal_set_message(t('Error saving user account.'), 'error');
          return '';
        }
        watchdog('user', 'New user: %n (ASU Webauth)', array('%n' => $user->name), WATCHDOG_NOTICE, l(t('edit user'), 'user/'. $user->uid .'/edit'));
        $user = $new_user;
      }
    }

    // Update the user's roles
    if ($user && $user->uid) {
      $roles = $user->roles;
      foreach ($asu_webauth_roles as $role) {
        $roles[$role] = $role;
      }
      $user_update = array(
        'roles' => $roles,
      );
      $user = user_save($user, $user_update);

      // Invoke the login
      $_SESSION['asu_webauth_noauth'] = FALSE;
      module_invoke_all('user', 'login', array(), $user);
      watchdog('user', 'Session opened for %n (ASU Webauth)', array('%n' => $user->name));
    }

    // Redirect to the saved page
    asu_webauth_goto($destination);
  }
  else {
    asu_webauth_redirect();
  }
}


/**
 * Menu callback.
 *
 * A blind auth attempt was made, and user didn't have an authenticator.
 * Just take them back to where they came from.
 */
function asu_webauth_noauth() {
  $_SESSION['asu_webauth_noauth'] = TRUE;
  $_SESSION['asu_webauth_attempting_to_auth'] = FALSE;

  if (ASU_WEBAUTH_DEBUG) {
    drupal_set_message(t('ASU Webauth: Discrete authentication attempt made, but user is not authenticated.'));
  }

  $destination = isset($_GET['webauthGoto']) ? $_GET['webauthGoto'] : NULL;
  asu_webauth_goto($destination);
}


/**
 * Menu callback.
 *
 * If set to do so, override the user password reset link and redirect to ASU's password reset page.
 */
function asu_webauth_user_password() {
  $destination = variable_get('asu_webauth_changePasswordURL', ASU_WEBAUTH_CHANGE_PW_URL);
  if ($destination) {
    drupal_goto($destination);
  }
  else {
    module_load_include('inc', 'user', 'user.pages');
    return drupal_get_form('user_pass');
  }
  return '';
}


/**
 * Menu callback.
 *
 * If set to do so, override the user register link and redirect to ASU's account registration page.
 */
function asu_webauth_user_register() {
  $destination = variable_get('asu_webauth_registerURL', ASU_WEBAUTH_REGISTER_URL);
  if ($destination) {
    drupal_goto($destination);
  }
  else {
    module_load_include('inc', 'user', 'user.pages');
    return drupal_get_form('user_register');
  }
  return '';
}


/**
 * Menu callback.
 *
 * Quick user-add form for ASURITE-based accounts.
 *
 * @todo Add notification options.
 */
function asu_webauth_add_user_form() {
  $form = array();
  $form['username'] = array(
    '#type' => 'value',
    '#value' => array()
  );
  $form['asurite'] = array(
    '#type' => 'textfield',
    '#title' => t('ASURITE ID'),
    '#description' => t('The ASURITE ID of the user to be added.'),
  );
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['advanced']['asurites'] = array(
    '#type' => 'textarea',
    '#title' => t('Add multiple ASURITE IDs'),
    '#description' => t('Enter one or more ASURITE IDs, one per line and/or comma-delimited.'),
  );
  /*
  $form['notify'] = array(
    '#type' => 'checkbox',
    '#title' => t('Notify user of new account'),
  );
  */
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create new account'),
  );
  return $form;
}


/**
 * Form validation callback.
 */
function asu_webauth_add_user_form_validate($form, &$form_state) {
  $asurites = array();
  $lines = explode("\n", $form_state['values']['asurites']);
  foreach ($lines as $line) {
    $items = explode(',', $line);
    foreach ($items as $item) {
      $id = trim($item);
      if ($id) {
        $asurites[$id] = $id;
      }
    }
  }
  $asurite = trim($form_state['values']['asurite']);
  if ($asurite) {
    $asurites[$asurite] = $asurite;
  }

  // Does the user already exist?
  foreach ($asurites as $asurite) {
    $username = $asurite;
    if (stristr($username, '@asu.edu')) {
      form_set_error('asurite', t('ASURITE ID should not have <em>@asu.edu</em> appended.'));
      return;
    }
    if (variable_get('asu_webauth_use_domain', FALSE)) {
      $username .= '@asu.edu';
    }
    $username = drupal_strtolower($username);
    $test_user = user_load(array('name' => $username));
    if ($test_user && $test_user->uid) {
      form_set_error('asurite', t('An account with the name %username already exists', array('%username' => $username)));
      return;
    }
    // Check LDAP, if available
    if (function_exists('ldap_search')) {
      module_load_include('inc', 'asu_webauth', 'includes/ldap');
      $result = asu_webauth_ldap_user_by_attribute('asuriteid', $asurite);
      if (!$result || !$result['asuriteid']) {
        drupal_set_message(t('Warning: %asurite is an invalid ASURITE ID, but the account has been created locally.', array('%asurite' => $asurite)), 'warning');
      }
    }
    $form_state['values']['usernames'][$asurite] = $username;
  }
}


/**
 * Form submit handler.
 */
function asu_webauth_add_user_form_submit($form, &$form_state) {
  foreach ($form_state['values']['usernames'] as $asurite => $username) {
    $email = $asurite .'@asu.edu';
    $asu_webauth_roles = variable_get('asu_webauth_auto_assigned_role', array(DRUPAL_AUTHENTICATED_RID));

    $user_info = array(
      'name' => $username,
      'pass' => user_password(),
      'mail' => $email,
      'init' => db_escape_string($username),
      'status' => 1,
      'roles' => $asu_webauth_roles,
    );
    $new_user = user_save(NULL, $user_info);

    if (!$new_user) {
      drupal_set_message(t('Error saving user account.'), 'error');
      return '';
    }
    drupal_set_message(t('Added user %username', array('%username' => $username)));
    watchdog('user', 'New user: %n (ASU Webauth)', array('%n' => $user->name), WATCHDOG_NOTICE, l(t('edit user'), 'user/'. $user->uid .'/edit'));
  }
}

