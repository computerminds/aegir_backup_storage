<?php

function drush_backup_storage_pre_provision_backup_delete($backup = null) {
  if ($backup) {
    $backups = explode(',', $backup);
    foreach ($backups as $filename) {
      if (d()->type == 'site') {
        d()->service('backup_storage')->retrieve_backup($filename);
      }
    }
  }
}

function drush_backup_storage_post_provision_backup_delete($backup = null) {
  if ($backup) {
    $backups = explode(',', $backup);
    foreach ($backups as $filename) {
      if (d()->type == 'site') {
        d()->service('backup_storage')->delete_backup($filename);
      }
    }
  }
}