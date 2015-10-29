<?php
/**
 * Redirects or renders the fieldset, depending on topic selected.
 *
 * @return array
 *   Renderable array (the ticket fieldset)
 */
function _trouble_tickets_ajax_topic_select_callback ($form, $form_state) {
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
    // Prevent Drupal messages to additionally display in field wrapper on ajax form rebuild.
    drupal_get_messages();

    return $form['ticket_fieldset'];
  }
}

/**
 * Selects just the room select element to be returned for re-rendering.
 *
 * Since the controlling logic for populating the form is in the form builder
 * function, all we do here is select the element and return it to be updated.
 *
 * @return array
 *   Renderable array (the room select element)
 */
function _trouble_tickets_ajax_room_callback($form, $form_state) {
  // Only return the form if it's not the empty option.
  if ($form_state['values']['location'] != '') {
    // Prevent Drupal messages to additionally display in field wrapper on ajax form rebuild.
    drupal_get_messages();

    return $form['ticket_fieldset']['room'];
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
 * Returns valid categories for trouble tickets.
 *
 * @param string $topic
 *   Topic of ticket
 *
 * @return array
 *   Associative array of FogBugz category label names.
 */
function _trouble_tickets_get_categories($topic) {
  $ticket_categories = array(
    'Bug' => ($topic == 'Library Maintenance and Operations') ? t('Repair') : t('Bug'),
    'Inquiry' => t('Inquiry'),
    'Feature Request' => ($topic == 'Library Maintenance and Operations') ? t('Request') : t('Feature Request'),
  );

  return $ticket_categories;
}

/**
 * Helper function to populate the Location select element.
 *
 * This would normally be pulling data from the database.
 *
 * @return array
 *   Options for Location select element.
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
 *   Options for Rooms select element.
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
 * Create a ticket in the FogBugz system based on user input from the ticket form.
 *
 * @param string $token
 *   Token for FogBugz API calls
 * @param array $form_state
 *   Form state
 *
 * @return bool
 *   TRUE if ticket creation successfully returned a case ID, FALSE if not
 */
function _trouble_tickets_get_fogbugz_data($form_state) {
  $ticket_information = array(
    'project' => $form_state['values']['ticket_topic'],
    'title' => $form_state['values']['title'],
    'category' => $form_state['values']['category'],
    'description' => $form_state['values']['description'],
  );
  $ticket_information['priority'] = _trouble_tickets_get_fogbugz_priority($ticket_information['category']);

  // Get Drupal username append "@unb.ca" to form customer email.
  global $user;
  $user_profile = entity_metadata_wrapper('user', user_load($user->uid));
  $user_name = (is_null($user_profile->field_first_name->value())) ? $user_profile->name->value() : $user_profile->field_first_name->value() . ' ' . $user_profile->field_last_name->value();
  error_log('USER Name=' . $user_name);
  return FALSE;
}

/**
 * Helper function that returns appropriate ticket priority based on category.
 *
 * @param string $category
 *   Category of ticket
 *
 * @return int
 *   Priority of ticket
 */
function _trouble_tickets_get_fogbugz_priority($category) {
  switch ($category) {
    case 'Bug':
      return 2;

    break;

    case 'Inquiry':
      return 3;

    break;

    case 'New Feature':
      return 4;

    break;
  }
}

function _trouble_tickets_get_topic_name($topic) {
  $valid_topics = _trouble_tickets_get_topics();
  foreach ($valid_topics as $topic_set) {
    if (array_key_exists($topic, $topic_set)) {
      return $topic_set[$topic];
    }
  }
}
