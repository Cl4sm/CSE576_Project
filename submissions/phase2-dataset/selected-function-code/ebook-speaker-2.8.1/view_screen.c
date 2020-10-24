void view_screen ()
{
   int i, x, l;

   mvwaddstr (misc.titlewin, 1, 0, "----------------------------------------");
   waddstr (misc.titlewin, "----------------------------------------");
   mvwprintw (misc.titlewin, 1, 0, gettext ("'h' for help "));
   if (! misc.discinfo)
   {
      if (misc.total_pages)
         mvwprintw (misc.titlewin, 1, 15, gettext (" %d pages "), misc.total_pages);
      mvwprintw (misc.titlewin, 1, 28,
         gettext (" level: %d of %d "), misc.level, misc.depth);
      mvwprintw (misc.titlewin, 1, 47, gettext (" total phrases: %d "),
                 misc.total_phrases);
      mvwprintw (misc.titlewin, 1, 74, " %d/%d ",
              misc.current / misc.max_y + 1, (misc.total_items - 1) / misc.max_y + 1);
   } // if
   wrefresh (misc.titlewin);

   wclear (misc.screenwin);
   for (i = 0; daisy[i].screen != daisy[misc.current].screen; i++);
   do
   {
      mvwprintw (misc.screenwin, daisy[i].y, daisy[i].x + 1,
                            daisy[i].label);
      l = strlen (daisy[i].label);
      if (l / 2 * 2 == l)
         waddstr (misc.screenwin, " ");
      for (x = l; x < 61; x += 2)
         waddstr (misc.screenwin, " .");
      if (daisy[i].page_number)
         mvwprintw (misc.screenwin, daisy[i].y, 65,
                    "(%3d)", daisy[i].page_number);
      l = daisy[i].n_phrases;
      x = i + 1;
      while (daisy[x].level > misc.level)
         l += daisy[x++].n_phrases;
      if (daisy[i].level <= misc.level)
         mvwprintw (misc.screenwin, daisy[i].y, 75, "%5d", l);
      if (i >= misc.total_items - 1)
         break;
   } while (daisy[++i].screen == daisy[misc.current].screen);
   if (misc.just_this_item != -1 &&
       daisy[misc.displaying].screen == daisy[misc.playing].screen)
      mvwprintw (misc.screenwin, daisy[misc.current].y, 0, "J");
   mvwprintw (misc.screenwin, 21, 0, "----------------------------------------");
   wprintw (misc.screenwin, "----------------------------------------");
   wmove (misc.screenwin, daisy[misc.current].y, daisy[misc.current].x);
   wrefresh (misc.screenwin);
} // view_screen