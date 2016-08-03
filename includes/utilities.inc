<?php

/**
 * @file
 * Utilities.
 */

/**
 * Returns a list of content models that this module supports.
 *
 * @return array
 *   A list of supported content model PIDs.
 */
function islandora_dump_datastreams_get_supported_cmodels() {
  return array(
    'islandora:bookCModel',
    'islandora:compoundCModel',
    'islandora:newspaperIssueCModel',
    'islandora:sp-audioCModel',
    'islandora:sp_basic_image',
    'islandora:sp_disk_image',
    'islandora:sp_large_image_cmodel',
    'islandora:sp_pdf',
    'islandora:sp_videoCModel',
    'islandora:sp_web_archive',
  );
}

/**
 * Populates a list of cmodels for use in the admin setting form.
 *
 * Filters out content models that are not installed.
 *
 * @return array
 *   A list of supported content models that are currently installed.
 */
function islandora_dump_datastreams_get_cmodels() {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $cmodels = islandora_get_content_models();
  $supported_cmodels = islandora_dump_datastreams_get_supported_cmodels();
  $options = array();
  foreach ($cmodels as $cmodel) {
    $pid = $cmodel['pid'];
    if (in_array($pid, $supported_cmodels)) {
      $options[$pid] = $cmodel['label'];
    }
  }
  return $options;
}

/**
 * Returns a list of DSIDs that we don't want to dump.
 *
 * @return array
 *   A list of DSIDs that we don't want to dump.
 */
function islandora_dump_datastreams_dsids_to_skip() {
  $skip = variable_get('islandora_dump_datastreams_skip_dsids', 'RELS-EXT');
  $skip = explode(',', $skip);
  $dsids_to_skip = array();
  foreach ($skip as &$dsid) {
    $dsid = trim($dsid);
    $dsids_to_skip[] = $dsid;
  }
  return $dsids_to_skip;
}

/**
 * Creates the object-level output directory for non-compound objects.
 *
 * E.g., image, PDF, video, paged content.
 *
 * @param AbstractObject $object
 *   The object that has just been ingested.
 */
function islandora_dump_datastreams_dump_single(AbstractObject $object) {
  $base_output_dir = variable_get('islandora_dump_datastreams_output_dir', '/tmp');
  $dsids_to_skip = islandora_dump_datastreams_dsids_to_skip();
  if (!is_writable($base_output_dir)) {
    return;
  }
  foreach ($object as $ds) {
    if (!in_array($ds->id, $dsids_to_skip)) {
      $extension = islandora_dump_datastreams_get_extension($ds->id, $ds->mimetype);
      $filename = $ds->id . '.' . $extension;

      $object_dir_name = preg_replace('/:/', '_', $object->id);
      $output_dir = $base_output_dir . DIRECTORY_SEPARATOR . $object_dir_name;
      if (!file_exists($output_dir)) {
        // The default permissions assigned by mkdir() are pretty loose but
        // this is probably not a huge security risk. There is a note in the
        // admin form indicating this.
        mkdir($output_dir);
      }
      $ds_output_path = $output_dir . DIRECTORY_SEPARATOR . $filename;

      if (!file_exists($ds_output_path)) {
        $ds->getContent($ds_output_path);
      }
    }
  }
  islandora_dump_datastreams_log_object_dump($object->id, $output_dir);
}

/**
 * Creates the object-level output directory for newspaper issue objects.
 *
 * @param AbstractObject $object
 *   The object that has just been ingested.
 */
function islandora_dump_datastreams_dump_newspaper(AbstractObject $object) {
  /*
  @todo: Get pages and write them out in the structure required by
  the Newspaper Batch module:

  my_batch/
      issue1/
          MODS.xml
          1/
              OBJ.tiff
              JP2.jp2
              OCR.txt
              TN.jpg
          2/
              OBJ.tiff
              JP2.jp2
              OCR.txt
              TN.jpg
  */
}

/**
 * Creates the object-level output directory for book objects.
 *
 * @param AbstractObject $object
 *   The object that has just been ingested.
 */
function islandora_dump_datastreams_dump_book(AbstractObject $object) {
  /*
  @todo: Get pages and write them out in the structure required by
  the Book Batch module:

  /tmp/batch_ingest/
   └── book/
        ├── 1/
        │   └── OBJ.tif
        ├── 2/
        │   ├── OBJ.tif
        │   ├── OCR.txt
        │   ├── MODS.xml
        │   └── HOCR.shtml
        └── MODS.xml
  */
}

/**
 * Writes a datastream file to the output directory.
 *
 * @param AbstractObject $object
 *   The object that has just been ingested.
 * @param AbstractDatastream $datastream
 *   The datastream that has just been ingested.
 */
function islandora_dump_datastreams_dump_datastream(AbstractObject $object, AbstractDatastream $datastream) {
  $base_output_dir = variable_get('islandora_dump_datastreams_output_dir', '/tmp');
  $dsids_to_skip = islandora_dump_datastreams_dsids_to_skip();
  if (in_array($datastream->id, $dsids_to_skip)) {
    return;
  }
  if (!is_writable($base_output_dir)) {
    return;
  }

  $extension = islandora_dump_datastreams_get_extension($datastream->id, $datastream->mimetype);
  $filename = $datastream->id . '.' . $extension;

  $object_dir_name = preg_replace('/:/', '_', $object->id);
  $output_dir = $base_output_dir . DIRECTORY_SEPARATOR . $object_dir_name;
  if (!file_exists($output_dir)) {
    // Loose permissions but probably not a huge security risk. There is a
    // note in the admin form indicating this.
    mkdir($output_dir);
  }
  $ds_output_path = $output_dir . DIRECTORY_SEPARATOR . $filename;

  if (!file_exists($ds_output_path)) {
    $datastream->getContent($ds_output_path);
  }
}

/**
 * Logs the dump to the watchdog.
 *
 * @param string $dsid
 *   The DSID of the datastream.
 * @param string $mimetype
 *   The MimeType of the datastream.
 *
 * @return string
 *   The extension to assign to the datastream's file, without the period.
 */
function islandora_dump_datastreams_get_extension($dsid, $mimetype) {
  if ($dsid == 'DC') {
    // To prevent getting .bin.
    $extension = 'xml';
  }
  else {
    $mime_detect = new MimeDetect();
    $extension = $mime_detect->getExtension($mimetype);
  }
  return $extension;
}

/**
 * Logs the dump to the watchdog.
 *
 * @param string $pid
 *   The PID of the object that has just been dumped.
 * @param string $path
 *   The path the object was dumpted to.
 */
function islandora_dump_datastreams_log_object_dump($pid, $path) {
  if (variable_get('islandora_dump_datastreams_log_dumps', 0)) {
    watchdog('islandora_dump_datastream', 'Object !pid dumped to !path.',
      array('!pid' => $pid, '!path' => $path));
  }
}
