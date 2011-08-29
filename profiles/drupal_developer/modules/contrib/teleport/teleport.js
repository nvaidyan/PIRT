// $Id: teleport.js,v 1.1.2.6 2008/05/16 23:50:53 incidentist Exp $
// JavaScript for Teleport module

Drupal.Teleport = {};

// Attach hidden Teleport button to DOM, and teleport DIV to body.
Drupal.Teleport.init = function() {
  if ( typeof(Drupal) == 'undefined' || typeof(Drupal.settings) == 'undefined' || typeof(Drupal.settings.teleport) == 'undefined') { return; } 
  var teleport = $("<div id='teleport'></div>");  
  teleport
    .html($(Drupal.settings.teleport.form))
    .click(function() {$('#edit-teleport-title').focus();});
    
  $("BODY")
    .append(teleport);
  teleport.hide();
  
  Drupal.Teleport.keycode = Drupal.settings.teleport.hotkey.charCodeAt(0);
  $(document).keydown(Drupal.Teleport.keyTrigger);
  
  if ($.browser.msie) {
    $('#teleport').css('position', 'absolute');
  }
  
  Drupal.behaviors.autocomplete('');
  
  $("#teleport-close-link")
    .attr('href', '#')
    .click(function(){ teleport.hide(); return false; });
};

Drupal.Teleport.toggle = function() {
  if ($('#teleport').css('display') == 'none') {
    Drupal.Teleport.appear();
  } else {
    Drupal.Teleport.vanish();
  }
};

Drupal.Teleport.vanish = function() {
  $('#teleport').fadeOut('fast');
  $(document).unbind('click', Drupal.Teleport.clickHandle);
  $(document).unbind('keydown', Drupal.Teleport.keyHandle);
};

Drupal.Teleport.appear = function() {
  $('#teleport').fadeIn('fast', Drupal.Teleport.focus);
  $(document).bind('click', Drupal.Teleport.clickHandle);
  $(document).bind('keydown', Drupal.Teleport.keyHandle);
};

Drupal.Teleport.focus = function() {
  // todo: this doesn't work on a page with another autocomplete called 'title'
  $('#edit-teleport-title').focus();
  $('#edit-teleport-title').select();
};


Drupal.Teleport.clickHandle = function (e) {
  if (!e) {
    var e = window.event;
  };
  // set tg to the clicked element
  if (e.target) var tg = e.target;
  else if (e.srcElement) var tg = e.srcElement;
  if (!Drupal.Teleport.checkParents(tg, 'teleport') && !Drupal.Teleport.checkParents(tg, 'teleport-button')) {
    Drupal.Teleport.vanish();
  }
};

/**
 * Handle any special keys when Teleport is active
 */
Drupal.Teleport.keyHandle = function (e) {
  if (!e) {
    e = window.event;
  }
  switch (e.keyCode) {
    case 27: // esc
      Drupal.Teleport.vanish();
      return false;
    default: // all other keys
      return true;
  }
};

/**
 * Handle the trigger key to activate Teleport
 */
Drupal.Teleport.keyTrigger = function (e) {
  if (!e) {
    e = window.event;
  }
  if (e.keyCode == Drupal.Teleport.keycode && e.ctrlKey) {
    Drupal.Teleport.toggle();
    return false;
  }
  else {
    return true;
  }
};

/**
 * Find out if the object or any of its parents has a given id
 * returns TRUE or FALSE
 */
Drupal.Teleport.checkParents = function (obj, id) {
  if (obj.id == id) {
    return true;
  }
  if (obj && obj.parentNode) {
    while (obj = obj.parentNode) {
      if (obj.id == id) {
        return true;
      }
    }
  }
  return false;
}
// 
// $(document).ready(function() {
//   Drupal.Teleport.keycode = Drupal.settings.teleport.keycode;
//   $(Drupal.Teleport.init);
// });

$(Drupal.Teleport.init);