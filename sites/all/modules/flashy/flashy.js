// $Id: flashy.js,v 1.2 2009/03/19 19:52:50 crell Exp $
Drupal.behaviors.flashyInit = function (context) {
  
  var settings = Drupal.settings.flashy;
  
  $('.flashy:not(.flashyInit-processed)', context).addClass('flashyInit-processed').each(function () {
    var config = settings['files'][$(this).attr('id')];
    
    // Create the new player.
    var so = new SWFObject(settings.player, $(this).attr('id') + '-player', config.width, config.height, "9.0.28.0", config.background);

    // Add any parameters defined on the player for the HTML.
    for (var param in config.params) {
      so.addParam(param, config.params[param]);
    }
    // Add any flash variables defined on the player.
    for (var flashVar in config.flashVars) {
      so.addVariable(flashVar, config.flashVars[flashVar]);
    }
    // Attach the player to the page
    so.write($(this).attr('id'));
  });
};

