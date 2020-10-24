void write_wav (int c, char *outfile)
{
   FILE *w;
   xmlTextReaderPtr local_reader;
   xmlDocPtr local_doc;
   sox_format_t *first, *output;
   #define MAX_SAMPLES (size_t)2048
   sox_sample_t samples[MAX_SAMPLES];
   size_t number_read;

   if ((w = fopen ("eBook-speaker.txt", "w")) == NULL)
   {
      endwin ();
      beep ();
      printf ("Can't make a temp file %s\n", "eBook-speaker.txt");
      fflush (stdout);
      _exit (0);
   } // if
   if (fwrite ("init", 4, 1, w) == 0)
   {
      endwin ();
      beep ();
      printf ("write (\"%s\"): failed.\n", misc.label);
      fflush (stdout);
      _exit (0);
   } // if
   fclose (w);
   if (! *misc.tts[misc.tts_no])
   {
      misc.tts_no = 0;
      select_tts ();
   } // if
   switch (system (misc.tts[misc.tts_no]))
   {
   default:
      break;
   } // switch
   sox_init ();
   rename ("eBook-speaker.wav", "first-eBook-speaker.wav");
   first = sox_open_read ("first-eBook-speaker.wav", NULL, NULL, NULL);
   output = sox_open_write (outfile, &first->signal, &first->encoding,
                            NULL, NULL, NULL);
   local_doc = NULL;
   local_reader = NULL;
   local_reader = open_text_file (local_reader, local_doc,
                  daisy[c].smil_file, daisy[c].anchor);
   while (1)
   {
      sox_format_t *input;

      if (! get_tag_or_label (local_reader))
         break;
      if (*daisy[c + 1].anchor &&
          strcasecmp (my_attribute.id, daisy[c + 1].anchor) == 0)
         break;
      if (! *misc.label)
         continue;;
      w = fopen ("eBook-speaker.txt", "w");
      fwrite (misc.label, strlen (misc.label), 1, w);
      fclose (w);
      switch (system (misc.tts[misc.tts_no]))
      {
      default:
         break;
      } // switch
      input = sox_open_read ("eBook-speaker.wav", NULL, NULL, NULL);
      while ((number_read = sox_read (input, samples, MAX_SAMPLES)))
         sox_write (output, samples, number_read);
      sox_close (input);
   } // while
   sox_close (output);
   sox_close (first);
   sox_quit ();
} // write_wav