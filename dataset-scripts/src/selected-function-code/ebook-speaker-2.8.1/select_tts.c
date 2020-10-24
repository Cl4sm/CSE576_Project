void select_tts ()
{
   int n, y, x = 2;

   wclear (misc.screenwin);
   wprintw (misc.screenwin, gettext ("\nSelect a Text-To-Speech application\n\n"));
   for (n = 0; n < 10; n++)
   {
      char str[MAX_STR];

      if (*misc.tts[n] == 0)
         break;
      strncpy (str, misc.tts[n], MAX_STR - 1);
      str[72] = 0;
      wprintw (misc.screenwin, "    %d %s\n", n, str);
   } // for
   wprintw (misc.screenwin, gettext (
"\nProvide a new TTS.\n\
Be sure that the new TTS reads its information from the file\n\
eBook-speaker.txt and that it writes to the file eBook-speaker.wav.\n\n\
Press DEL to delete a TTS\n\n"));
   wprintw (misc.screenwin,
           "    -------------------------------------------------------------");
   y = misc.tts_no + 3;
   nodelay (misc.screenwin, FALSE);
   for (;;)
   {
      wmove (misc.screenwin, y, x);
      switch (wgetch (misc.screenwin))
      {
      int i;

      case 13:
         misc.tts_no = y - 3;
         view_screen ();
         nodelay (misc.screenwin, TRUE);
         return;
      case KEY_DOWN:
         if (++y == n + 3)
         {
            y = n + 10;
            x = 4;
            wmove (misc.screenwin, y, x);
            nodelay (misc.screenwin, FALSE);
            echo ();
            misc.tts_no = n;
            if (misc.tts_no > 9)
               misc.tts_no = 9;
            wgetnstr (misc.screenwin, misc.tts[misc.tts_no], MAX_STR - 1);
            noecho ();
            if (*misc.tts[misc.tts_no])
            {
               view_screen ();
               nodelay (misc.screenwin, TRUE);
               return;
            } // if
            y = 3;
            x = 2;
         } // if
         break;
      case KEY_UP:
         if (--y == 2)
         {
            y = n + 10;
            x = 4;
            wmove (misc.screenwin, y, x);
            nodelay (misc.screenwin, FALSE);
            echo ();
            misc.tts_no = n;
            if (misc.tts_no > 9)
               misc.tts_no = 9;
            wgetnstr (misc.screenwin, misc.tts[misc.tts_no], MAX_STR - 1);
            noecho ();
            if (*misc.tts[misc.tts_no])
            {
               view_screen ();
               nodelay (misc.screenwin, TRUE);
               return;
            } // if
            y = 3;
            x = 2;
         } // if
         break;
      case KEY_DC:
         for (i = y - 3; i < 10; i++)
            strncpy (misc.tts[i], misc.tts[i + 1], MAX_STR - 1);
         misc.tts_no = 0;
         view_screen ();
         return;
      default:
         if (x == 2)
         {
            view_screen ();
            nodelay (misc.screenwin, TRUE);
            return;
         } // if
      } // switch
   } // for
} // select_tts