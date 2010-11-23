<?php

/**
 * Implementation of drush_hook_post_provision_backup.
 *
 * We can now perform such operations as restarting the server,
 * by calling the public API of the service we have created.
 */
function drush_backup_storage_post_provision_backup() {
  if (d()->type == 'site') {
    $backup_file = drush_get_option('backup_file');
    d()->service('backup_storage')->store_backup($backup_file);
  }
}