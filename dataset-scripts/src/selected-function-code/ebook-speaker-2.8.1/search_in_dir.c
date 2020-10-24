int search_in_dir (int start, int total, char mode,
                   char *search_str, struct dirent **namelist)
{
   static int c;
   int found = 0;

   if (*search_str == 0)
   {
      mvwaddstr (misc.titlewin, 1, 0, "----------------------------------------");
      waddstr (misc.titlewin, "----------------------------------------");
      mvwprintw (misc.titlewin, 1, 0, gettext ("What do you search? "));
      echo ();
      wgetnstr (misc.titlewin, search_str, 25);
      noecho ();
   } // if
   if (mode == '/' || mode == 'n')
   {
      for (c = start; c <= total; c++)
      {
         if (strcasestr (namelist[c]->d_name, search_str))
         {
            found = 1;
            break;
         } // if
      } // for
      if (! found)
      {
         for (c = 0; c < start; c++)
         {
            if (strcasestr (namelist[c]->d_name, search_str))
            {
               found = 1;
               break;
            } // if
         } // for
      } // if
   }
   else
   { // mode == 'N'
      for (c = start; c > 0; c--)
      {
         if (strcasestr (namelist[c]->d_name, search_str))
         {
            found = 1;
            break;
         } // if
      } // for
      if (! found)
      {
         for (c = total - 1; c > start; c--)
         {
            if (strcasestr (namelist[c]->d_name, search_str))
            {
               found = 1;
               break;
            } // if
         } // for
      } // if
   } // if
   if (found)
   {
      return c;
   }
   else
   {
      beep ();
      return -1;
   } // if
} // search_in_dir