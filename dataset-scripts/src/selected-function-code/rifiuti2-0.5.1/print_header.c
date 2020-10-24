static void print_header (FILE     *outfile,
                          char     *infilename,
                          uint32_t  version,
                          int       output_format)
{
  char *shown_filename;
  GError *error = NULL;

  switch (output_format)
  {
    case OUTPUT_CSV:

      if (!no_heading)
      {
        fprintf (outfile, _("Recycle bin file: '%s'\n"), infilename);
        fprintf (outfile, _("Version: %u\n\n"), version);
        fprintf (outfile, _("Index%sDeleted Time%sGone?%sSize%sPath\n"), delim, delim, delim, delim);
      }

      break;

    case OUTPUT_XML:

      fputs ("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<recyclebin>\n", outfile);
      shown_filename = g_filename_to_utf8 (infilename, -1, NULL, NULL, &error);

      if (error)
      {
        fprintf (stderr, _("Error converting INFO2 file name to UTF-8: %s\n"),
                 error->message);
        g_free (shown_filename);
        shown_filename = g_strdup (_("(File name can not be represented in UTF-8 encoding)"));
      }

      fprintf (outfile, "  <filename>%s</filename>\n", shown_filename);
      fprintf (outfile, "  <version>%u</version>\n", version);
      g_free (shown_filename);

      break;

    default:
      /* something is wrong */
      fputs (_("Unrecognized output format\n"), stderr);
      break;
  }
}