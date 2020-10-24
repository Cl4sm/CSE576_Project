void search (xmlTextReaderPtr reader, xmlDocPtr doc, int start, char mode)
{
   int c, found = 0;

   if (mode == '/')
   {
      if (misc.playing != -1)
         kill_player ();
      mvwaddstr (misc.titlewin, 1, 0, "----------------------------------------");
      waddstr (misc.titlewin, "----------------------------------------");
      mvwprintw (misc.titlewin, 1, 0, gettext ("What do you search?                           "));
      echo ();
      wmove (misc.titlewin, 1, 20);
      wgetnstr (misc.titlewin, misc.search_str, 25);
      noecho ();
   } // if
   if (mode == '/' || mode == 'n')
   {
      for (c = start; c < misc.total_items; c++)
      {
         if (strcasestr (daisy[c].label, misc.search_str))
         {
            found = 1;
            break;
         } // if
      } // for
      if (! found)
      {
         for (c = 0; c < start; c++)
         {
            if (strcasestr (daisy[c].label, misc.search_str))
            {
               found = 1;
               break;
            } // if
         } // for
      } // if
   }
   else
   { // mode == 'N'
      for (c = start; c >= 0; c--)
      {
         if (strcasestr (daisy[c].label, misc.search_str))
         {
            found = 1;
            break;
         } // if
      } // for
      if (! found)
      {
         for (c = misc.total_items + 1; c > start; c--)
         {
            if (strcasestr (daisy[c].label, misc.search_str))
            {
               found = 1;
               break;
            } // if
         } // for
      } // if
   } // if
   if (found)
   {
      misc.current = c;
      misc.phrase_nr = 0;
      misc.just_this_item = -1;
      misc.displaying = misc.playing = misc.current;
      if (misc.playing != -1)
         kill_player ();
      xmlTextReaderClose (reader);
      xmlFreeDoc (doc);
      reader = open_text_file (reader, doc, daisy[misc.current].smil_file,
                      daisy[misc.current].anchor);
   }
   else
   {
      beep ();
      kill_player ();
      misc.phrase_nr--;
      get_next_phrase (reader, doc, misc.playing);
   } // if
   view_screen ();
} // search