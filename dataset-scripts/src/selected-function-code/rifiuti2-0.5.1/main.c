int main (int argc, char **argv)
{
  uint32_t recordsize, info2_version, dummy;
  void *buf;
  int readstatus;
  FILE *infile, *outfile;
  char *infilename = NULL;
  int output_format = OUTPUT_CSV;
  GOptionGroup *textoptgroup;

  rbin_struct *record;
  uint64_t win_filetime;
  time_t file_epoch;

  unsigned char driveletters[28] =
  {
    'A', 'B', 'C', 'D', 'E', 'F', 'G',
    'H', 'I', 'J', 'K', 'L', 'M', 'N',
    'O', 'P', 'Q', 'R', 'S', 'T', 'U',
    'V', 'W', 'X', 'Y', 'Z', '\\', '?'
  };

  GError *error = NULL;
  GOptionContext *context;


  setlocale (LC_ALL, "");
  bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);

  context = g_option_context_new (_("FILE"));
  g_option_context_add_main_entries (context, mainoptions, "rifiuti");

  textoptgroup = g_option_group_new ("text", _("Plain text output options:"),
                                     _("Show plain text output options"), NULL, NULL);
  g_option_group_add_entries (textoptgroup, textoptions);
  g_option_context_add_group (context, textoptgroup);

  if (!g_option_context_parse (context, &argc, &argv, &error))
  {
    fprintf (stderr, _("Error parsing argument: %s\n"), error->message);
    g_error_free (error);
    g_option_context_free (context);
    exit (RIFIUTI_ERR_ARG);
  }

  g_option_context_free (context);

  if ( fileargs && g_strv_length (fileargs) > 1 )
  {
    fputs (_("ERROR: Must specify exactly one INFO2 file as argument.\n"), stderr);
    exit (RIFIUTI_ERR_ARG);
  }

  if (fileargs)
  {
    /* FIXME: insecure */
    infilename = g_strdup (fileargs[0]);
    infile = fopen (infilename, "rb");

    if (!infile)
    {
      g_fprintf (stderr, _("Error opening file '%s' for reading: %s\n"), infilename, strerror (errno));
      g_free (infilename);
      exit (RIFIUTI_ERR_OPEN_FILE);
    }
  }
  else
  {
    infile = stdin;
    infilename = g_strndup ("-", 2);
  }

  if (outfilename)
  {
    outfile = fopen (outfilename, "wb");
    if (!outfile)
    {
      g_fprintf (stderr, _("Error opening file '%s' for writing: %s\n"), outfilename, strerror (errno));
      exit (RIFIUTI_ERR_OPEN_FILE);
    }
  }
  else
    outfile = stdout;

  if (xml_output)
  {
    if ( no_heading || show_legacy_filename || always_utf8 || (NULL != delim) )
    {
      fputs (_("ERROR: Plain text format options can not be used in XML mode.\n"), stderr);
      exit (RIFIUTI_ERR_ARG);
    }
    output_format = OUTPUT_XML;
  }


  if (NULL == delim)
    delim = g_strndup ("\t", 2);

  /* check for valid info2 file header */
  if ( !fread (&info2_version, 4, 1, infile) )
  {
    fprintf (stderr, _("ERROR: '%s' is not a valid INFO2 file.\n"), infilename);
    exit (RIFIUTI_ERR_BROKEN_FILE);
  }
  info2_version = GUINT32_FROM_LE (info2_version);

  if ( (info2_version != 4) && (info2_version != 5) )
  {
    fprintf (stderr, _("ERROR: '%s' is not a valid INFO2 file.\n"), infilename);
    exit (RIFIUTI_ERR_BROKEN_FILE);
  }

  if ( (4 == info2_version) && (OUTPUT_XML == output_format) && (!from_encoding) )
  {
    fputs (_("ERROR: For Win98 INFO2 file, original file name encoding must be specified with --from-encoding option if output is in XML format.\n"), stderr);
    exit (RIFIUTI_ERR_ARG);
  }

  /*
   * Skip for now, though they probably mean number of files left in Recycle bin
   * and last index, or some related number.
   */
  fread (&dummy, 4, 1, infile);
  fread (&dummy, 4, 1, infile);

  if ( !fread (&recordsize, 4, 1, infile) )
  {
    fprintf (stderr, _("ERROR: '%s' is not a valid INFO2 file.\n"), infilename);
    exit (RIFIUTI_ERR_BROKEN_FILE);
  }
  recordsize = GUINT32_FROM_LE (recordsize);

  /*
   * limit record item size to a ballpark figure; prevent corrupted file
   * or specially crafted INFO2 file (EEEEEEK!) from causing rifiuti allocating
   * too much memory
   */
  if ( recordsize > 65536 )
  {
    fputs (_("ERROR: Size of record of each deleted item is too large."), stderr);
    exit (RIFIUTI_ERR_BROKEN_FILE);
  }

  /* only version 5 contains UCS2 filename */
  if ( (5 == info2_version) && (0x320 == recordsize) )
  {
    has_unicode_filename = TRUE;
  }

  print_header (outfile, infilename, info2_version, output_format);

  buf = g_malloc (recordsize);
  g_assert (buf);

  record = g_malloc (sizeof (rbin_struct));
  g_assert (record);

  while (1) {

    readstatus = fread (buf, recordsize, 1, infile);
    if (readstatus != 1)
    {
      if ( !feof (infile) )
        fprintf (stderr, _("ERROR: Failed to read next record: %s\n"), strerror (errno));

      /* FIXME: Also warn if last read is not exactly 4 bytes? */
      break;
    }

    /* Any legacy character set can contain embedded null byte? */
    record->legacy_filename = g_strndup ((char *) (buf + LEGACY_FILENAME_OFFSET),
                                         RECORD_INDEX_OFFSET - LEGACY_FILENAME_OFFSET);

    memcpy (&record->index, buf + RECORD_INDEX_OFFSET, 4);
    record->index = GUINT32_FROM_LE (record->index);

    memcpy (&record->drive, buf + DRIVE_LETTER_OFFSET, 4);
    record->drive = GUINT32_FROM_LE (record->drive);

    /* first byte will be removed from filename if file is not in recycle bin */
    record->emptied = FALSE;
    if (!record->legacy_filename || !*record->legacy_filename)
    {
      record->emptied = TRUE;
      g_free (record->legacy_filename);

      /* 0-25 => A-Z, 26 => '\', 27 or above is erraneous(?) */
      if (record->drive > sizeof (driveletters) - 2)
      {
        record->drive = sizeof (driveletters) - 1;
        g_fprintf (stderr,
                   _("WARNING: Drive letter (0x%X) exceeded maximum (0x1A) for record %u.\n"),
                   record->drive, record->index);
      }

      record->legacy_filename = (char *) g_malloc (RECORD_INDEX_OFFSET - LEGACY_FILENAME_OFFSET);
      g_assert (record->legacy_filename);
      g_snprintf (record->legacy_filename, RECORD_INDEX_OFFSET - LEGACY_FILENAME_OFFSET,
                  "%c%s", driveletters[record->drive],
                  (char *) (buf + LEGACY_FILENAME_OFFSET + 1));
    }

    memcpy (&win_filetime, buf + FILETIME_OFFSET, 8);

    file_epoch = win_filetime_to_epoch (win_filetime);
    record->filetime = localtime (&file_epoch);

    memcpy (&record->filesize, buf + FILESIZE_OFFSET, 4);
    record->filesize = GUINT32_FROM_LE (record->filesize);

    if (has_unicode_filename)
    {
      record->utf8_filename = g_utf16_to_utf8 ((gunichar2 *) (buf + UNICODE_FILENAME_OFFSET),
                                               (recordsize - UNICODE_FILENAME_OFFSET) / 2,
                                               NULL, NULL, &error);
      /* not checking error, since Windows <= 2000 may insert junk after UCS2 file name */
      if (!record->utf8_filename)
      {
        g_fprintf (stderr,
                 _("Error converting file name from UCS2 encoding to UTF-8 for record %u: %s"),
                 record->index, error->message);
        g_error_free (error);
        record->utf8_filename = g_strdup (_("(File name can not be represented in UTF-8 encoding)"));
      }
    }
    else
      record->utf8_filename = NULL;

    print_record (outfile, record, output_format);

    if (has_unicode_filename)
      g_free (record->utf8_filename);

    g_free (record->legacy_filename);

  }

  print_footer (outfile, output_format);

  fclose (infile);
  fclose (outfile);

  g_free (record);
  g_free (buf);
  g_free (infilename);

  exit (0);
}