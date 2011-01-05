<?php

// $Id$

 /**
  * @file
  *   A 's3' implementation of the 'backup_storage' service type.
  */

include_once(dirname(__FILE__) . '/includes/S3.php');

/**
 * A class containing the 's3' implementation of the 'backuo_storage' service.
 */
class provisionService_backup_storage_s3 extends provisionService_backup_storage {

  /**
   * Create a new S3 class, checks to make sure that the bucket exists.
   */
  function s3_bucket_factory($spec = array()) {
    $s3 = new S3($spec['aws_key'], $spec['aws_private']);
    list($bucket_name, ) = explode('/', $spec['aws_bucket'], 2);
    if ($s3->getBucket($bucket_name) !== FALSE) {
      return $s3;
    }

    // Otherwise the bucket did not exist.
    return FALSE;
  }

  /**
   * Compute the remote location of the given filename.
   *
   * In the future this could do recursive scans for files in S3.
   */
  function s3_compute_bucket_name_and_uri($location, $filename) {
    $return = array(
      'bucket_name' => '',
      'uri' => '',
    );

    // Allow for [uri] being replaced with the current site URI.
    $location['aws_bucket'] = str_replace('[uri]', d()->uri, $location['aws_bucket']);
    list($return['bucket_name'], $return['uri']) = explode('/', $location['aws_bucket'], 2);
    $return['uri'] = rtrim($return['uri'], '/');
    $return['uri'] .= '/' . basename($filename);
    $return['uri'] = ltrim($return['uri'], '/');

    return $return;
  }

  /**
   * Store the given file into S3 for real.
   */
  function store_object($location_id, $filename) {
    $location = $this->get_location($location_id);

    if ($s3 = $this->s3_bucket_factory($location)) {
      $target = s3_compute_bucket_name_and_uri($location, $filename);
      return $s3->putObject(S3::inputFile($filename), $target['bucket_name'], $target['uri']);
    }

  }

  /**
   * Get the given file from S3 for real.
   */
  function get_object($location_id, $filename) {
    $location = $this->get_location($location_id);

    if ($s3 = $this->s3_bucket_factory($location)) {
      $target = s3_compute_bucket_name_and_uri($location, $filename);
      return $s3->getObject($target['bucket_name'], $target['uri'], $filename);
    }
  }

  /**
   * Delete the given file from S3 for real.
   */
  function remove_object($location_id, $filename) {
    $location = $this->get_location($location_id);

    if ($s3 = $this->s3_bucket_factory($location)) {
      $target = s3_compute_bucket_name_and_uri($location, $filename);
      return $s3->deleteObject($target['bucket_name'], $target['uri']);
    }

  }

}
