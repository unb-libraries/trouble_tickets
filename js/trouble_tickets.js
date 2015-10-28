/*global jQuery, document, window*/

jQuery(document).ready(function () {
    // Emulate old AJAX Trouble Tickets behaviours using jQuery 1.6+.
    jQuery('.form-item-ticket-topic select').change(function () {
        // New Feature Request selected hide 'Category' container and default to 'Feature Request' radio.
        if (this.value.indexOf('New Systems') > -1) {
            jQuery('input[value="Feature Request"]').prop('checked', true);
            jQuery('.form-item-category').hide();
        } else {
            jQuery('.form-item-category').show();
        }

        // Repairs / Requests selected alter radio input labels.
        if (this.value.indexOf('Library Maintenance') > -1) {
            jQuery('input[value="Bug"] + label').text('Repair');
            jQuery('input[value="Feature Request"] + label').text('Request');
            jQuery('.form-item-building').show();
        } else {
            jQuery('input[value="Bug"] + label').text('Bug');
            jQuery('input[value="Feature Request"] + label').text('Feature Request');
            jQuery('.form-item-building').hide();
            jQuery('select[name="building"] option:first').prop('selected', true);
        }
    });
    jQuery('html, body').animate({scrollTop : 0}, 'fast');
});
