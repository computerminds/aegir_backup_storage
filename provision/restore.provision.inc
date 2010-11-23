<?php

/**
 * Implementation of drush_hook_pre_provision_restore.
 *
 * We can now perform such operations as restarting the server,
 * by calling the public API of the service we have created.
 */
function drush_backup_storage_provision_restore_validate($restore_file = NULL) {
  if (d()->type == 'site') {
    d()->service('backup_storage')->retrieve_backup($restore_file);
  }
}