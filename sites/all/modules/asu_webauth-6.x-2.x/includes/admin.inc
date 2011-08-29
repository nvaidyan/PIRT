<?php
// $Id$


/**
 * @file
 * Administrative settings for webauth module
 */


/**
 * Menu callback.
 *
 * Module administrative settings form.
 */
function asu_webauth_admin_settings() {
  $form = array();

  //------- General Settings -------
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
  );
  $form['general']['asu_webauth_verify_always'] = array(
    '#type' => 'checkbox',
    '#title' => t('Verify on every page view'),
    '#description' => t('<strong>Recommended:</strong> Whether or not to call verify on every page view for active Webauth users.'),
    '#default_value' => variable_get('asu_webauth_verify_always', TRUE),
  );
  $form['general']['asu_webauth_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable debug mode'),
    '#description' => t('Turn on debugging statements for ASU Webauth.'),
    '#default_value' => variable_get('asu_webauth_debug', FALSE),
  );

  //------- User Account Settings -------
  $form['user'] = array(
    '#type' => 'fieldset',
    '#title' => t('User account settings'),
  );
  $form['user']['asu_webauth_auto_register'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically register new users'),
    '#description' => t('Register any user that logs in via webauth. Useful for tracking who visits the site, but please note that creating a session for every webauth user can create unnecessary load on the server.'),
    '#default_value' => variable_get('asu_webauth_auto_register', FALSE),
  );
  $form['user']['asu_webauth_use_domain'] = array(
    '#type' => 'checkbox',
    '#title' => t('Append @ASU.EDU to all Webauth account usernames'),
    '#description' => t('If users login via Webauth, their account username should have @ASU.EDU appended to it, i.e. asurite@ASU.EDU. This is useful if your site needs to have both Webauth and non-Webauth accounts.'),
    '#default_value' => variable_get('asu_webauth_use_domain', FALSE),
  );
  $form['user']['asu_webauth_hide_email'] = array(
    '#type' => 'checkbox',
    '#title' => t('Users cannot change email address'),
    '#description' => t('Hide the email address field on the edit user form. Field will still be available to user administrators. All users will be registered with the email address asurite@asu.edu'),
    '#default_value' => variable_get('asu_webauth_hide_email', TRUE),
  );
  $form['user']['asu_webauth_hide_password'] = array(
    '#type' => 'checkbox',
    '#title' => t('Users cannot change local password'),
    '#description' => t('Hide the password field on the user edit form. Users will be directed to the ASU change password form. Field will still be available to user administrators.'),
    '#default_value' => variable_get('asu_webauth_hide_password', TRUE),
  );
  // Get the roles and unset "anonymous"
  $roles = user_roles();
  unset($roles[1]);
  $form['user']['asu_webauth_auto_assigned_role'] = array(
    '#type' => 'select',
    '#title' => t('Auto-assign users to the role(s)'),
    '#description' => t('This value can be used to establish a role automatically for all webauth users. This is useful if you need to assign different permissions to local users versus webauth users.'),
    '#options' => $roles,
    '#multiple' => TRUE,
    '#default_value' => variable_get('asu_webauth_auto_assigned_role', array(DRUPAL_AUTHENTICATED_RID)),
  );
  $form['user']['asu_webauth_show_ldap_name'] = array(
    '#type' => 'checkbox',
    '#title' => t('Render name as LDAP "common name" value'),
    '#description' => t('If selected, render the display name of the user as the LDAP value for "common name." The user account name will remain the ASURITE ID of the user, but when the name of the user is rendered, it will be rendered as the common name of the user. e.g. <em>johndoe</em> becomes <em>John Doe</em>.'),
    '#default_value' => variable_get('asu_webauth_show_ldap_name', FALSE),
  );
  
  //------- Redirection Settings -------
  $form['pages'] = array(
    '#type' => 'fieldset',
    '#title' => t('Redirection settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['pages']['asu_webauth_pages_check'] = array(
    '#type' => 'radios',
    '#title' => t('Require ASU Webauth login for'),
    '#default_value' => variable_get('asu_webauth_pages_check', 'none'),
    '#options' => array(
      'none' => t('no pages'),
      'all' => t('all pages'),
      'specific' => t('specific pages'),
      'all except' => t('all pages except specific pages'),
    ),
    '#description' => t('If one of the "specific pages" options is selected, pages must be defined below.'),
  );
  $form['pages']['asu_webauth_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Specific pages'),
    '#default_value' => variable_get('asu_webauth_pages', ''),
    '#description' => t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are '<em>blog</em>' for the blog page and '<em>blog/*</em>' for every personal blog. '<em>&lt;front&gt;</em>' is the front page."),
  );
  $form['pages']['asu_webauth_callapp'] = array(
    '#type' => 'textfield',
    '#title' => t('On login, redirect to'),
    '#default_value' => variable_get('asu_webauth_callapp', ''),
    '#description' => t('The URL Webauth should redirect to after successful login. If left blank, Drupal will use whatever page the user is currently on.'),
  );
  $form['pages']['asu_webauth_onLogoutURL'] = array(
    '#type' => 'textfield',
    '#title' => t('On logout, redirect to'),
    '#default_value' => variable_get('asu_webauth_onLogoutURL', 'https://webapp4.asu.edu/myasu/Signout'),
    '#description' => t('The URL Webauth should redirect to after successful logout. If left blank, Drupal will use whatever page the user is currently on.'),
  );

  //------- Misc Settings -------
  $form['misc'] = array(
    '#type' => 'fieldset',
    '#title' => t('Miscellaneous settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['misc']['asu_webauth_changePasswordURL'] = array(
    '#type' => 'textfield',
    '#title' => t('Change password URL'),
    '#description' => t('The URL users should use for changing their password. Set to blank to use the standard Drupal page.'),
    '#default_value' => variable_get('asu_webauth_changePasswordURL', ASU_WEBAUTH_CHANGE_PW_URL),
  );
  $form['misc']['asu_webauth_registerURL'] = array(
    '#type' => 'textfield',
    '#title' => t('Registration URL'),
    '#description' => t('The URL users should use for registration for the site. Set to blank to use the standard Drupal page.'),
    '#default_value' => variable_get('asu_webauth_registerURL', ASU_WEBAUTH_REGISTER_URL),
  );

  //------- Bypass Settings -------
  $form['bypass'] = array(
    '#type' => 'fieldset',
    '#title' => t('WebAuth Bypass settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['bypass']['asu_webauth_services'] = array(
    '#type' => 'textarea',
    '#title' => t('Service Pages'),
    '#default_value' => variable_get('asu_webauth_services', t("services/amfphp\nservices/json\nservices/rest\nservices/soap\nservices/xmlrpc")),
    '#description' => t("Enter one path per line as Drupal paths."),
  );

  $form['bypass']['asu_webauth_user_agents'] = array(
    '#type' => 'textarea',
    '#title' => t('User Agents'),
    '#default_value' => variable_get('asu_webauth_user_agents', t("googlebot\nmediapartners-google\nmsnbot\nslurp\njeeves\nnutch\nalexa.com\nbaiduspider\nlibcurl\nwget\ngsa-crawler")),
    '#description' => t("Enter one User Agent per line."),
  );

  return system_settings_form($form);
}
