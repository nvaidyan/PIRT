ASU Webauth Drupal module
- Version: 6.x-2.x branch
- Author: Jeff Beeman (UTO Web Consulting Services)

Please report all issues to webconsulting@asu.edu


DESCRIPTION
------------------------
A module that provides sign-in capabilities to Drupal through ASU Webauth. 

Contains settings for defining auto-registration behavior, which pages to require webauth for and more.


REQUIREMENTS
------------------------
* Drupal 6.x
* Has only been tested and verified on 
  - PHP 5.2.x
  - Drupal 6.15

This new version /no longer requires/ the Webauth Verify client.

While it hasn't been tested, the module /should/ work on Windows. If you happen to test and verify this, please let us know.


UPGRADING FROM 6.x-1.x to 6.x-2.x
------------------------
If you are upgrading from an earlier release, do the following /before/ adding the new module to your site:

* If you are currently redirecting on specific paths, note the paths that you have set. Due to an architectural change in the updated module, this setting will not port over to the new version.

* Note the new setting for appending @ASU.EDU to all usernames. If you enable this setting, all Webauth users will will have a username like asurite@ASU.EDU. /This could result in duplicate accounts/ so enable it with caution.

* After upgrading, clear your cache at /admin/settings/performance.

* You do /not/ need to run update.php when upgrading the module.


INSTALLATION
------------------------
* Before proceeding with installation, it is recommended that you setup a user, generally the #1 user in the site (uid=1) to have a user name that is /not/ a webauth user name (i.e. site.administrator).  This will allow you to login to the site even if webauth is down or if the webauth module is badly configured.

* Place the asu_webauth folder into an appropriate location on the server for modules (i.e. /sites/all/modules/asu_webauth)

* Enable the asu_webauth module, which can be found on admin/build/modules under the heading "ASU"

* Configure the module at admin/settings/asu_webauth (details below)

* Disable the standard Drupal "Login" and "Logout" links at admin/build/menus


CONFIGURATION
------------------------
* Setup the role that webauth users should receive when becoming a user of the site.  It is highly recommended that you make this role /not/ 'authenticated user', as it will make every user, even site admins, have to be webauth'd in order to maintain a session.  It is generally recommended to setup a new role called 'webauth user' and configure the module to automatically assign that role to any Webauth'd users.

* Go to admin/settings/asu_webauth to configure the module

* The module can be configured to automatically register users that login to the site via webauth.  Turn this on if you would like all users to automatically  become local Drupal account holders.  Otherwise, keep this disabled and manually add user IDs via admin/user/user/create.

* You can require Webauth login for no pages, all pages, specified pages or all pages except specified pages, the choice is  yours.  Wildcards are permitted for the "specific pages" settings.  If "no pages" or "all pages" is set, the values set in "specific pages" will be ignored.
