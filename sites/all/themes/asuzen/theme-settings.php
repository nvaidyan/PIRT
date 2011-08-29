<?php
// asuzen 6.x-3.x

// Include the definition of zen_settings() and zen_theme_get_default_settings().
include_once './' . drupal_get_path('theme', 'zen') . '/theme-settings.php';


/**
 * Implementation of THEMEHOOK_settings() function.
 *
 * @param $saved_settings
 *   An array of saved settings for this theme.
 * @return
 *   A form array.
 */
function asuzen_settings($saved_settings) {

  // Get the default values from the .info file.
  $defaults = zen_theme_get_default_settings('asuzen');

  // Merge the saved variables and their default values.
  $settings = array_merge($defaults, $saved_settings);

  /*
   * Create the form using Forms API: http://api.drupal.org/api/6
   */

  $form = array();

  $form['asuzen_asu_header_basepath'] = array(
    '#type'          => 'textfield',
    '#title'         => t('ASU theme basepath'),
    '#description'   => t("Enter the path to the asuthemes folder."),
    '#default_value' => $settings['asuzen_asu_header_basepath'],
    '#element_validate' => array('asuzen_asu_header_validate'),
	'#required'		 => true,
  );
  
  $form['asuzen_asu_header_template'] = array(
    '#type'          => 'textfield',
    '#title'         => t('ASU header template key'),
    '#description'   => t("Enter the key for your custom header."),
    '#default_value' => $settings['asuzen_asu_header_template'],
    '#element_validate' => array('asuzen_asu_header_validate'),
	'#required'		 => true,
  );
  
  $form['asuzen_asu_footer_color'] = array(
    '#type'          => 'select',
    '#title'         => t('ASU Footer Color'),
    '#description'   => t("Select the color of the footer"),
	'#options'		 => array('' => 'Black', '_light_grey' => 'Light Grey', '_dark_grey' => 'Dark Grey'),
    '#default_value' => $settings['asuzen_asu_footer_color'],
    '#element_validate' => array('asuzen_asu_header_validate'),
  );

  $form['asuzen_fixed'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Fixed width layout'),
    '#default_value' => $settings['asuzen_fixed'],
    '#description'   => t("Sets a fixed-width of 1000px."),
  );
  
  $form['asuzen_ga_alternate'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Alternate Google Analytics Account'),
    '#default_value' => $settings['asuzen_ga_alternate'],
    '#description'   => t('Track users in an additional Google Analytics account. Format: UA-XXXXX-X'),
	'#element_validate' => array('asuzen_asu_ga_validate'),
  );


  // Add the base theme's settings.
  $form += zen_settings($saved_settings, $defaults);

  // Remove some of the base theme's settings.
  unset($form['themedev']['zen_layout']); // We don't need to select the base stylesheet.

  // Return the form
  return $form;
}

/**
* Capture theme settings submissions and validate
*/
function asuzen_asu_header_validate ($element, &$form_state) {
	$defaults = zen_theme_get_default_settings('asuzen');
	
	if ($element['#name'] == 'asuzen_asu_header_basepath') {
		if (!file_exists($form_state['values']['asuzen_asu_header_basepath'])) {
			form_error($element, t("{$element['#title']}: Base path does not exist."));
		}
	} /*else if ($element['#name'] == 'asuzen_asu_header_version') {
		$folder_path = $form_state['values']['asuzen_asu_header_basepath'].'/'.$form_state['values']['asuzen_asu_header_version'];
		if (!file_exists($folder_path)) {
			form_error($element, t("{$element['#title']}: Selected header version is not available."));
		}
	}*/ else if ($element['#name'] == 'asuzen_asu_header_template') {
		$header_path = $form_state['values']['asuzen_asu_header_basepath'].'/'.
			$defaults['asuzen_asu_header_version'].'/headers/'.
			$form_state['values']['asuzen_asu_header_template'].'.shtml';
		if (!file_exists($header_path)) {
			form_error($element, t("{$element['#title']}: Selected header template does not exist."));
		}
	} else if ($element['#name'] == 'asuzen_asu_footer_color') {
		$footer_path = $form_state['values']['asuzen_asu_header_basepath'].'/'.
			$defaults['asuzen_asu_header_version'].'/includes/'.
			'footer'.$form_state['values']['asuzen_asu_footer_color'].'.shtml';
		if (!file_exists($footer_path)) {
			form_error($element, t("{$element['#title']}: Selected footer color does not exist."));
		}
	}
}

function asuzen_asu_ga_validate($element, &$form_state) {
	if ($element['#value']) {
		// ensure upper case
		$element['#value'] = trim(strtoupper($element['#value']));
		
		// check our regular expression
		preg_match('/^UA\-[0-9]+\-[0-9]+$/', $element['#value'], $matches);
		if (count($matches) == 0) {
			form_error($element, t("Please verify the format of your Google Analytics account. It should resemble UA-####-#."));
		}
		
		$form_state['values']['asuzen_ga_alternate'] = $element['#value'];
	}
}