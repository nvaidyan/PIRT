<?php
// asuzen 6.x-3.0

/**
 * @file
 * Contains theme override functions and preprocess functions for the theme.
 *
 * ABOUT THE TEMPLATE.PHP FILE
 *
 *   The template.php file is one of the most useful files when creating or
 *   modifying Drupal themes. You can add new regions for block content, modify
 *   or override Drupal's theme functions, intercept or make additional
 *   variables available to your theme, and create custom PHP logic. For more
 *   information, please visit the Theme Developer's Guide on Drupal.org:
 *   http://drupal.org/theme-guide
 *
 * OVERRIDING THEME FUNCTIONS
 *
 *   The Drupal theme system uses special theme functions to generate HTML
 *   output automatically. Often we wish to customize this HTML output. To do
 *   this, we have to override the theme function. You have to first find the
 *   theme function that generates the output, and then "catch" it and modify it
 *   here. The easiest way to do it is to copy the original function in its
 *   entirety and paste it here, changing the prefix from theme_ to STARTERKIT_.
 *   For example:
 *
 *     original: theme_breadcrumb()
 *     theme override: STARTERKIT_breadcrumb()
 *
 *   where STARTERKIT is the name of your sub-theme. For example, the
 *   zen_classic theme would define a zen_classic_breadcrumb() function.
 *
 *   If you would like to override any of the theme functions used in Zen core,
 *   you should first look at how Zen core implements those functions:
 *     theme_breadcrumbs()      in zen/template.php
 *     theme_menu_item_link()   in zen/template.php
 *     theme_menu_local_tasks() in zen/template.php
 *
 *   For more information, please visit the Theme Developer's Guide on
 *   Drupal.org: http://drupal.org/node/173880
 *
 * CREATE OR MODIFY VARIABLES FOR YOUR THEME
 *
 *   Each tpl.php template file has several variables which hold various pieces
 *   of content. You can modify those variables (or add new ones) before they
 *   are used in the template files by using preprocess functions.
 *
 *   This makes THEME_preprocess_HOOK() functions the most powerful functions
 *   available to themers.
 *
 *   It works by having one preprocess function for each template file or its
 *   derivatives (called template suggestions). For example:
 *     THEME_preprocess_page    alters the variables for page.tpl.php
 *     THEME_preprocess_node    alters the variables for node.tpl.php or
 *                              for node-forum.tpl.php
 *     THEME_preprocess_comment alters the variables for comment.tpl.php
 *     THEME_preprocess_block   alters the variables for block.tpl.php
 *
 *   For more information on preprocess functions, please visit the Theme
 *   Developer's Guide on Drupal.org: http://drupal.org/node/223430
 *   For more information on template suggestions, please visit the Theme
 *   Developer's Guide on Drupal.org: http://drupal.org/node/223440 and
 *   http://drupal.org/node/190815#template-suggestions
 */


/*
 * Add any conditional stylesheets you will need for this sub-theme.
 *
 * To add stylesheets that ALWAYS need to be included, you should add them to
 * your .info file instead. Only use this section if you are including
 * stylesheets based on certain conditions.
 */
// Optionally add a fixed width CSS file.
if (theme_get_setting('asuzen_fixed')) {
  drupal_add_css(path_to_theme() . '/layout-fixed.css', 'theme', 'all');
}


/**
 * Implementation of HOOK_theme().
 */
function asuzen_theme(&$existing, $type, $theme, $path) {
  $hooks = zen_theme($existing, $type, $theme, $path);
  // Add your theme hooks like this:
  /*
  $hooks['hook_name_here'] = array( // Details go here );
  */
  // @TODO: Needs detailed comments. Patches welcome!
  return $hooks;
}

/**
 * Override or insert variables into all templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered (name of the .tpl.php file.)
 */
/* -- Delete this line if you want to use this function
function asuzen_preprocess(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the page templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("page" in this case.)
 */
function asuzen_preprocess_page(&$vars, $hook) {
  /** pulled from http://stackoverflow.com/questions/247991/displaying-a-drupal-view-without-a-page-template-around-it */
  if ( isset($_GET['ajax']) && $_GET['ajax'] == 1 ) {
        $vars['template_file'] = 'page-ajax';
  }

  /* variable added by Nick Vaidyanathan 4/11/11 to move the physics header to an externally includable file */  
  $vars['physics_header'] = '/home/pirtadmn/public_html/sites/all/themes/asuzen/physicsHeader.html';
  $vars['pirt_header'] = '/home/pirtadmn/public_html/sites/all/themes/asuzen/pirtHeader.html';
  global $user;

  //display an error if the cas module and webauth module are enabled at the same time
  if(module_exists('cas') && module_exists('asu_webauth')) {
    drupal_set_message('Please  <a href="'.base_path().'admin/build/modules"> disable the asu_webauth</a> module for the CAS module to function properly.');
  }

  $vars['asu_sso_signedin'] = 'false';
  if ($user->uid) {
    $vars['asu_sso_signedin'] = 'true';
  }
  $vars['asu_sso_signinurl'] = '/user/login?destination='.urlencode(drupal_get_path_alias($_GET['q']));
  $vars['asu_sso_signouturl'] = '/logout';

  if(module_exists('cas')) {
    $vars['asu_sso_signinurl'] = '/cas?destination='.urlencode(drupal_get_path_alias($_GET['q']));
    $vars['asu_sso_signouturl'] = '/caslogout';
  }

  /************** ~-ASUZEN 3.0 additions-~ *********************/

  /**
   * #1 Correct information about the presence of a
   * left sidebar in <body> classes
   */

  // true left determines the presence of any of the theming elements
  // that are located on top of the left sidebar region
  $true_left = $vars['logo'] || $vars['site_name'] || $vars['site_slogan'];

  // Unset no-sidebars array element
  if ($index = array_search('no-sidebars', $vars['classes_array'])) {
    unset($vars['classes_array'][$index]);
    if (!$true_left) {
      $vars['classes_array'][] = 'no-sidebars';
    }
    else {
      $vars['classes_array'][] = 'one-sidebar';
      $vars['classes_array'][] = 'sidebar-left';
    }
  }
  else if ($index = array_search('one-sidebar', $vars['classes_array'])) {
    if ($index2 = array_search('sidebar-right', $vars['classes_array'])) {
      unset($vars['classes_array'][$index]);
      unset($vars['classes_array'][$index2]);
      if (!$true_left) {
        $vars['classes_array'][] = 'one-sidebar';
        $vars['classes_array'][] = 'sidebar-right';
      }
      else {
        $vars['classes_array'][] = 'two-sidebars';
      }
    }
  }

	/**
	 * #2 Create new variable to set path to ASU header
	 */
	$vars['asu_header_version'] = theme_get_setting('asuzen_asu_header_version');
	
	$vars['asu_header_basepath'] = theme_get_setting('asuzen_asu_header_basepath');
	if (!$vars['asu_header_basepath']) {
		$vars['asu_header_basepath'] = '/afs/asu.edu/www/asuthemes';
	}
	$vars['asu_header_template'] = theme_get_setting('asuzen_asu_header_template');
	if (!$vars['asu_header_template']) {
		$vars['asu_header_template'] = 'default';
	}
	$vars['asu_footer_color'] = theme_get_setting('asuzen_asu_footer_color');
	
	$vars['asu_ga_alternate'] = theme_get_setting('asuzen_ga_alternate');
}

/**
 * Override or insert variables into the node templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("node" in this case.)
 */
/* -- Delete this line if you want to use this function
function asuzen_preprocess_node(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the comment templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("comment" in this case.)
 */
/* -- Delete this line if you want to use this function
function asuzen_preprocess_comment(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the block templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
/* -- Delete this line if you want to use this function
function asuzen_preprocess_block(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */
