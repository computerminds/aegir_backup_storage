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

  /**
   * Initialize the service along with the server object.
   */
  function init() {
    // REMEMBER TO CALL THE PARENT!
    parent::init();

    /**
     * We do not need to use this in our example.
     *
     * You would extend this if you needed to save values
     * for all possible implementations of this service type.
     */
  }

  function store_backup($filename) {
    return TRUE;
  }

  function retrieve_backup($filename) {
    return TRUE;
  }



}