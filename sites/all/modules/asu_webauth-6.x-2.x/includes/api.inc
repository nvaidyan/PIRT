<?php
// $Id$


/**
 * @file
 * General API methods for ASU Webauth module
 */


/**
 * Generate the callapp string for this host.
 */
function asu_webauth_generate_callapp() {
  global $base_url;
  return $base_url .'/asuwebauth';
}


/**
 * Send user to the saved destination.
 */
function asu_webauth_goto($destination = NULL) {
  global $base_path;
  if (!$destination) {
    $destination = $_SESSION['asu_webauth_goto'];
  }
  $pieces = parse_url($destination);
  $query = $_GET;
  unset($_SESSION['asu_webauth_goto']);
  unset($query['webauthGoto']);
  unset($query['authenticator']);
  if ($pieces['scheme']) {
    $url = $pieces['scheme'] .'://'. $pieces['host'];
    if ($pieces['port']) {
      $url .= ':'. $pieces['port'];
    }
    $url .= $pieces['path'];
  }
  elseif ($pieces['path']) {
    $path = trim($pieces['path'], '/');
    if (trim($base_path, '/')) {
      $path = str_ireplace($base_path, '', $pieces['path']);
    }
    unset($query['q']);
    $url = $path;
  }
  $goto = url($url, array('absolute' => TRUE, 'query' => $query));
  header('Location: '. $goto);
}


/**
 * Check that the user still has a valid webauth session (authenticator).
 */
function asu_webauth_verify_or_logout() {
  if (ASU_WEBAUTH_DEBUG) {
    drupal_set_message(t('ASU Webauth: Verifying user is still authenticated.'));
  }

  global $user;
  $authenticator = $_SESSION['asu_webauth_authenticator'];
  if ($authenticator && $user->uid) {
    $ip = $_SERVER['REMOTE_ADDR'];
    $result = asu_webauth_verify($authenticator, $ip);
    if (!$result) {
      if (ASU_WEBAUTH_DEBUG) {
        drupal_set_message(t('ASU Webauth: User is no longer authenticated, logging out'));
      }
      asu_webauth_logout();
    }
  }
}



/**
 * Redirect to webauth login page.
 *
 * @param $force
 *   Force the user to login if they aren't already logged in.
 */
function asu_webauth_redirect($force = FALSE) {
  if (ASU_WEBAUTH_DEBUG) {
    drupal_set_message(t('ASU Webauth: Redirect to weblogin'));
  }

  // Always say that we're discretely attempting
  $_SESSION['asu_webauth_tried_discrete'] = TRUE;
  // If we're forcing to attempt a login
  if ($force) {
    $_SESSION['asu_webauth_noauth'] = FALSE;
    $_SESSION['asu_webauth_tried_forced'] = TRUE;
  }
  $_SESSION['asu_webauth_attempting_to_auth'] = TRUE;

  $page = asu_webauth_save_page();
  $callapp = asu_webauth_generate_callapp();
  $goto = 'webauthGoto='. $page;

  $url = ASU_WEBAUTH_LOGIN_URL;
  $url .= '?callapp=' . urlencode($callapp .'?'. $goto);
  if (!$force) {
    $url .= '&onLoginURL='. urlencode($callapp .'/noauth?'. $goto);
  }

  // From drupal_goto() - not used here due to the way it alters the destination URL
  if (!defined('MAINTENANCE_MODE') || MAINTENANCE_MODE != 'update') {
    module_invoke_all('exit', $url);
  }
  session_write_close();
  header('Location: '. $url);
  exit();
}


/**
 * Helper function for processing the verify result into logical pieces.
 *
 * @param $result
 *   The verify result.
 */
function asu_webauth_process_verify_result($result) {
  if (variable_get('asu_webauth_use_domain', FALSE)) {
    return $result;
  }
  else {
    $parts = explode('@', $result);
    return $parts[0];
  }
}

/**
 * Saves the page that the user was on when login was requested or required.
 *
 * @param $path
 *   Set the path. Otherwise, uses logic to determine the referrer.
 */
function asu_webauth_save_page($path = NULL) {
  global $base_path;
  $page = '';
  if ($path) {
    $page = $base_path . $path;
  }
  elseif (!$_SESSION['asu_webauth_goto']) {
    if ($_GET['destination']) {
      $destination = urldecode($_GET['destination']);
      $pieces = parse_url($destination);
      if ($pieces['scheme']) {
        $page = $destination;
      }
      else {
        $page = $base_path . $destination;
      }
    }
    elseif (strstr($_GET['q'], 'asuwebauth')) {
      // We're destined to the ASU Webauth login page, so set the page to the referrer.
      $pieces = parse_url($_SERVER['HTTP_REFERER']);
      $page = $pieces['path'];
    }
    else {
      // We're destined for another page, so save the group.
      $page = $base_path . $_GET['q'];
    }
  }
  $_SESSION['asu_webauth_goto'] = $page;
  return urlencode($page);
}


/**
 * Clean a URL so it can be compared effectively against another URL.
 */
function asu_webauth_clean_url($url) {
  $parsed = parse_url(drupal_strtolower($url));
  $url = $parsed['scheme'] .'://'. $parsed['host'];
  if (isset($parsed['port'])) {
    $url .= ':'. $parsed['port'];
  }
  if (isset($parsed['path'])) {
    $url .= $parsed['path'];
  }
  return $url;
}


/**
 * Verify the webauth authenticator.
 */
function asu_webauth_verify($authenticator, $ip, $requirements_check = FALSE) {
  $host = ASU_WEBAUTH_HOST;
  $port = ASU_WEBAUTH_PORT;
  $timeout = ASU_WEBAUTH_TIMEOUT;;

  // Create the socket
  if ($socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP)) {
  }
  else {
    $error = t('Unable open socket connection to webauth.asu.edu');
    drupal_set_message($error, 'error');
    watchdog('asu webauth', $error, array(), WATCHDOG_ERROR);
    return;
  }

  $time = time();

  // Connect to $host until successful or $timeout
  while (!@socket_connect($socket, $host, $port)) {
    $err = socket_last_error($socket);
    if ($err == 115 || $err == 114) {
      if ((time() - $time) >= $timeout) {
        socket_close($socket);
        $error = t('Connection to webauth.asu.edu timed out');
      }
      sleep(1);
      continue;
    }
    else {
      $err_num = $err;
      $err_msg = socket_strerror($err);
      $error = t('Error with socket connection to webauth.asu.edu (%num: %msg)', array('%num' => $err_num, '%msg' => $err_msg));
    }
    drupal_set_message($error, 'error');
    watchdog('asu webauth', $error, array(), WATCHDOG_ERROR);
    return;
  }

  // Send WebAuth authenticator token and client IP
  $msg = $authenticator . ':' . $ip . ":N: : :E:\n";
  $length = socket_write($socket, $msg);
  $verify_result = trim(socket_read($socket, $length));

  if (!$verify_result) {
    if (ASU_WEBAUTH_DEBUG) {
      drupal_set_message(t('ASU Webauth: Verify called, but no result or empty returned'));
    }
    return FALSE;
  }

  // Close the socket
  socket_close($socket);

  if (ASU_WEBAUTH_DEBUG) {
    drupal_set_message(t('ASU Webauth: Verify result: %r', array('%r' => $verify_result)));
  }

  $explode = explode(':', $verify_result);

  // Check the status code
  $code = array_shift($explode);
  $message = array_shift($explode);
  $error = NULL;

  // Handle the returned data
  switch ($code) {

    // Successfully verified
    case 0:
      // Make sure the callapp matches
      $callapp_correct = asu_webauth_clean_url(asu_webauth_generate_callapp());
      $callapp_reported = asu_webauth_clean_url(urldecode(array_pop($explode)));
      if ($callapp_reported != $callapp_correct) {
        $error = t('Callapp URL mismatch. This can occur if you are attempting to view a page that you do not have permission to access, but which requires authentication.');
      }
      elseif (!strstr($message, '@')) {
        $error = t('Malformed verify result');
      }
      else {
        // Verified, return username
        return $message;
      }
      break;

    // Expired authenticator, do nothing
    // asu_webauth_init() will handle logging out.
    case 1:
      //asu_webauth_redirect();
      break;

    // Client or server host mismatch
    case 2:
    case 3:
      $error = t('Client / server mismatch');
      break;

    // Error on the verify server
    // @todo Send notification
    case 4:
    case 5:
    case 6:
      $error = t('@message (@code)', array('@code' => $code, '@message' => $message));
      watchdog('asu webauth', $error, array(), WATCHDOG_ERROR);
      break;

    // Invalid authenticator
    case 7:
      $error = t('Invalid webauth authenticator');
      break;
  }

  if ($requirements_check) {
    return $code;
  }
  if ($error) {
    drupal_set_message(t('Could not authenticate. ASU Webauth error: %error', array('%error' => $error)), 'error');
  }
  return FALSE;
}


/**
 * Menu callback.
 *
 * Logout the user.
 */
function asu_webauth_logout() {
  global $base_url;
  asu_webauth_save_page();
  if (isset($_GET['destination'])) {
    $dest = url($_GET['destination'], array('absolute' => TRUE));
  }
  elseif (isset($_SERVER['HTTP_REFERER'])) {
    $dest = $_SERVER['HTTP_REFERER'];
  }
  else {
    $dest = $base_url .'/'. $_GET['q'];
  }

  session_destroy();
  module_invoke_all('user', 'logout', NULL, $user);
  $user = user_load(array('uid' => 0));

  $url = ASU_WEBAUTH_LOGIN_URL .'?onLogoutURL='. urlencode($dest);
  header('Location: '. $url);
  exit();
}


/**
 * Helper function to see if we need to force login.
 */
function asu_webauth_check_force_login() {
  $check = variable_get('asu_webauth_pages_check', 'none');

  // Skip these paths no matter what
  $arg0 = arg(0);
  $arg1 = arg(1);
  $bad_paths = array(
    'asuwebauth',
    'asuwebauth-login',
    'asuwebauth-logout',
    'logout',
  );
  if (in_array($arg0, $bad_paths) || ($arg0 .'/'. $arg1 == 'user/login')) {
    return FALSE;
  }


  if ($check == 'none') {
    return FALSE;
  }
  if ($check == 'all') {
    if (ASU_WEBAUTH_DEBUG) {
      drupal_set_message(t('ASU Webauth: Redirect to Webauth required on all pages'));
    }
    return TRUE;
  }

  $pages = variable_get('asu_webauth_pages', '');
  if ($pages) {
    $match = path_match($pages);

    // Alter the default
    if ($match) {
      if ($check == 'all except') {
        return FALSE;
      }
      if ($check == 'specific') {
        if (ASU_WEBAUTH_DEBUG) {
          drupal_set_message(t('ASU Webauth: Redirect to Webauth required on this page'));
        }
        return TRUE;
      }
    }
    else {
      if ($check == 'all except') {
        return TRUE;
      }
      if ($check == 'specific') {
        return FALSE;
      }
    }
  }

  return TRUE;
}

/**
 * Helper function to see if currnnt path matches any of the provided patterns in
 * $pages.
 */
function path_match($pages) {
  $path_match = 0;
  // This common page matching logic used throughout drupal.
  if ($pages) {
    $path = drupal_get_path_alias($_GET['q']);
    $regexp = '/^('. preg_replace(array('/(\r\n?|\n)/', '/\\\\\*/', '/(^|\|)\\\\<front\\
\\>($|\|)/'), array('|', '.*', '\1'. variable_get('site_frontpage', 'node') .'\2'), preg_quote($pages, '/')) .')$/';
    $path_match = preg_match($regexp, $path);
  }

  return $path_match;
}
