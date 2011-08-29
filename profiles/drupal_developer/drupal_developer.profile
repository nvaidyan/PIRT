<?php
// $Id: drupal_developer.profile,v 1.6.2.6 2009/09/03 08:00:46 snufkin Exp $
/**
 * @file
 * Install profile for a developer setup
 */

/**
 * Return an array of the modules to be enabled when this profile is installed.
 *
 * @return
 *   An array of modules to enable.
 */
function drupal_developer_profile_modules() {
  return array(
    // Enable core modules.
    'user', 'filter', 'block', 'node', 'taxonomy', 'comment', 'profile', 'menu', 'dblog',
    // enable devel modules.
    'devel', 'devel_generate', 
    // Enable admin menu.
    'admin_menu',
    // Enable install profile api.
    'install_profile_api',
    // Enable coder.
    'coder',
  );
}

function drupal_developer_profile_details() {
  return array(
    'name' => 'Drupal developer profile',
    'description' => 'Profile containing and installing development modules. WARNING! It contains dangerous user permissions, for development purposes only.',
  );
}

/**
 * Return a list of tasks that this profile supports.
 *
 * @return
 *   A keyed array of tasks the profile will perform during
 *   the final stage. The keys of the array will be used internally,
 *   while the values will be displayed to the user in the installer
 *   task list.
 */
function drupal_developer_profile_task_list() {
}

/**
 * Perform any final installation tasks for this profile.
 *
 * @param $task
 *   The current $task of the install system. When hook_profile_tasks()
 *   is first called, this is 'profile'.
 * @param $url
 *   Complete URL to be used for a link or form action on a custom page,
 *   if providing any, to allow the user to proceed with the installation.
 *
 * @return
 *   An optional HTML string to display to the user. Only used if you
 *   modify the $task, otherwise discarded.
 */
function drupal_developer_profile_tasks(&$task, $url) {
  // Insert default user-defined node types into the database. For a complete
  // list of available node type attributes, refer to the node type API
  // documentation at: http://api.drupal.org/api/HEAD/function/hook_node_info.
  $types = array(
    array(
      'type' => 'page',
      'name' => st('Page'),
      'module' => 'node',
      'description' => st("A <em>page</em>, similar in form to a <em>story</em>, is a simple method for creating and displaying information that rarely changes, such as an \"About us\" section of a website. By default, a <em>page</em> entry does not allow visitor comments and is not featured on the site's initial home page."),
      'custom' => TRUE,
      'modified' => TRUE,
      'locked' => FALSE,
      'help' => '',
      'min_word_count' => '',
    ),
    array(
      'type' => 'story',
      'name' => st('Story'),
      'module' => 'node',
      'description' => st("A <em>story</em>, similar in form to a <em>page</em>, is ideal for creating and displaying content that informs or engages website visitors. Press releases, site announcements, and informal blog-like entries may all be created with a <em>story</em> entry. By default, a <em>story</em> entry is automatically featured on the site's initial home page, and provides the ability to post comments."),
      'custom' => TRUE,
      'modified' => TRUE,
      'locked' => FALSE,
      'help' => '',
      'min_word_count' => '',
    ),
  );

  foreach ($types as $type) {
    $type = (object) _node_type_set_defaults($type);
    node_type_save($type);
  }

  // Default page to not be promoted and have comments disabled.
  variable_set('node_options_page', array('status'));
  variable_set('comment_page', COMMENT_NODE_DISABLED);

  // Don't display date and author information for page nodes by default.
  $theme_settings = variable_get('theme_settings', array());
  $theme_settings['toggle_node_info_page'] = FALSE;
  variable_set('theme_settings', $theme_settings);
  $core_required = array('block', 'filter', 'node', 'system', 'user');
  install_include(array_merge(drupal_developer_profile_modules(), $core_required));

  drupal_developer_configure_devel();

  // Update the menu router information.
  menu_rebuild();
}


/**
 * Implementation of hook_form_alter().
 *
 * Allows the profile to alter the site-configuration form. This is
 * called through custom invocation, so $form_state is not populated.
 */
function drupal_developer_form_alter(&$form, $form_state, $form_id) {
  if ($form_id == 'install_configure') {
    drupal_set_message(t('Configuration was provided by the developer profile. Default username: drupal. Default password: drupal.'), 'notice');

    $form['site_information']['#type'] = 'hidden';
    $form['admin_account']['#type'] = 'hidden';

    $form['site_information']['site_name'] = array(
      '#value' => t('Drupal dev site'),
      '#type' => 'value',
    );

    $form['site_information']['site_mail'] = array(
      '#value' => 'drupal@localhost.com',
      '#type' => 'value',
    );

    $form['admin_account']['account']['name'] = array(
      '#value' => 'drupal',
      '#type' => 'value',
    );

    $form['admin_account']['account']['mail'] = array(
      '#value' => 'drupal@localhost.com',
      '#type' => 'value',
    );
    $form['admin_account']['account']['pass'] = array(
      '#value' => 'drupal',
      '#type' => 'value',
    );
    // @TODO check for clean urls via drupal_http_request
    // @TODO check for timezone automatically
  }
}

function drupal_developer_configure_devel() {
  install_add_block('menu', 'devel', 'garland', 1, 1, 'left');
  install_add_block('devel', 0, 'garland', 1, 2, 'left');
  install_add_block('devel', 2, 'garland', 1, 0, 'footer');

  // Configure settings.
  // Save any old SMTP library
  $smtp_old = variable_get('smtp_library', '');
  $smtp_new = drupal_get_filename('module', 'devel');
  if ($smtp_old != '' && $smtp_old != $smtp_new) {
    variable_set('devel_old_smtp_library', $smtp_old);
  }
  variable_set('smtp_library', $smtp_new);
  // Make sure the switch user block is big enough for all the
  // auto-generated users on the site.
  variable_set('devel_switch_user_list_size', 15);
}

function drupal_developer_profile_final() {
  return t('Welcome to <a href="@url>your new development install</a>. Username:password is drupal:drupal. Happy coding.', array('@url' => url('')));
}
