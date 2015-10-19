/*global jQuery, document, window*/

jQuery(document).ready(function () {
    // Emulate old AJAX Trouble Tickets behaviours using jQuery 1.6+.
    jQuery('.form-item-ticket-topic select').change(function () {
        // Redirect select items tagged as 'eResource' to external lib.unb.ca form.
        if (this.value.indexOf('eResources') > -1) {
            window.location = 'http://www.lib.unb.ca/help/troubleticket.php';
        } else {
            // Update legend text on select option when not redirecting.
            var legend = 'Trouble tickets for ';
            legend += jQuery('.form-item-ticket-topic select option:selected').text();
            jQuery('span.fieldset-legend').text(legend);
        }

       // New Feature Request selected hide 'Category' container and default to 'Feature Request' radio.
       if (this.value.indexOf('New Systems') > -1) {
         jQuery('input[value="Feature Request"]').prop('checked', true);
       }

        // Repairs / Requests selected alter radio input labels.
        if (this.value.indexOf('Library Maintenance') > -1) {
            jQuery('input[value="Bug"] + label').text('Repair');
            jQuery('input[value="Feature Request"] + label').text('Request');
        } else {
            jQuery('input[value="Bug"] + label').text('Bug');
            jQuery('input[value="Feature Request"] + label').text('Feature Request');
        }

        // Handle legend arrow toggle icons, make form visible on select.
        jQuery('fieldset.form-wrapper').addClass('wrapper-expanded').removeClass('wrapper-collapsed');
        jQuery('.fieldset-wrapper').slideDown('fast');
    });
});
