int get_next_phrase (xmlTextReaderPtr reader, xmlDocPtr doc, int pl)
{
   FILE *w;

   if (access ("eBook-speaker.wav", R_OK) == 0)
   // when still playing
      return 0;
   while (1)
   {
      if (! get_tag_or_label (reader))
      {
         if (pl >= misc.total_items - 1)
         {
            char str[MAX_STR];
            struct passwd *pw;;

            save_bookmark_and_xml ();
            pw = getpwuid (geteuid ());
            snprintf (str, MAX_STR - 1, "%s/.eBook-speaker/%s",
                      pw->pw_dir, misc.bookmark_title);
            unlink (str);
            _exit (0);
         } // if
         return -1;
      } // if
      if (*daisy[pl + 1].anchor &&
          strcasecmp (my_attribute.id, daisy[pl + 1].anchor) == 0)
         return -1;
      if (*misc.label)
         break;
   } // while

   if (daisy[pl].screen == daisy[misc.current].screen)
   {
      wattron (misc.screenwin, A_BOLD);
      mvwprintw (misc.screenwin, daisy[pl].y, 69, "%5d %5d",
               misc.phrase_nr + 1, daisy[pl].n_phrases - misc.phrase_nr - 1);
      (misc.phrase_nr)++;
      wattroff (misc.screenwin, A_BOLD);
      mvwprintw (misc.screenwin, 22, 0, "%s", misc.label);
      wclrtoeol (misc.screenwin);
      wmove (misc.screenwin, daisy[misc.displaying].y, daisy[misc.displaying].x);
      wrefresh (misc.screenwin);
   } // if
   if ((w = fopen ("eBook-speaker.txt", "w")) == NULL)
   {
      endwin ();
      beep ();
      printf ("Can't make a temp file %s\n", "eBook-speaker.txt");
      fflush (stdout);
      _exit (0);
   } // if
   if (fwrite (misc.label, strlen (misc.label), 1, w) == 0)
   {
      endwin ();
      beep ();
      printf ("write (\"%s\"): failed.\n", misc.label);
      fflush (stdout);
      kill (getppid (), SIGINT);
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

   switch (misc.player_pid = fork ())
   {
   case -1:
   {
      int e;

      e = errno;
      endwin ();
      beep ();
      reader = skip_left (reader, doc);
      put_bookmark ();
      printf ("%s\n", strerror (e));
      fflush (stdout);
      _exit (1);
   }
   case 0:
      setsid ();
      playfile ("eBook-speaker.wav", "wav", misc.sound_dev, "alsa", misc.speed);
      _exit (0);
   default:
      return 0;
   } // switch
} // get_next_phrase