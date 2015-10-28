<?php
/**
 * Form constructor for topic selection and ticket submission.
 *
 * @see trouble_tickets_submit
 *
 * @ingroup forms
 */
function trouble_tickets_form($form, &$form_state) {
  // Trouble Tickets Fogbugz credentials check.
  $fogbugz_user_email = variable_get('fogbugz_user_email');
  $fogbugz_user_pass = variable_get('fogbugz_user_password');
  if (is_null($fogbugz_user_email) || is_null($fogbugz_user_pass)) {
    drupal_set_message(t('FogBugz is not configured, please contact the site administrator.'), 'error');
    return;
  }

  // Implement Javascript requirement for Form API.
  $form['#prefix'] = '<noscript>Sorry, Trouble Tickets requires Javascript.</noscript><div class="needs-js trouble-tickets-wrapper"><h2>Submit a Trouble Ticket</h2>';
  $form['#suffix'] = '</div>'>

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
    '#empty_option' => '--Select--',
    '#options' => $topic_options,
    '#default_value' => $selected_topic_option,
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => 'trouble_tickets_ajax_topic_select_callback',
      'wrapper' => 'trouble-tickets-fieldset',
      'progress' => array(
        'type' => 'throbber',
        'message' => '',
      ),
    ),
  );

  // Get the visible display name of selected topic option.
  $selected_topic_option = isset($form_state['values']['ticket_topic']) ? $form_state['values']['ticket_topic'] : '';
  $topic_name = _trouble_tickets_get_topic_name($selected_topic_option);
  // The div wrapping this fieldset has the same ID ("trouble-tickets-fieldset") as the wrapper above.
  // This div replaces the placeholder during the AJAX callback, and is itself replaced during subsequent callbacks.

  // Display trouble ticket form fieldset if not empty or redirect option.
  $form['ticket_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => 'Trouble ticket for ' . $topic_name,
    '#prefix' => '<div id="trouble-tickets-fieldset">',
    '#suffix' => '</div>',
    '#states' => array(
      'invisible' => array(
        ':input[name="ticket_topic"]' => array(
          array('value' => ''),
          array('value' => 'eresource'),
          array('value' => 'Worldcat Records'),
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
  } else {
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
    // If we have a value for the Location select from $form_state['values'] we use this both as the default value for the Location select and also as a
    // parameter to pass to the function that retrieves the options for the Room select.
    $selected_location = isset($form_state['values']['location']) ? $form_state['values']['location'] : '';

    $form['ticket_fieldset']['location'] = array(
      '#type' => 'select',
      '#title' => 'Location',
      '#options' => $options_location,
      '#empty_option' => 'Not Available',
      '#default_value' => $selected_location,
      // Bind an ajax callback to the change event (which is the default for the select form type) of the Location select.
      // It will replace the Room select when rebuilt.
      '#ajax' => array(
        // When 'event' occurs, Drupal will perform an ajax request in the background. Usually the default value is sufficient
        // (eg. change for select elements), but valid values include any jQuery event, most notably 'mousedown', 'blur', and 'submit'.
        'callback' => 'trouble_tickets_ajax_room_callback',
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
      // The entire enclosing div created here gets replaced when Location select is changed.
      '#prefix' => '<div id="room-replace">',
      '#suffix' => '</div>',
      // When the form is rebuilt during ajax processing, the $selected variable will now have the new value and so the options will change.
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
        $allowed_file_ext
      ),
      'file_validate_size' => array(
        $max_file_size
      ),
    ),
  );

  $form['ticket_fieldset']['submit'] = array(
    '#type' => 'submit',
    '#name' => 'ticket-submit',
    '#value' => 'Submit',
    '#id' => 'ticket-submit',
    '#submit' => array('trouble_tickets_submit'),
  );

  return $form;
}

/**
 * Form submission handler for ticket selection portion of trouble_tickets_form().
 */
function trouble_tickets_submit($form, &$form_state) {
  $fb_obj = new FogbugzConnection;
  $success = FALSE; // force failure for now

  if ($success) {
    drupal_set_message(t('Your trouble ticket was successfully submitted. A confirmation email will be sent to your UNB email address.'));
  }
  else {
    drupal_set_message(t('Your trouble ticket was not successfully submitted. If this error persists, please contact the site administrator.'), 'error');
  }
}

/**
 * Selects just the second dropdown to be returned for re-rendering.
 *
 * Since the controlling logic for populating the form is in the form builder
 * function, all we do here is select the element and return it to be updated.
 *
 * @return array
 *   Renderable array (the second dropdown)
 */
function trouble_tickets_ajax_room_callback($form, $form_state) {
  // Only return the form if it's not the empty option.
  if ($form_state['values']['location'] != '') {
    return $form['ticket_fieldset']['room'];
  }
}

function trouble_tickets_ajax_topic_select_callback ($form, $form_state) {
  $selected_topic = $form_state['values']['ticket_topic'];

  // Topic options containing 'eresource' and 'Worldcat' redirect to e-Resources Trouble Ticket on Library website.
  if (strpos($selected_topic, 'eresource') !== false || strpos($selected_topic, 'Worldcat') !== false) {
    ctools_include('ajax');
    $commands[] = ctools_ajax_command_redirect('http://www.lib.unb.ca/help/troubleticket.php');
    return array(
      '#type' => 'ajax',
      '#commands' => $commands,
    );
  } elseif ($selected_topic != '') {
   $form_state['rebuild'] = TRUE;
    return $form['ticket_fieldset'];
  }
}

/**
 * Returns valid topics for trouble tickets.
 *
 * @return array
 *   Associative array of FogBugz project names (where applicable, for API communication) and topic display names.
 */
function _trouble_tickets_get_topics() {
  $ticket_topics = array(
    // select options in eResource optgroup should redirect to http://www.lib.unb.ca/help/troubleticket.php: tag as 'eResources'
    'eResources' => array(
      'eresource' => t('eJournals, eBooks, databases, etc.'),
      'Worldcat Records' => t('UNB WorldCat records, holdings, and display'),
    ),
    'Systems Group' => array(
      'Proxy / Authentication Issues' => t('Proxy / Authentication Issues'),
      'UNB WorldCat / WMS' => t('WMS'),
      'Document Delivery' => t('Document Delivery / Relais'),
      'Staff Machines / Printers' => t('Staff Machines & Printers'),
      'Public Machines / Printers' => t('Public Machines & Printers'),
      'Library Website' => t('Library Website'),
      'Potential New Systems Projects' => t('New Feature Request or Solution Required'),
      'Reserves' => t('Reserves'),
      'Staff Intranet' => t('Staff Intranet and Projects'),
      'Inbox' => t('Other Systems Issues'),
    ),
    'Centre for Digital Scholarship (ETC)' => array(
      'Centre for Digital Scholarship Projects' => t('CDS Projects'),
      'UNB Scholar' => t('UNB Scholar'),
      'Newspapers' => t('Newspapers'),
    ),
    'Library Maintenance and Operations (FR)' => array(
      'Library Maintenance and Operations' => t('Repairs / Requests'),
    ),
  );

  return $ticket_topics;
}


/**
 * Helper function to populate the Location dropdown.
 *
 * This would normally be pulling data from the database.
 *
 * @return array
 *   Dropdown options.
 */
function _trouble_tickets_get_location_options() {
  return drupal_map_assoc(
    array(
      t('Harriet Irving Library'),
      t('Science and Forestry Library'),
      t('Engineering and Computer Science Library'),
      t('Engineering and Computer Science Library (storage)'),
      t('South Gym'),
    )
  );
}

/**
 * Returns list of Rooms for library maintenance tickets.
 *
 * @return array
 *   Associative array of buildings and their Rooms.
 */
function _trouble_tickets_get_room_options($key = '') {
  $room_options = array(
    t('Harriet Irving Library') => drupal_map_assoc(
      array(
        t('Room 011B'),
        t('Room 012A'),
        t('Room 013A'),
        t('Room 013B'),
        t('Room 014A'),
        t('Room 01A'),
        t('Room 01B'),
        t('Room 01C'),
        t('Room 01D'),
        t('Room 05A'),
        t('Room 5A-1'),
        t('Room 06A'),
        t('Room 06B'),
        t('Room 06C'),
        t('Room 08A'),
        t('Room 10'),
        t('Room 102'),
        t('Room 102A'),
        t('Room 103A'),
        t('Room 103B'),
        t('Room 103C'),
        t('Room 104B'),
        t('Room 104C'),
        t('Room 104E'),
        t('Room 104F'),
        t('Room 105'),
        t('Room 105A'),
        t('Room 105B'),
        t('Room 105C'),
        t('Room 105D'),
        t('Room 106'),
        t('Room 107'),
        t('Room 108'),
        t('Room 109'),
        t('Room 110'),
        t('Room 111'),
        t('Room 111A'),
        t('Room 112'),
        t('Room 112A'),
        t('Room 112B'),
        t('Room 113'),
        t('Room 114'),
        t('Room 115'),
        t('Room 115A'),
        t('Room 115B'),
        t('Room 115C'),
        t('Room 116'),
        t('Room 116C'),
        t('Room 117'),
        t('Room 118'),
        t('Room 11A'),
        t('Room 11B'),
        t('Room 11C'),
        t('Room 12'),
        t('Room 13'),
        t('Room 14'),
        t('Room 15'),
        t('Room 18'),
        t('Room 19'),
        t('Room 20'),
        t('Room 201A'),
        t('Room 201B'),
        t('Room 201C'),
        t('Room 201D'),
        t('Room 204'),
        t('Room 204A'),
        t('Room 205'),
        t('Room 206'),
        t('Room 207'),
        t('Room 208'),
        t('Room 209'),
        t('Room 21'),
        t('Room 210'),
        t('Room 211'),
        t('Room 2'),
        t('Room 3'),
        t('Room 301B'),
        t('Room 301C'),
        t('Room 302A'),
        t('Room 302B'),
        t('Room 303'),
        t('Room 303A'),
        t('Room 306'),
        t('Room 307'),
        t('Room 308'),
        t('Room 308A'),
        t('Room 309'),
        t('Room 309A'),
        t('Room 310'),
        t('Room 311'),
        t('Room 311A'),
        t('Room 311B'),
        t('Room 311C'),
        t('Room 311D'),
        t('Room 311E'),
        t('Room 311F'),
        t('Room 311G'),
        t('Room 311H'),
        t('Room 314'),
        t('Room 315'),
        t('Room 316'),
        t('Room 317'),
        t('Room 318'),
        t('Room 319'),
        t('Room 320'),
        t('Room 321'),
        t('Room 322'),
        t('Room 4'),
        t('Room 401A'),
        t('Room 401B'),
        t('Room 401E'),
        t('Room 401F'),
        t('Room 408'),
        t('Room 408A'),
        t('Room 409'),
        t('Room 409A'),
        t('Room 409B'),
        t('Room 410'),
        t('Room 412'),
        t('Room 412B'),
        t('Room 413'),
        t('Room 414'),
        t('Room 415'),
        t('Room 415A'),
        t('Room 416'),
        t('Room 417'),
        t('Room 418'),
        t('Room 419'),
        t('Room 420'),
        t('Room 421'),
        t('Room 5'),
        t('Room 502'),
        t('Room 503'),
        t('Room 504'),
        t('Room 504A'),
        t('Room 505'),
        t('Room 505A'),
        t('Room 506'),
        t('Room 506A'),
        t('Room 507'),
        t('Room 507A'),
        t('Room 507B'),
        t('Room 507C'),
        t('Room 508'),
        t('Room 508A'),
        t('Room 508B'),
        t('Room 508C'),
        t('Room 508D'),
        t('Room 508E'),
        t('Room 508F'),
        t('Room 508G'),
        t('Room 508H'),
        t('Room 509'),
        t('Room 509A'),
        t('Room 509B'),
        t('Room 509C'),
        t('Room 510'),
        t('Room 511'),
        t('Room 511A'),
        t('Room 511B'),
        t('Room 511C'),
        t('Room 511D'),
        t('Room 511E'),
        t('Room 511F'),
        t('Room 511G'),
        t('Room 511H'),
        t('Room 511I'),
        t('Room 511J'),
        t('Room 511K'),
        t('Room 511L'),
        t('Room 511M'),
        t('Room 511N'),
        t('Room 511O'),
        t('Room 511P'),
        t('Room 513'),
        t('Room 514'),
        t('Room 514A'),
        t('Room 514B'),
        t('Room 514C'),
        t('Room 514D'),
        t('Room 514E'),
        t('Room 514F'),
        t('Room 514G'),
        t('Room 514H'),
        t('Room 514I'),
        t('Room 514J'),
        t('Room 514K'),
        t('Room 514L'),
        t('Room 514M'),
        t('Room 514N'),
        t('Room 514O'),
        t('Room 514P'),
        t('Room 515'),
        t('Room 516'),
        t('Room 516A'),
        t('Room 516B'),
        t('Room 516C'),
        t('Room 517'),
        t('Room 517A'),
        t('Room 517B'),
        t('Room 517C'),
        t('Room 517D'),
        t('Room 517E'),
        t('Room 517F'),
        t('Room 517G'),
        t('Room 517H'),
        t('Room 518'),
        t('Room 518A'),
        t('Room 518B'),
        t('Room 518C'),
        t('Room 518D'),
        t('Room 519'),
        t('Room 519A'),
        t('Room 520'),
        t('Room 520A'),
        t('Room 520B'),
        t('Room 521'),
        t('Room 522'),
        t('Room 6'),
        t('Room 600'),
        t('Room 601'),
        t('Room 7'),
        t('Room 8'),
        t('Room 9'),
        t('Room A'),
        t('Room A399'),
        t('Room A699'),
        t('Room B'),
        t('Room B199'),
        t('Room B199A'),
        t('Room B199B'),
        t('Room B399'),
        t('Room C'),
        t('Room C399'),
        t('Room C499'),
        t('Room C698'),
        t('Room D'),
        t('Room D499'),
        t('Room D698'),
        t('Media Storage Areas'),
        t('Room E'),
        t('Room E399A'),
        t('Room E399C'),
        t('Room E399D'),
        t('Room E'),
        t('Room E499'),
        t('Room F'),
        t('Room F399'),
        t('Room F499A'),
        t('Room F499B'),
        t('Room F499C'),
        t('Room F499D'),
        t('Room G'),
        t('Room H'),
        t('Room H499'),
        t('Room I'),
        t('Room I399'),
        t('Room J199'),
        t('Room J'),
        t('Room K'),
        t('Room L'),
        t('Room M99'),
      )
    ),

    t('Science and Forestry Library') => drupal_map_assoc(
      array(
        t('Room 101'),
        t('Room 101A - Central Core'),
        t('Room 101B - Central Core'),
        t('Room 101C - Reader Study'),
        t('Room 101D - Central Core'),
        t('Room 101E - Reader Study'),
        t('Room 105'),
        t('Room 105A'),
        t('Room 105B'),
        t('Room 114 - Central Core'),
        t('Room D199'),
        t('Room 629C – I.U.C. Central Core'),
      )
    ),

    t('Engineering and Computer Science Library') => drupal_map_assoc(
      array(
        t('Room C8'),
        t('Room C14'),
        t('Room C15A'),
        t('Room C18A'),
        t('Room C34A'),
        t('Room C34'),
        t('Room 609N'),
      )
    ),

    t('Engineering and Computer Science Library (storage)') => drupal_map_assoc(
      array(
        t('609G – Gillin Hall'),
        t('A117 - Equip Div Stations'),
      )
    ),

    t('South Gym') => drupal_map_assoc(
      array(
        t('Room 198C'),
        t('Room 201'),
        t('Room 202'),
        t('Room 203'),
        t('Room 204'),
        t('Room 205'),
        t('Room 206'),
        t('Room 207'),
        t('Room 208'),
        t('Room 209'),
        t('Room 298A'),
        t('Room 298B'),
        t('Room 299A'),
        t('Room 299B'),
        t('Room 299C'),
        t('Room 630S'),
      )
    ),
  );
  if (isset($room_options[$key])) {
    return $room_options[$key];
  }
  else {
    return array();
  }
}

/**
 * Returns valid categories for trouble tickets.
 *
 * @param string $topic
 *   Topic of ticket
 *
 * @return array
 *   Associative array of FogBugz category names (for API communication) and display names.
 */
function _trouble_tickets_get_categories($topic) {
  $ticket_categories = array(
    'Bug' => $topic == 'Library Maintenance and Operations' ? t('Repair') : t('Bug'),
    'Inquiry' => t('Inquiry'),
    'Feature Request' => $topic == 'Library Maintenance and Operations' ? t('Request') : t('Feature Request'),
  );
  return $ticket_categories;
}

function _trouble_tickets_get_topic_name($topic) {
  $valid_topics = _trouble_tickets_get_topics();
  foreach ($valid_topics as $topic_set) {
    if (array_key_exists($topic, $topic_set)) {
      return $topic_set[$topic];
    }
  }
}
