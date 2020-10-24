void browse (char *file)
{
   int old;
   xmlTextReaderPtr reader;
   xmlDocPtr doc;

   misc.current = misc.displaying = misc.playing = 0;
   misc.just_this_item = misc.playing = -1;
   reader = NULL;
   doc = NULL;
   if (! misc.scan_flag)
      reader = get_bookmark (reader, doc);
   if (misc.depth <= 0)
      misc.depth = 1;
   view_screen ();
   nodelay (misc.screenwin, TRUE);
   wmove (misc.screenwin, daisy[misc.current].y, daisy[misc.current].x);
   for (;;)
   {
      switch (wgetch (misc.screenwin))
      {
      case 13: // ENTER
         misc.playing = misc.displaying = misc.current;
         remove ("eBook-speaker.wav");
         remove ("old.wav");
         misc.phrase_nr = 0;
         misc.just_this_item = -1;
         view_screen ();
         kill_player ();
         reader = open_text_file (reader, doc, daisy[misc.current].smil_file,
                                  daisy[misc.current].anchor);
         break;
      case '/':
         search (reader, doc, misc.current + 1, '/');
         break;
      case ' ':
      case KEY_IC:
      case '0':
         pause_resume (reader, doc);
         break;
      case 'B':
         misc.displaying = misc.current = misc.total_items - 1;
         misc.phrase_nr = daisy[misc.current].n_phrases - 1;
         view_screen ();
         break;
      case 'f':
         if (misc.playing == -1)
         {
            beep ();
            break;
         } // if
         misc.current = misc.displaying= misc.playing;
         view_screen ();
         break;
      case 'g':
         reader = go_to_phrase (reader, doc);
         break;
      case 'h':
      case '?':
         kill_player ();
         help ();
         kill_player ();
         if (misc.playing != -1)
         {
            misc.phrase_nr--;
            get_next_phrase (reader, doc, misc.playing);
         } // if
         break;
      case 'j':
      case '5':
      case KEY_B2:
         if (misc.just_this_item != -1)
            misc.just_this_item = -1;
         else
         {
            if (misc.playing == -1)
            {
               misc.phrase_nr = 0;
               reader = open_text_file (reader, doc, daisy[misc.current].smil_file,
                                        daisy[misc.current].anchor);
            } // if
            misc.playing = misc.just_this_item = misc.current;
         } // if
         mvwprintw (misc.screenwin, daisy[misc.current].y, 0, " ");
         if (misc.playing == -1)
         {
            misc.just_this_item = misc.displaying = misc.playing = misc.current;
            misc.phrase_nr = 0;
            kill_player ();
            reader = open_text_file (reader, doc, daisy[misc.current].smil_file,
                            daisy[misc.current].anchor);
         } // if
         if (misc.just_this_item != -1 &&
             daisy[misc.displaying].screen == daisy[misc.playing].screen)
            mvwprintw (misc.screenwin, daisy[misc.current].y, 0, "J");
         else
            mvwprintw (misc.screenwin, daisy[misc.current].y, 0, " ");
         wrefresh (misc.screenwin);
         break;
      case 'l':
         change_level ('l');
         break;
      case 'L':
         change_level ('L');
         break;
      case 'n':
         search (reader, doc, misc.current + 1, 'n');
         break;
      case 'N':
         search (reader, doc, misc.current - 1, 'N');
         break;
      case 'o':
         if (misc.playing == -1)
         {
            beep ();
            break;
         } // if
         pause_resume (reader, doc);
         select_next_output_device ();
         misc.playing = misc.displaying;
         kill_player ();
         if (misc.playing != -1)
         {
            misc.phrase_nr--;
            get_next_phrase (reader, doc, misc.playing);
         } // if
         break;
      case 'p':
         put_bookmark ();
         break;
      case 'q':
         quit_eBook_speaker ();
      case 'r':
      {
         char cmd[MAX_CMD + 1], str[MAX_STR];

         if (misc.scan_flag != 1)
         {
            beep ();
            break;
         } // if
         xmlTextReaderClose (reader);
         xmlFreeDoc (doc);
         remove ("eBook-speaker.wav");
         remove ("old.wav");
         misc.phrase_nr = 0;
         view_screen ();
         wattron (misc.titlewin, A_BOLD);
         mvwprintw (misc.titlewin, 0, 0, "%s - %s", misc.copyright,
                  gettext ("Please wait..."));
         wattroff (misc.titlewin, A_BOLD);
         wrefresh (misc.titlewin);
         kill_player ();
         snprintf (cmd, MAX_CMD, "pnmflip -r180 \"%s\" > \"%s.rotated\"",
                   file, file);
         switch (system (cmd))
         {
         default:
            break;
         } // switch
         snprintf (str, MAX_STR, "%s.rotated", file);
         rename (str, file);
         unlink ("out");
         unlink ("out.epub");
         unlink ("out.txt");
         view_screen ();
         start_tesseract (file);
         read_daisy_3 (get_current_dir_name ());
         check_phrases ();
         wclear (misc.titlewin);
         mvwprintw (misc.titlewin, 0, 0, misc.copyright);
         if ((int) strlen (misc.daisy_title) + (int)
             strlen (misc.copyright) >= misc.max_x)
            mvwprintw (misc.titlewin, 0, misc.max_x - strlen (misc.daisy_title) - 4, "... ");
         mvwprintw (misc.titlewin, 0, misc.max_x - strlen (misc.daisy_title),
              "%s", misc.daisy_title);
         mvwaddstr (misc.titlewin, 1, 0, "----------------------------------------");
         waddstr (misc.titlewin, "----------------------------------------");
         mvwprintw (misc.titlewin, 1, 0, gettext ("Press 'h' for help "));
         wrefresh (misc.titlewin);
         misc.level = 1;
         *misc.search_str = 0;
         misc.current = misc.displaying = misc.playing = 0;
         misc.just_this_item = misc.playing = -1;
         reader = NULL;
         doc = NULL;
         if (misc.depth <= 0)
            misc.depth = 1;
         view_screen ();
         nodelay (misc.screenwin, TRUE);
         wmove (misc.screenwin, daisy[misc.current].y, daisy[misc.current].x);
         break;
      }
      case 's':
         misc.playing = misc.just_this_item = -1;
         view_screen ();
         kill_player ();
         break;
      case 't':

         if (misc.playing != -1)
            pause_resume (reader, doc);
         select_tts ();
         misc.playing = misc.displaying;
         kill_player ();
         if (misc.playing != -1)
         {
            misc.phrase_nr--;
            get_next_phrase (reader, doc, misc.playing);
         } // if
         break;
      case 'T':
         misc.displaying = misc.current = 0;
         misc.phrase_nr = daisy[misc.current].n_phrases - 1;
         view_screen ();
         break;
      case KEY_DOWN:
      case '2':
         next_item ();
         break;
      case KEY_UP:
      case '8':
         previous_item ();
         break;
      case KEY_RIGHT:
      case '6':
         skip_right ();
         break;
      case KEY_LEFT:
      case '4':
         reader = skip_left (reader, doc);
         break;
      case KEY_NPAGE:
      case '3':
         if (misc.current / misc.max_y == (misc.total_items - 1) / misc.max_y)
         {
            beep ();
            break;
         } // if
         old = misc.current / misc.max_y;
         while (misc.current / misc.max_y == old)
            next_item ();
         view_screen ();
         break;
      case KEY_PPAGE:
      case '9':
         if (misc.current / misc.max_y == 0)
         {
            beep ();
            break;
         } // if
         old = misc.current / misc.max_y - 1;
         misc.current = 0;
         while (misc.current / misc.max_y != old)
            next_item ();
         view_screen ();
         break;
      case ERR:
         break;
      case 'U':
      case '+':
         if (misc.speed >= 2)
         {
            beep ();
            break;
         } // if
         misc.speed += 0.1;
         if (misc.playing == -1)
            break;
         kill_player ();
         misc.phrase_nr--;
         get_next_phrase (reader, doc, misc.playing);
         break;
      case 'd':
         store_to_disk (reader, doc);
         break;
      case 'D':
      case '-':
         if (misc.speed <= 0.1)
         {
            beep ();
            break;
         } // if
         misc.speed -= 0.1;
         if (misc.playing == -1)
            break;
         kill_player ();
         misc.phrase_nr--;
         get_next_phrase (reader, doc, misc.playing);
         break;
      case KEY_HOME:
      case '*':
      case '7':
         misc.speed = 1.0;
         if (misc.playing == -1)
            break;
         kill_player ();
         misc.phrase_nr--;
         get_next_phrase (reader, doc, misc.playing);
         break;
      default:
         beep ();
         break;
      } // switch

      if (misc.playing != -1)
      {
         if (get_next_phrase (reader, doc, misc.playing) == -1)
         {
            misc.phrase_nr = 0;
            misc.playing++;
            if (daisy[misc.playing].level <= misc.level)
               misc.current = misc.displaying = misc.playing;
            if (misc.just_this_item != -1 && daisy[misc.playing].level <= misc.level)
               misc.playing = misc.just_this_item = -1;
            remove ("eBook-speaker.wav");
            remove ("old.wav");
            reader = open_text_file (reader, doc, daisy[misc.current].smil_file,
                                     daisy[misc.current].anchor);
            view_screen ();
         } // if
      } // if

      fd_set rfds;
      struct timeval tv;

      FD_ZERO (&rfds);
      FD_SET (0, &rfds);
      tv.tv_sec = 0;
      tv.tv_usec = 100000;
// pause till a key has been pressed or 0.1 seconds are elapsed
      select (1, &rfds, NULL, NULL, &tv);
   } // for
} // browse