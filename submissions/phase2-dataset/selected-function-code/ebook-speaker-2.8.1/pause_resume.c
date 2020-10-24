void pause_resume (xmlTextReaderPtr reader, xmlDocPtr doc)
{
   if (misc.playing != -1)
      misc.playing = -1;
   else
      misc.playing = misc.displaying;
   if (! reader)
      return;
   else
   {
      if (misc.playing != -1)
      {
         FILE *w;

         kill (misc.player_pid, SIGKILL);
         wattron (misc.screenwin, A_BOLD);
         mvwprintw (misc.screenwin, daisy[misc.playing].y, 69, "%5d %5d",
                    misc.phrase_nr, daisy[misc.playing].n_phrases - misc.phrase_nr - 1);
         wattroff (misc.screenwin, A_BOLD);
         wmove (misc.screenwin, daisy[misc.displaying].y, daisy[misc.displaying].x);
         wrefresh (misc.screenwin);
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
            return;
         } // switch
      }
      else
         kill_player ();
   } // if
} // pause_resume