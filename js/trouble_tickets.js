/*global jQuery, document, window*/

jQuery(document).ready(function () {
    jQuery('html, body').animate({scrollTop : 0}, 'fast');
});

jQuery(document).ajaxSuccess(function() {
    // toggle expanding/collapsing fieldset content via legend click
    jQuery("fieldset legend").click(function() {
        jQuery(this).parent().toggleClass("wrapper-expanded wrapper-collapsed");
        jQuery(this).next(".fieldset-wrapper").slideToggle("fast");
    });
});
