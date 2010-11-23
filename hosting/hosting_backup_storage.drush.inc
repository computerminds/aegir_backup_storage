<?php


/**
 * Implementation of drush_hook_pre_hosting_task()
 */
function hosting_backup_storage_hosting_site_context_options(&$task) {
  $task->context_options['backup_storage_server'] = hosting_context_name($task->ref->backup_storage_server);
  $task->context_options['backup_storage_location_id'] = $task->ref->backup_storage_location_id;
}

function hosting_backup_storage_hosting_server_context_options(&$task) {
  $task->context_options['backup_storage_locations'] = $task->ref->services['backup_storage']->locations;
}