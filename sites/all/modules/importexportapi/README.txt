
Readme
------

The Import / Export API allows for the definition of data entities within
Drupal. The definition system lets data entities be related to each other,
and it includes a number of different field types. It also supports a system
of import (parsing) and export (rendering) engines, for formats such as XML
and CSV. Data entities are able to be exported from, and imported into, a
Drupal site, for purposes such as backup, offline editing, data migration,
and data transfer between Drupal databases.


Installation
------------

1.  Extract the package into your site's '/modules' directory.
2.  Enable the importexportapi and importexportui modules at 'administer ->
    modules'.
3.  Configure access privileges at 'administer -> access control'.


Performing imports and exports
------------------------------

A simple UI module, called 'importexportui', is currently included with the
API. To start performing data exports, go to 'administer -> export' and follow
the wizard on that page. To start performing data imports, go to 'administer ->
import', and follow the wizard on that page.

All data for importing is currently stored in a series of files in the
'/testdata' directory of the package. To add or edit the available import data,
simply edit the files in this directory, or add new files of your own (no
configuration needed - all files in this directory are made available for
import).


Support
-------

All issues (e.g. support requests, bug reports) are to be submitted using the
drupal.org issue tracker.


Author
------

Jeremy Epstein <jazepstein [ATT] greenash DOTT net DOTT au>
