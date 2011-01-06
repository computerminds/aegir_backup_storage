<?php
/**
 *   $Id$
 */

/**
 * @file
 *   Backup storage service implementation.
 */

/**
 * Hosting backup storage class that provides options for backup storage.
 *
 * The base service type class, registered with hook_hosting_service_type.
 */
class hostingService_backup_storage extends hostingService {
 /**
 *   the value stored in the service column of hosting_service table.
 */
  public $service = 'backup_storage';

  public $locations = array();

  public $configurable_locations = TRUE;

  /**
   * Return the locations in a string form for editing by the user.
   */
  function get_locations_string() {
    return '';
  }

  /**
   * Returns the backup locations in a pretty format.
   */
  function get_locations_pretty() {
    return '';
  }

  /**
   * Load associated values for the service.
   */
  function load() {
    // REMEMBER TO CALL THE PARENT!
    parent::load();
  }

  /**
   * Display settings on the server node page.
   *
   * Modify the reference passed to the method to add additional implementation
   * specific fields to be displayed.
   *
   * @param
   *   A reference to the associative array of the subsection of the page
   *   reserved for this service implementation.
   */
  function view(&$render) {
    // REMEMBER TO CALL THE PARENT!
    parent::view($render);
    if ($this->configurable_locations) {
      $render['backup_storage'] = array(
        '#type' => 'item',
        '#title' => t('Backup locations'),

        // Remember to pass the display through filter_xss!
        '#value' => $this->get_locations_pretty(),
      );
    }
  }

  /**
   * Extend the server node form.
   *
   * Modify the reference passed to the method to add additional implementation
   * specific fields to be stored and managed.
   *
   * @param
   *   A reference to the associative array of the subsection of the form
   *   reserved for this service implementation.
   */
  function form(&$form) {
    // REMEMBER TO CALL THE PARENT!
    parent::form($form);

    // TODO: Improve this UI.

    if ($this->configurable_locations) {
      $form['backup_storage_locations'] = array(
        '#type' => 'textarea',
        '#title' => t('Backup locations'),
        '#description' => t('A list of backup locations.'),
        //'#size' => 40,
        '#default_value' => $this->get_locations_string(),
        //'#maxlength' => 64,
        '#rows' => 5,
        //'#weight' => 5,
      );
    }
  }

  /**
   * Validate a form submission.
   */
  function validate(&$node, &$form) {
    // REMEMBER TO CALL THE PARENT!
    parent::validate($node, $form);

  }

  /**
  * Insert a record into the database.
  *
  * Called by hosting_server_hook_insert().
  *
  * The values associated with this implementation have already
  * been set as properties of $this object, so we now need to
  * save them.
  */
  function insert() {
    // REMEMBER TO CALL THE PARENT!
    parent::insert();
    $this->update_actual();
  }

  function update() {
    if (isset($this->backup_storage_locations)) {
      $this->delete_revision();
      $this->insert();
    }
  }

  function update_actual() {

  }

  function delete() {
    // REMEMBER TO CALL THE PARENT!
    parent::delete();

  }

  /**
   * Delete a specific reivision from the database.
   */
  function delete_revision() {
    // REMEMBER TO CALL THE PARENT!
    parent::delete_revision();

  }
}


/**
 * An implementation of the backup_storage service type, registered with hook_hosting_service.
 */
class hostingService_backup_storage_s3 extends hostingService_backup_storage {
 /**
  *   the value stored in the type column of the hosting_service table.
  */
  public $type = 's3';

  function load() {
    // REMEMBER TO CALL THE PARENT!
    parent::load();
    $result = db_query("SELECT location_id, location_name, aws_key, aws_private, aws_bucket FROM {hosting_backup_storage_s3} WHERE vid = %d", $this->server->vid);
    while ($row = db_fetch_array($result)) {
      $this->locations[$row['location_id']] = $row;
    }
  }

  function form(&$form) {
    // REMEMBER TO CALL THE PARENT!
    parent::form($form);

    $form['backup_storage_locations']['#description'] = t('A list of backup locations in the following format:<br />Machine name|Friendly name|AWS Key|AWS Private|Bucket name');
  }

  function validate(&$node, &$form) {
    // REMEMBER TO CALL THE PARENT!
    parent::validate($node, $form);

    // Explode the locations:
    $this->backup_storage_locations = explode("\n", $this->backup_storage_locations);
    foreach ($this->backup_storage_locations as $location) {
      $location = explode('|', $location);
      if (count($location) < 5) {
        form_set_error('backup_storage_locations', t("Not enough options given for the backup location"));
      }
      if (count($location) < 5) {
        form_set_error('backup_storage_locations', t("Too many options given for the backup location"));
      }

      // TODO: More checking!

    }
  }

  function update_actual() {
    if (isset($this->backup_storage_locations)) {
      db_query('DELETE FROM {hosting_backup_storage_s3} WHERE vid = %d', $this->server->vid);

      $locations = explode("\n", $this->backup_storage_locations);
      foreach ($locations as $location) {
        $location = explode('|', trim($location));
        $location = array_merge(array($this->server->nid, $this->server->vid), $location);
        db_query("INSERT INTO {hosting_backup_storage_s3} (nid, vid, location_id, location_name, aws_key, aws_private, aws_bucket) VALUES (%d, %d, '%s', '%s', '%s', '%s', '%s')", $location);
      }
    }
  }

  function delete() {
    // REMEMBER TO CALL THE PARENT!
    parent::delete();

    db_query('DELETE FROM {hosting_backup_storage_s3} WHERE nid = %d', $this->server->nid);
  }

  /**
   * Delete a specific reivision from the database.
   */
  function delete_revision() {
    // REMEMBER TO CALL THE PARENT!
    parent::delete_revision();

    db_query('DELETE FROM {hosting_backup_storage_s3} WHERE vid = %d', $this->server->vid);
  }

  function get_locations_string() {
    $locs = array();
    foreach ($this->locations as $location) {
      $locs[] = implode('|', array(
        $location['location_id'],
        $location['location_name'],
        $location['aws_key'],
        $location['aws_private'],
        $location['aws_bucket'],
      ));

    }
    return implode("\n", $locs);
  }

  function get_locations_pretty() {
    $locs = array();
    foreach ($this->locations as $location) {
      $locs[] = check_plain($location['location_name']);
    }

    $output = implode(', ', $locs);
    if (empty($output)) {
      $output = theme('placeholder', t('None'));
    }
    return $output;
  }
}
