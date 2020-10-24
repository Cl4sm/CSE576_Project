rbin_struct *get_record_data (FILE     *inf,
                              gboolean  erraneous)
{
  uint64_t version, win_filetime;
  rbin_struct *record;
  time_t file_epoch;
  gunichar2 ucs2_filename[0x104];
  GError *error = NULL;

  record = g_malloc0 (sizeof (rbin_struct));

  fread (&version, 8, 1, inf); /* useless for now */

  /*
   * In rare cases, the size of index file is 543 bytes versus (normal) 544 bytes.
   * In such occasion file size only occupies 56 bit, not 64 bit as it ought to be.
   * Actually this 56-bit file size is very likely misleading.
   */
  if (erraneous)
    fread (&record->filesize, 7, 1, inf);
  else
    fread (&record->filesize, 8, 1, inf);

  fread (&win_filetime, 8, 1, inf);

  file_epoch = win_filetime_to_epoch (win_filetime);
  record->filetime = localtime (&file_epoch);

  fread (&ucs2_filename, 2, 0x104, inf);

  record->utf8_filename = g_utf16_to_utf8 ((gunichar2 *)ucs2_filename, 0x104, NULL, NULL, &error);

  if (error) {
    fprintf (stderr, _("Error converting file name to UTF-8 encoding: %s\n"), error->message);
    g_error_free (error);
  }

  return record;
}