<?php
/**
 * @file
 * Defines functionality for the Trouble Tickets topic form.
 */

/**
 * Form constructor for topic selection and ticket submission.
 *
 * @see trouble_tickets_submit
 *
 * @ingroup forms
 */
function trouble_tickets_form($form, &$form_state) {
  module_load_include('inc', 'trouble_tickets', 'includes/trouble_tickets');

  // Implement Javascript requirement for Form API.
  $form['#prefix'] = '<noscript>Sorry, Trouble Tickets requires Javascript.</noscript><div class="needs-js trouble-tickets-wrapper"><h2>Submit a Trouble Ticket</h2>';
  $form['#suffix'] = '</div>';

  // Attach module css and/or javascript files for module page only.
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'trouble_tickets') . '/css/trouble_tickets.css',
  );
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'trouble_tickets') . '/js/trouble_tickets.js',
  );

  $topic_options = _trouble_tickets_get_topics();
  $selected_topic_option = isset($form_state['values']['ticket_topic']) ? $form_state['values']['ticket_topic'] : key($topic_options);

  $form['ticket_topic'] = array(
    '#type' => 'select',
    '#title' => t('Select a topic'),
    '#empty_option' => '- Select -',
    '#options' => $topic_options,
    '#default_value' => $selected_topic_option,
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => '_trouble_tickets_ajax_topic_select_callback',
      'wrapper' => 'trouble-tickets-fieldset',
      'progress' => array(
        'type' => 'throbber',
        'message' => '',
      ),
      'effect' => 'fade',
    ),
  );

  $selected_topic_option = isset($form_state['values']['ticket_topic']) ? $form_state['values']['ticket_topic'] : '';
  $topic_name = _trouble_tickets_get_topic_name($selected_topic_option);

  // The div wrapping this fieldset has same ID ("trouble-tickets-fieldset") as
  // wrapper above. This div replaces placeholder during the AJAX callback, and
  // is itself replaced during subsequent callbacks.
  $form['ticket_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => 'Trouble ticket for ' . $topic_name,
    '#prefix' => '<div id="trouble-tickets-fieldset">',
    '#suffix' => '</div>',
    '#attributes' => array(
      'class' => array('wrapper-expanded')
    ),
    '#states' => array(
      'invisible' => array(
        ':input[name="ticket_topic"]' => array(
          array('value' => ''),
          array('value' => t('eresource')),
          array('value' => t('Worldcat Records')),
        ),
      ),
    ),
  );

  $form['ticket_fieldset']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#size' => 75,
    '#maxlength' => 250,
    '#required' => TRUE,
  );

  if ($selected_topic_option == 'Potential New Systems Projects') {
    $form['ticket_fieldset']['category'] = array(
      '#type' => 'hidden',
      '#title' => t('Category'),
      '#value' => 'Feature Request',
      '#required' => TRUE,
    );
  }
  else {
    $form['ticket_fieldset']['category'] = array(
      '#type' => 'radios',
      '#title' => t('Category'),
      '#options' => _trouble_tickets_get_categories($selected_topic_option),
      '#default_value' => 'Bug',
      '#required' => TRUE,
      '#prefix' => '<div class="form-container container-inline">',
      '#suffix' => '</div>',
    );
  }

  if ($selected_topic_option == 'Library Maintenance and Operations') {
    // Get the list of options to populate the Location select.
    $options_location = _trouble_tickets_get_location_options();

    // If we have a value for the Location select from $form_state['values']
    // use this both as the default value for the Location select and also as a
    // parameter passed to the function that retrieves the Room select options.
    $selected_location = isset($form_state['values']['location']) ? $form_state['values']['location'] : '';

    $form['ticket_fieldset']['location'] = array(
      '#type' => 'select',
      '#title' => 'Location',
      '#options' => $options_location,
      '#empty_option' => 'Not Available',
      '#default_value' => $selected_location,
      // Bind an ajax callback to change event of the Location select wrapper.
      // It will replace the Room select when rebuilt.
      '#ajax' => array(
        // When 'event' occurs, Drupal performs ajax request in the background.
        // Usually default value is sufficient (eg. change for select elements),
        // but any jQuery event is valid value, eg. mousedown, blur, & submit.
        'callback' => '_trouble_tickets_ajax_room_callback',
        'wrapper' => 'room-replace',
        'progress' => array(
          'type' => 'throbber',
          'message' => '',
        ),
      ),
    );

    $form['ticket_fieldset']['room'] = array(
      '#type' => 'select',
      '#title' => t('Room'),
      // Enclosing div created here is replaced when Location select is changed.
      '#prefix' => '<div id="room-replace">',
      '#suffix' => '</div>',
      // When form is rebuilt during ajax processing, $selected variable will
      // have the new value and so the options will change.
      '#empty_option' => 'Not Available',
      '#options' => _trouble_tickets_get_room_options($selected_location),
      '#default_value' => isset($form_state['values']['room']) ? $form_state['values']['room'] : '',
      '#states' => array(
        'invisible' => array(
          ':input[name="location"]' => array(
            array('value' => ''),
          ),
        ),
      ),
      // Ajax error 200 timeout suggestion.
      // Source: https://groups.drupal.org/node/169519#comment-913843
      '#validated' => TRUE,
    );
  }

  $form['ticket_fieldset']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#resizeable' => TRUE,
    '#rows' => 10,
    '#cols' => 70,
    '#required' => TRUE,
  );

  $allowed_file_ext = 'gif jpg png zip';
  $max_file_size = 8 * 1024 * 1024;
  $form['ticket_fieldset']['file'] = array(
    '#type' => 'managed_file',
    '#title' => t('Screenshot'),
    '#description' => t("You may optionally attach a screenshot to this trouble ticket.<br/>
      Valid extensions are <b>%allowed_file_ext</b>. File size cannot be > <b>%max_file_sizeMB</b>.", array(
        '%allowed_file_ext' => $allowed_file_ext,
        '%max_file_size' => $max_file_size / 1024 / 1024,
      )
    ),
    '#upload_location' => 'public://trouble_tickets',
    '#upload_validators' => array(
      'file_validate_extensions' => array(
        $allowed_file_ext,
      ),
      'file_validate_size' => array(
        $max_file_size,
      ),
    ),
  );

  $form['ticket_fieldset']['submit'] = array(
    '#type' => 'submit',
    '#name' => 'ticket-submit',
    '#value' => 'Submit',
    '#id' => 'ticket-submit',
    '#submit' => array('trouble_tickets_form_submit'),
  );

  return $form;
}

/**
 * Form submission handler for ticket selection portion of trouble_tickets_form.
 *
 * FogBugz post() method command arguments: new (default), forward.
 */
function trouble_tickets_form_submit($form, &$form_state) {
  module_load_include('inc', 'trouble_tickets', 'includes/trouble_tickets');
  $fb_obj = new FogbugzConnection();

  // FogBugz module adds connection failure msg but add 'ticket wasn't created'
  // message item as well.
  if (is_null($fb_obj->connectionToken)) {
    drupal_set_message(t('Your trouble ticket could not be created.'), 'error');
    return;
  }
  else {
    $fb_ticket_data = _trouble_tickets_get_fogbugz_ticket_data($form_state);
    $create_fogbugz_ticket_response = $fb_obj->post($fb_ticket_data);

    // Get FogBugz case number from xml response for email message.
    $simple_xml = simplexml_load_string($create_fogbugz_ticket_response, 'SimpleXMLElement', LIBXML_NOCDATA);
    $case_id = (string) $simple_xml->case['ixBug'];
  }

  if ($create_fogbugz_ticket_response && $case_id) {
    drupal_set_message(t('Your trouble ticket was successfully submitted. A confirmation email will be sent to your UNB email address.'));
    $fb_email_data = _trouble_tickets_get_fogbugz_email_data($form_state, $case_id);
    // Forward operation forwards copy of user email message to FogBugz ticket.
    $send_fogbugz_email_response = $fb_obj->post($fb_email_data, 'forward');
  }
  else {
    drupal_set_message(t('Your trouble ticket was not successfully submitted. If this error persists, please contact the site administrator.'), 'error');
  }
}
