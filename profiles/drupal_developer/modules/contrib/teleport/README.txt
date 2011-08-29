Teleport module
Created in 2008 by Dan Kurtz (incidentist)
Licensed under the GPL

This is the Drupal 6 version of Teleport.

This module is intended for administrators who have to jump around to specific pages while managing Drupal sites. It's based off of the Quicksilver launching utility.

INSTALLATION

Unzip the module into your modules directory, enable it, and set the appropriate permissions in admin/user/access.

USAGE

Pressing Ctrl-M brings up the dialog (the hotkey can be customized on the settings page). Type in any part of the title or path of the page, scroll down to the page you're looking for, and hit the Teleport button. Unlike many other dynamic search modules, Teleport searches menu items as well as node titles. In the future, it will be configurable and hook into more content, such as api calls. 

If you don't want to wait for the autocompletions, type in where you want to go and hit Teleport. The module will try to figure out what you meant, and if it can't figure it out, it'll give you a page with a list of options.

TODO:

* Make interface act more like Quicksilver (i.e. you should only have to press Enter once)
* 'Actions' like Quicksilver: if you select a node, a second input should appear with options to go to the View page, Edit page, (un)publish, etc. Same with users.
* Hook into more non-node content, like users and functions in the API module.
