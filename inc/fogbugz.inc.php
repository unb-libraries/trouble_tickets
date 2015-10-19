<?php
/**
 * @file
 * Provides an interface to FogBugz via the Fogbugz API.
 */
/**
 * Generate and return FogBugz token.
 *
 * Authenticates Mr. Robot through the FogBugz API and generates a token for
 * subsequent API calls.
 *
 * @return string
 *   Token for FogBugz API calls
 */


/**
 * Authenticates Mr. Robot through the FogBugz API and generates a token for subsequent API calls.
 *
 * @return string
 *   Token for FogBugz API calls
 */
function _trouble_tickets_get_fogbugz_token() {

  $curl_handle = curl_init('https://support.lib.unb.ca/api.asp?cmd=logon');

  curl_setopt($curl_handle, CURLOPT_RETURNTRANSFER, TRUE);
  curl_setopt($curl_handle, CURLOPT_POST, TRUE);
  $ch_post_data = array(
    'email' => variable_get('trouble_tickets_fogbugz_email'),
    'password' => variable_get('trouble_tickets_fogbugz_password'),
  );
  curl_setopt($curl_handle, CURLOPT_POSTFIELDS, $ch_post_data);

  $curl_response = curl_exec($curl_handle);
  $token = NULL;
  if ($curl_response) {
    $xml = simplexml_load_string($curl_response, 'SimpleXMLElement', LIBXML_NOCDATA);
    $token = (string) $xml->token;
  }
  curl_close($curl_handle);

  return $token;

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
function _trouble_tickets_create_fogbugz_ticket($token, $form_state) {

  $ticket_information = array(
    'project' => $form_state['values']['ticket_topic'],
    'title' => $form_state['values']['title'],
    'category' => $form_state['values']['category'],
    'description' => $form_state['values']['description'],
  );
  $ticket_information['priority'] = _get_priority($ticket_information['category']);

  // Get Drupal username append "@unb.ca" to form customer email.
  global $user;
  $active_user = user_load($user->uid);
  $username = $active_user->name;
  $username = $username == 'admin' ? 'libsystems' : $username;
  $firstname = isset($active_user->field_first_name['und']['0']['value']) ? $active_user->field_first_name['und']['0']['value'] : '';
  $lastname = isset($active_user->field_last_name['und']['0']['value']) ? $active_user->field_last_name['und']['0']['value'] : '';

  if ($firstname != '' && $lastname != '') {
    $ticket_information['email'] = $firstname . " " . $lastname . " <" . $username . '@unb.ca>';
  }
  else {
    $ticket_information['email'] = $username . '@unb.ca';
  }

  if ($ticket_information['project'] == 'Public Machines / Printers' || $ticket_information['project'] == 'Library Maintenance and Operations') {
    $ticket_information['email'] .= '; Alicia McLaughlin <amclaugh@unb.ca>';
  }

  // If topic is Library Maintenance, add location to description
  if ($form_state['values']['ticket_topic'] == 'Library Maintenance and Operations') {
    $location = '';
    if (isset($form_state['values']['building']) && $form_state['values']['building'] != 'Not Applicable') {
      $location .= $form_state['values']['building'] . ', ';
      if (isset($form_state['values']['room']) && $form_state['values']['room'] != 'Not Applicable') {
        $location .= $form_state['values']['room'];
      }
      else {
        $location .= 'room not specified';
      }
    }

    if ($location != '') {
      $ticket_information['description'] .= "\n\nLocation: " . $location;
    }
  }

  $sEvent = "Submitted by: " . $firstname . " " . $lastname . "\n\n" . $ticket_information['description'];

  $curl_handle = curl_init('https://support.lib.unb.ca/api.asp?cmd=new');
  curl_setopt($curl_handle, CURLOPT_RETURNTRANSFER, TRUE);
  curl_setopt($curl_handle, CURLOPT_POST, TRUE);
  $ch_post_data = array(
    'token' => $token,
    'sProject' => $ticket_information['project'],
    'sTitle' => $ticket_information['title'],
    'sCategory' => $ticket_information['category'],
    'sEvent' => $sEvent,
    'ixPriority' => $ticket_information['priority'],
    'sTags' => 'trouble_ticket',
    'sCustomerEmail' => $ticket_information['email'],
    'ixMailbox' => 1,
  );
  curl_setopt($curl_handle, CURLOPT_POSTFIELDS, $ch_post_data);
  // testing - don't create ticket
  print_r($ch_post_data);
  exit;

  $curl_response = curl_exec($curl_handle);
  $success = FALSE;
  $case_id = NULL;
  if ($curl_response) {
    $xml = simplexml_load_string($curl_response, 'SimpleXMLElement', LIBXML_NOCDATA);
    $case_id = (string) $xml->case['ixBug'];
    if ($case_id != NULL) {
      $ticket_information['case_id'] = $case_id;
      $success = TRUE;
      _trouble_tickets_send_fogbugz_confirmation_email($token, $ticket_information);
    }
  }
  curl_close($curl_handle);

  return $success;
}

/**
 * Sends an email to the user confirming ticket submission.
 *
 * @param string $token
 *   Token for FogBugz API calls
 * @param array $ticket_information
 *   Associative array of information from the newly submitted ticket
 */
function _trouble_tickets_send_fogbugz_confirmation_email($token, $ticket_information) {
  $message = "The ticket below has been submitted to FogBugz. Your reference number for this ticket is " . $ticket_information['case_id'] . ".\n\n";
  $message .= "Topic: " . _get_topic_name($ticket_information['project']) . "\n";
  $message .= "Category: " . $ticket_information['category'] . "\n";
  $message .= "Title: " . $ticket_information['title'] . "\n";
  $message .= "Description: " . $ticket_information['description'] . "\n";

  $to = $ticket_information['email'];

  if ($ticket_information['project'] == 'Library Maintenance and Operations') {
    $to .= '; mtiozzo@unb.ca';
  }

  $curl_handle = curl_init('https://support.lib.unb.ca/api.asp?cmd=forward');
  curl_setopt($curl_handle, CURLOPT_RETURNTRANSFER, TRUE);
  curl_setopt($curl_handle, CURLOPT_POST, TRUE);
  $ch_post_data = array(
    'token' => $token,
    'ixBug' => $ticket_information['case_id'],
    'sFrom' => 'libsystems@unb.ca',
    'sTo' => $to,
    'sSubject' => 'Your trouble ticket has been submitted (Case ' . $ticket_information['case_id'] . ')',
    'sEvent' => $message,
  );
  curl_setopt($curl_handle, CURLOPT_POSTFIELDS, $ch_post_data);
  curl_exec($curl_handle);
  curl_close($curl_handle);
}
