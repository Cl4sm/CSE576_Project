void playfile (char *in_file, char *in_type,
               char *out_file, char *out_type, float sp)
{
   sox_format_t *in, *out; /* input and output files */
   sox_effects_chain_t *chain;
   sox_effect_t *e;
   char *args[MAX_STR], str[MAX_STR];

   sox_globals.verbosity = 0;
   sox_init ();
   if (! (in = sox_open_read (in_file, NULL, NULL, in_type)))
   {
      int e;

      e = errno;
      endwin ();
      printf ("sox_open_read: %s: %s\n", in_file, strerror (e));
      fflush (stdout);
      beep ();
      kill (getppid (), SIGQUIT);
   } // if
   if ((out = sox_open_write (out_file, &in->signal,
           NULL, out_type, NULL, NULL)) == NULL)
   {
      endwin ();
      printf ("%s: %s\n", out_file, gettext (strerror (EINVAL)));
      strncpy (misc.sound_dev, "hw:0", 5);
      save_xml ();
      beep ();
      fflush (stdout);
      kill (getppid (), SIGQUIT);
   } // if

   chain = sox_create_effects_chain (&in->encoding, &out->encoding);

   e = sox_create_effect (sox_find_effect ("input"));
   args[0] = (char *) in, sox_effect_options (e, 1, args);
   sox_add_effect (chain, e, &in->signal, &in->signal);

   e = sox_create_effect (sox_find_effect ("tempo"));
   snprintf (str, MAX_STR, "%lf", sp);
   args[0] = "-s";
   args[1] = str;
   sox_effect_options (e, 2, args);
   sox_add_effect (chain, e, &in->signal, &in->signal);

   snprintf (str, MAX_STR - 1, "%lf", out->signal.rate);
   e = sox_create_effect (sox_find_effect ("rate"));
   args[0] = str, sox_effect_options (e, 1, args);
   sox_add_effect (chain, e, &in->signal, &in->signal);

   snprintf (str, MAX_STR - 1, "%i", out->signal.channels);
   e = sox_create_effect (sox_find_effect ("channels"));
   args[0] = str, sox_effect_options (e, 1, args);     
   sox_add_effect (chain, e, &in->signal, &in->signal);

   e = sox_create_effect (sox_find_effect ("output"));
   args[0] = (char *) out, sox_effect_options (e, 1, args);
   sox_add_effect (chain, e, &in->signal, &out->signal);

   sox_flow_effects (chain, NULL, NULL);
   sox_delete_effects_chain (chain);
   sox_close (out);
   sox_close (in);
   sox_quit ();
   unlink ("eBook-speaker.wav");
} // playfile