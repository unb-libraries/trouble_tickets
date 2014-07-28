<div id="ajax-status-messages-wrapper"></div>
<?php 
print (drupal_render($ticket_form));
drupal_add_js('
  jQuery(document).ready(function() { 
    jQuery("#topic-submit").remove();
  });', 'inline');
drupal_add_css('
  #content h2 {
    margin-top: 0;
  }
  .page-trouble-tickets .tabs {
    margin: 0;
  }
  #trouble-tickets-form p {
    margin: 1em 0 0 0;
  }
  #trouble-tickets-form .container-inline {
    margin-bottom: 10px;
  }
  #trouble-tickets-form input.form-radio {
    margin-left: 20px;
  }', 'inline');
?>
