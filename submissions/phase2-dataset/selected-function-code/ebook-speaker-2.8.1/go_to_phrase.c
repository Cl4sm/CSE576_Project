xmlTextReaderPtr go_to_phrase (xmlTextReaderPtr reader, xmlDocPtr doc)
{
   char pn[15];

   kill_player ();
   misc.player_pid = -2;
   mvwaddstr (misc.titlewin, 1, 0, "----------------------------------------");
   waddstr (misc.titlewin, "----------------------------------------");
   mvwprintw (misc.titlewin, 1, 0, gettext ("Go to phrase in current item: "));
   echo ();
   wgetnstr (misc.titlewin, pn, 8);
   noecho ();
   view_screen ();
   if (! *pn || atoi (pn) > daisy[misc.current].n_phrases + 1)
   {
      beep ();
      reader = skip_left (reader, doc);
      skip_right ();
      return reader;  
   } // if
   reader = open_text_file (reader, doc, daisy[misc.current].smil_file,
                            daisy[misc.current].anchor);
   misc.phrase_nr = 0;
   if (atoi (pn) > 1)
   {
      while (1)
      {
         if (! get_tag_or_label (reader))
            break;
         if (! *misc.label)
            continue;
         (misc.phrase_nr)++;
         if (misc.phrase_nr + 1 == atoi (pn))
            break;
      } // while
   } // if
   misc.playing = misc.displaying = misc.current;
   view_screen ();
   mvwprintw (misc.screenwin, daisy[misc.current].y, 69, "%5d %5d",
              misc.phrase_nr + 1, daisy[misc.current].n_phrases - misc.phrase_nr - 1);
   wrefresh (misc.screenwin);
   wmove (misc.screenwin, daisy[misc.current].y, daisy[misc.current].x);
   misc.just_this_item = -1;
   remove ("eBook-speaker.wav");
   remove ("old.wav");
   return reader;
} // go_to_phrase