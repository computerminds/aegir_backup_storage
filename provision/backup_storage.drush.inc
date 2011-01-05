<?php


/**
 * Expose the service type this extension defines to provision.
 *
 * Implementations are then conditionally loaded when the option
 * "--$service_service_type=$type" is passed to the provision-save
 * command of a server.
 *
 * Implementations are automatically loaded from :
 * provision/$service/$type/$type_service.inc.
 *
 * @return
 *   An array with the service type the key, and the default implementation the value.
 */
function backup_storage_provision_services() {
  return array('backup_storage' => NULL);
}

/**
 * The service type base class.
 *
 * All implementations of the service type will inherit this class.
 * This class should define the 'public API' to be used by the rest
 * of the system, which should not expose implementation details.
 */
class provisionService_backup_storage extends provisionService {
  public $service = 'backup_storage';

  static function subscribe_server($context) {
    $context->setProperty('backup_storage_locations', array());
  }

  static function subscribe_site($context) {
    $context->setProperty('backup_storage_server', '');
    $context->is_oid('backup_storage_server');
    $context->setProperty('backup_storage_location_id', '');
  }

  /**
   * Store a given filename in the correct backup storage backend.
   */
  public function store_backup($filename) {
    if (d()->backup_storage_server->service('backup_storage')->store_object(d()->backup_storage_location_id, $filename)) {
      return TRUE;
    }

  }

  /**
   * Delete a given filename from the correct backup storage backend.
   */
  public function delete_backup($filename) {
    if (d()->backup_storage_server->service('backup_storage')->remove_object(d()->backup_storage_location_id, $filename)) {
      return TRUE;
    }

  }

  /**
   * Get a given filename from the correct backup storage backend.
   */
  public function retrieve_backup($filename) {
    if (!file_exists($filename)) {
      // Try to get the file from S3
      if (d()->backup_storage_server->service('backup_storage')->get_object(d()->backup_storage_location_id, $filename)) {
        return TRUE;
      }

    }
    return FALSE;
  }

  /**
   * Get a stored backup location.
   *
   * This should only ever be called on the service when attached to a server.
   */
  protected function get_location($location_id) {
    if (isset($this->context->backup_storage_locations[$location_id])) {
      return $this->context->backup_storage_locations[$location_id];
    }
    else {
      return FALSE;
    }
  }

  /**
   * Override this method in subclasses to provide the actual storage functionality.
   */
  protected function store_object($location_id, $filename) {
    return FALSE;
  }

  /**
   * Override this method in subclasses to provide the actual storage functionality.
   */
  protected function remove_object($location_id, $filename) {
    return FALSE;
  }

  /**
   * Override this method in subclasses to provide the actual storage functionality.
   */
  protected function get_object($location_id, $filename) {
    return FALSE;
  }

}
