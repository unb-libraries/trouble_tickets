<?php
/**
 * @file
 * Helper functions for Trouble Tickets form rendering.
 */

/**
 * Redirects or renders the fieldset, depending on topic selected.
 *
 * @return array
 *   Renderable array (the ticket fieldset)
 */
function _trouble_tickets_ajax_topic_select_callback($form, $form_state) {
  $selected_topic = $form_state['values']['ticket_topic'];

  // Topic options containing 'eresource' and 'Worldcat' redirect to
  // e-Resources Trouble Ticket on Library website.
  if (strpos($selected_topic, 'eresource') !== FALSE || strpos($selected_topic, 'Worldcat') !== FALSE) {
    ctools_include('ajax');
    $commands[] = ctools_ajax_command_redirect('http://www.lib.unb.ca/help/troubleticket.php');

    return array(
      '#type' => 'ajax',
      '#commands' => $commands,
    );
  }
  elseif ($selected_topic) {
    // Prevent duplicate Drupal message display in wrapper on ajax form rebuild.
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
  if ($form_state['values']['location']) {
    // Prevent duplicate Drupal message display in wrapper on ajax form rebuild.
    drupal_get_messages();

    return $form['ticket_fieldset']['room'];
  }
}

/**
 * Provides values for Trouble Tickets topic select options.
 *
 * @return array
 *   Associative array of FogBugz project names (where applicable,
 *   for API communication) and topic display names.
 */
function _trouble_tickets_get_topics() {
  $ticket_topics = array(
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
 *   Topic of ticket.
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
        t('011B'),
        t('012A'),
        t('013A'),
        t('013B'),
        t('014A'),
        t('01A'),
        t('01B'),
        t('01C'),
        t('01D'),
        t('05A'),
        t('5A-1'),
        t('06A'),
        t('06B'),
        t('06C'),
        t('08A'),
        t('10'),
        t('100'),
        t('102'),
        t('102A'),
        t('103A'),
        t('103B'),
        t('103C'),
        t('104B'),
        t('104C'),
        t('104E'),
        t('104F'),
        t('105'),
        t('105A'),
        t('105B'),
        t('105C'),
        t('105D'),
        t('106'),
        t('107'),
        t('108'),
        t('109'),
        t('110'),
        t('111'),
        t('111A'),
        t('112'),
        t('112A'),
        t('112B'),
        t('113'),
        t('114'),
        t('115'),
        t('115A'),
        t('115B'),
        t('115C'),
        t('116'),
        t('116C'),
        t('117'),
        t('118'),
        t('11A'),
        t('11B'),
        t('11C'),
        t('12'),
        t('13'),
        t('14'),
        t('15'),
        t('18'),
        t('19'),
        t('20'),
        t('201A'),
        t('201B'),
        t('201C'),
        t('201D'),
        t('204'),
        t('204A'),
        t('205'),
        t('206'),
        t('207'),
        t('208'),
        t('209'),
        t('21'),
        t('210'),
        t('211'),
        t('2'),
        t('3'),
        t('301B'),
        t('301C'),
        t('302A'),
        t('302B'),
        t('303'),
        t('303A'),
        t('306'),
        t('307'),
        t('308'),
        t('308A'),
        t('309'),
        t('309A'),
        t('310'),
        t('311'),
        t('311A'),
        t('311B'),
        t('311C'),
        t('311D'),
        t('311E'),
        t('311F'),
        t('311G'),
        t('311H'),
        t('314'),
        t('315'),
        t('316'),
        t('317'),
        t('318'),
        t('319'),
        t('320'),
        t('321'),
        t('322'),
        t('4'),
        t('401A'),
        t('401B'),
        t('401E'),
        t('401F'),
        t('408'),
        t('408A'),
        t('409'),
        t('409A'),
        t('409B'),
        t('410'),
        t('412'),
        t('412B'),
        t('413'),
        t('414'),
        t('415'),
        t('415A'),
        t('416'),
        t('417'),
        t('418'),
        t('419'),
        t('420'),
        t('421'),
        t('5'),
        t('502'),
        t('503'),
        t('504'),
        t('504A'),
        t('505'),
        t('505A'),
        t('506'),
        t('506A'),
        t('507'),
        t('507A'),
        t('507B'),
        t('507C'),
        t('508'),
        t('508A'),
        t('508B'),
        t('508C'),
        t('508D'),
        t('508E'),
        t('508F'),
        t('508G'),
        t('508H'),
        t('509'),
        t('509A'),
        t('509B'),
        t('509C'),
        t('510'),
        t('511'),
        t('511A'),
        t('511B'),
        t('511C'),
        t('511D'),
        t('511E'),
        t('511F'),
        t('511G'),
        t('511H'),
        t('511I'),
        t('511J'),
        t('511K'),
        t('511L'),
        t('511M'),
        t('511N'),
        t('511O'),
        t('511P'),
        t('513'),
        t('514'),
        t('514A'),
        t('514B'),
        t('514C'),
        t('514D'),
        t('514E'),
        t('514F'),
        t('514G'),
        t('514H'),
        t('514I'),
        t('514J'),
        t('514K'),
        t('514L'),
        t('514M'),
        t('514N'),
        t('514O'),
        t('514P'),
        t('515A'),
        t('515B'),
        t('515BA'),
        t('516'),
        t('516A'),
        t('516B'),
        t('516C'),
        t('517'),
        t('517A'),
        t('517B'),
        t('517C'),
        t('517D'),
        t('517E'),
        t('517F'),
        t('517G'),
        t('517H'),
        t('518'),
        t('518A'),
        t('518B'),
        t('518C'),
        t('518D'),
        t('519'),
        t('519A'),
        t('520'),
        t('520A'),
        t('520B'),
        t('521'),
        t('522'),
        t('6'),
        t('600'),
        t('601'),
        t('7'),
        t('8'),
        t('9'),
        t('A'),
        t('A399'),
        t('A699'),
        t('B'),
        t('B199'),
        t('B199A'),
        t('B199B'),
        t('B399'),
        t('C'),
        t('C399'),
        t('C499'),
        t('C698'),
        t('D'),
        t('D499'),
        t('D698'),
        t('Media Storage Areas'),
        t('E'),
        t('E399A'),
        t('E399C'),
        t('E399D'),
        t('E'),
        t('E499'),
        t('F'),
        t('F399'),
        t('F499A'),
        t('F499B'),
        t('F499C'),
        t('F499D'),
        t('G'),
        t('H'),
        t('H499'),
        t('I'),
        t('I399'),
        t('J199'),
        t('J'),
        t('K'),
        t('L'),
        t('M99'),
      )
    ),

    t('Science and Forestry Library') => drupal_map_assoc(
      array(
        t('101'),
        t('101A - Central Core'),
        t('101B - Central Core'),
        t('101C - Reader Study'),
        t('101D - Central Core'),
        t('101E - Reader Study'),
        t('105'),
        t('105A'),
        t('105B'),
        t('114 - Central Core'),
        t('D199'),
        t('629C – I.U.C. Central Core'),
      )
    ),

    t('Engineering and Computer Science Library') => drupal_map_assoc(
      array(
        t('C8'),
        t('C14'),
        t('C15A'),
        t('C18A'),
        t('C34A'),
        t('C34'),
        t('609N'),
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
        t('198C'),
        t('201'),
        t('202'),
        t('203'),
        t('204'),
        t('205'),
        t('206'),
        t('207'),
        t('208'),
        t('209'),
        t('298A'),
        t('298B'),
        t('299A'),
        t('299B'),
        t('299C'),
        t('630S'),
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
 * Generates post data for creating FogBugz tickets from ticket form submission.
 *
 * @param array $form_state
 *   Form state.
 *
 * @return associative array of post data
 *   Post data arguments formatted for FogBugz create ticket commmand.
 */
function _trouble_tickets_get_fogbugz_ticket_data($form_state) {
  $auth_user = _trouble_tickets_get_auth_user_info();
  $customer_email = $auth_user['email_addr'];

  // Copy 'Instruction Facilities > Library Assistant' on
  // Public Machines + Library Maintenance ticket topics.
  if ($form_state['values']['ticket_topic'] == 'Public Machines / Printers' || $form_state['values']['ticket_topic'] == 'Library Maintenance and Operations') {
    $customer_email .= '; Alicia McLaughlin <amclaugh@unb.ca>';
  }

  $event = "Submitted by: $auth_user[first_name] $auth_user[last_name]\n\n" . $form_state['values']['description'];

  // Append location info to description text, if applicable.
  $location = _trouble_tickets_get_location_string($form_state);
  if ($location) {
    $event .= $location;
  }

  // Append file attachment info at end of FogBugz sEvent, if applicable.
  $form_fid = $form_state['values']['file'];
  if ($form_fid) {
    $form_file = file_load($form_fid);
    $event .= "\n\nAttachment: " . $form_file->filename;
  }

  $post_data = array(
    'sProject' => $form_state['values']['ticket_topic'],
    'sTitle' => $form_state['values']['title'],
    'sCategory' => $form_state['values']['category'],
    'sEvent' => $event,
    'ixPriority' => _trouble_tickets_get_fogbugz_priority($form_state['values']['category']),
    'sTags' => 'trouble_ticket',
    'sCustomerEmail' => $customer_email,
    'ixMailbox' => 1,
  );

  // Upload attached file to FogBugz ticket, nFileCount omitted as max = 1 file.
  if ($form_fid) {
    $form_file_uri = drupal_realpath($form_file->uri);
    $post_data['File1']  = new CurlFile($form_file_uri);
  }

  return $post_data;
}

/**
 * Sends an email to the user confirming ticket submission.
 *
 * @param array $form_state
 *   Associative array of information from trouble ticket form.
 * @param string $case_id
 *   Case number from FogBugz ticket creation xml response.
 */
function _trouble_tickets_get_fogbugz_email_data($form_state, $case_id) {
  $auth_user = _trouble_tickets_get_auth_user_info();
  $to = $auth_user['email_addr'];

  // Copy 'Assistant Director, Finance / Facilities' + 'Administrative Assistant' on
  // Public Machines + Library Maintenance ticket topics.
  if ($form_state['values']['ticket_topic'] == 'Library Maintenance and Operations') {
    $to .= '; Mario Tiozzo <mtiozzo@unb.ca>; Lisa Pollock <pollockl@unb.ca>';
  }

  $message = "The ticket below has been submitted to FogBugz. Your reference number for this ticket is $case_id.\n\n";
  $message .= 'Sender: ' . $to . "\n";
  $message .= 'Topic: ' . _trouble_tickets_get_topic_name($form_state['values']['ticket_topic']) . "\n";
  $message .= 'Category: ' . $form_state['values']['category'] . "\n";
  $message .= 'Title: ' . $form_state['values']['title'] . "\n";
  $message .= "Description:\n" . $form_state['values']['description'] . "\n";

  // Add file attachment info to message, if applicable.
  $form_fid = $form_state['values']['file'];
  if ($form_fid) {
    $form_file = file_load($form_fid);
    $message .= "\nAttachment: " . $form_file->filename;
  }

  // Add location info to message text, if applicable.
  $location = _trouble_tickets_get_location_string($form_state);
  if ($location) {
    $message .= $location;
  }

  $post_email_data = array(
    'ixBug' => $case_id,
    'sFrom' => 'libsystems@unb.ca',
    'sTo' => $to,
    'sSubject' => "Your trouble ticket has been submitted (Case $case_id)",
    'sEvent' => $message,
  );

  return $post_email_data;
}

/**
 * Determines appropriate ticket priority based on category.
 *
 * @param string $category
 *   Category of ticket.
 *
 * @return int
 *   Priority of ticket
 */
function _trouble_tickets_get_fogbugz_priority($category) {
  switch ($category) {
    case 'Bug':
      return 2;

    case 'Inquiry':
      return 3;

    case 'Feature Request':
      return 4;

    default:
      return 2;
  }
}

/**
 * Determines appropriate ticket priority based on category.
 *
 * @param string $topic
 *   Value of topic select option.
 *
 * @return string
 *   Display text of topic select option
 */
function _trouble_tickets_get_topic_name($topic) {
  $valid_topics = _trouble_tickets_get_topics();
  foreach ($valid_topics as $topic_set) {
    if (array_key_exists($topic, $topic_set)) {
      return $topic_set[$topic];
    }
  }
}

/**
 * Provides information re: authenticated user.
 *
 * @return array $user_info
 *   Associative array of first name, last name, UNB email address.
 */
function _trouble_tickets_get_auth_user_info() {
  // Construct UNB email address from Drupal authenticated user entity metadada.
  global $user;
  $auth_user = entity_metadata_wrapper('user', user_load($user->uid));

  // Use libsystems@unb.ca email address for Drupal 'admin' user.
  $user_name = ($auth_user->name->value() == 'admin') ? 'libsystems' : $auth_user->name->value();
  $user_info['first_name'] = ($auth_user->field_first_name->value() !== NULL) ? $auth_user->field_first_name->value() : '';
  $user_info['last_name']  = ($auth_user->field_last_name->value() != NULL) ? $auth_user->field_last_name->value() : '';
  $user_info['email_addr'] = trim("$user_info[first_name] $user_info[last_name] <$user_name@unb.ca>");

  return $user_info;
}

/**
 * Generates location string from trouble tickets location + room field values.
 *
 * @param array $form_state
 *   Trouble Ticket form state values.
 *
 * @return string $location
 *   Building, room location string
 */
function _trouble_tickets_get_location_string($form_state) {
  $location = $form_state['values']['location'];
  if ($form_state['values']['ticket_topic'] === 'Library Maintenance and Operations' && $location !== '') {
    switch ($form_state['values']['room']) {
      case '':
        $room = 'room not specified';
        break;

      case 'Media Storage Areas':
        $room = $form_state['values']['room'];
        break;

      default:
        $room = "Room " . $form_state[values][room];
    }
    $location = "\nLocation: $location, $room";

    return $location;
  }
  else {
    return;
  }
}
