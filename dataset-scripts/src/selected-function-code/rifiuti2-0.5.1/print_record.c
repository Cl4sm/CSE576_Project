static void print_record (FILE        *outfile,
                          rbin_struct *record,
                          int          output_format)
{
  char *shown_filename;
  char ascdeltime[21];
  GError *error = NULL;

  if (strftime (ascdeltime, 20, "%Y-%m-%d %H:%M:%S", record->filetime) == 0)
    fprintf (stderr, _("Error formatting deleted file date/time for index %u."), record->index);

  switch (output_format)
  {
    case OUTPUT_CSV:

      if (always_utf8)
      {
        if (has_unicode_filename)
          shown_filename = g_strdup (record->utf8_filename);
        else
        {
          shown_filename = g_convert (record->legacy_filename, -1, "UTF-8", from_encoding,
                                      NULL, NULL, &error);
          if (error)
          {
            g_fprintf (stderr,
                       _("Error converting file name from %s encoding to UTF-8 for record %u: %s\n"),
                       from_encoding, record->index, error->message);
            g_error_free (error);
            g_free (shown_filename);
            shown_filename = g_strdup (_("(File name can not be represented in UTF-8 encoding)"));
          }
        }
      }
      else if (has_unicode_filename && !show_legacy_filename)
      {
        shown_filename = g_locale_from_utf8 (record->utf8_filename, -1, NULL, NULL, &error);
        if (error)
        {
          g_fprintf (stderr,
                     _("Error converting file name from UTF-8 encoding to current one for record %u: %s\n"),
                     record->index, error->message);
          g_error_free (error);
          g_free (shown_filename);
          shown_filename = g_strdup (_("(File name can not be represented in current character set)"));
        }
      }
      else
        shown_filename = g_strdup (record->legacy_filename);

      fprintf (outfile, "%d%s%s%s%s%s%d%s%s\n",
               record->index                        , delim,
               ascdeltime                           , delim,
               record->emptied ? _("Yes") : _("No") , delim,
               record->filesize                     , delim,
               shown_filename);

      g_free (shown_filename);

      break;

    case OUTPUT_XML:

      fputs ("  <record>\n", outfile);
      fprintf (outfile, "    <index>%u</index>\n", record->index);
      fprintf (outfile, "    <time>%s</time>\n", ascdeltime);
      fprintf (outfile, "    <emptied>%c</emptied>\n", record->emptied ? 'Y' : 'N');
      fprintf (outfile, "    <size>%u</size>\n", record->filesize);

      if (has_unicode_filename)
        fprintf (outfile, "    <path>%s</path>\n", record->utf8_filename);
      else
      {
        /*
         * guessing charset is not useful, since the system generating INFO2 and the system
         * analyzing INFO2 would be different, and quite likely using different charset as well
         */
        shown_filename = g_convert (record->legacy_filename, -1, "UTF-8", from_encoding,
                                    NULL, NULL, &error);
        if (error)
        {
          g_fprintf (stderr,
                     _("Error converting file name from %s encoding to UTF-8 for record %u: %s\n"),
                     from_encoding, record->index, error->message);
          g_error_free (error);
          g_free (shown_filename);
          shown_filename = g_strdup (_("(File name can not be represented in UTF-8 encoding)"));
        }
        fprintf (outfile, "    <path>%s</path>\n", shown_filename);
        g_free (shown_filename);
      }

      fputs ("  </record>\n", outfile);

      break;

    default:
      /* something is wrong */
      fputs (_("Unrecognized output format\n"), stderr);
      break;
  }
}