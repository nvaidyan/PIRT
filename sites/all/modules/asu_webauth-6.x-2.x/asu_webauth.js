Drupal.asu_webauth = {};

Drupal.asu_webauth.initialize = function() {
    var ssi = $('#asu_hdr_ssi a');
    var sso = $('#asu_hdr_sso a');
    
    if (ssi) {
        url = Drupal.settings.basePath + 'asuwebauth-login';
        if (Drupal.settings.asu_webauth.callapp) {
            url += '?destination=' + Drupal.settings.asu_webauth.callapp;
        }
        ssi.attr('href', url);
    }
    
    if (sso) {
        url = Drupal.settings.basePath + 'asuwebauth-logout'
        if (Drupal.settings.asu_webauth.onLogoutURL) {
            url += '?destination=' + Drupal.settings.asu_webauth.onLogoutURL;
        }
        sso.attr('href', url);
    }
}

if (Drupal.jsEnabled) {
  $(document).ready(function() {
    Drupal.asu_webauth.initialize();
  });
}

