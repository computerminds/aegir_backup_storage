<?php
/**
 *   $Id$
 */

/**
 * @file
 *   Example service implementation for the hosting front end.
 */

/**
 * The base service type class , registered with hook_hosting_service_type.
 */
class hostingService_backup_storage extends hostingService {
 /**
 *   the value stored in the service column of hosting_service table.
 */
  public $service = 'backup_storage';
}



/**
 * An implementation of the backup_storage service type, registered with hook_hosting_service.
 */
class hostingService_backup_storage_s3 extends hostingService_backup_storage {
 /**
  *   the value stored in the type column of the hosting_service table.
  */
  public $type = 's3';
}