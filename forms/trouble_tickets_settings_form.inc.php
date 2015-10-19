<?php
/**
 * @file
 * Defines the Trouble Ticket settings form.
 */

/**
 * Form constructor for the trouble tickets configuration settings form.
 *
 * @ingroup forms
 */
function trouble_tickets_settings_form($form, &$form_state) {

  drupal_set_title(
    t('Trouble Tickets configuration settings')
  );

  $form['fogbugz_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fogbugz credentials:'),
  );

  $form['fogbugz_settings']['fogbugz_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#default_value' => variable_get('trouble_tickets_fogbugz_email'),
    '#size' => 50,
    '#maxlength' => 50,
    '#required' => TRUE,
    '#prefix' => '<p>Enter the credentials for the FogBugz account that will be used for API calls in this module.</p>',
  );
  $form['fogbugz_settings']['fogbugz_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#default_value' => variable_get('trouble_tickets_fogbugz_password'),
    '#size' => 50,
    '#maxlength' => 50,
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
