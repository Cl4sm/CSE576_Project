void select_next_output_device ()
{
   FILE *r;
   int n, y;
   size_t bytes;
   char *list[10], *trash;

   wclear (misc.screenwin);
   wprintw (misc.screenwin, gettext ("\nSelect a soundcard:\n\n"));
   if (! (r = fopen ("/proc/asound/cards", "r")))
   {
      endwin ();
      beep ();
      puts (gettext ("Cannot read /proc/asound/cards"));
      fflush (stdout);
      _exit (1);
   } // if
   for (n = 0; n < 10; n++)
   {
      list[n] = (char *) malloc (1000);
      bytes = getline (&list[n], &bytes, r);
      if ((int) bytes == -1)
         break;
      trash = (char *) malloc (1000);
      bytes = getline (&trash, &bytes, r);
      free (trash);
      wprintw (misc.screenwin, "   %s", list[n]);
      free (list[n]);
   } // for          
   fclose (r);
   y = 3;
   nodelay (misc.screenwin, FALSE);
   for (;;)
   {
      wmove (misc.screenwin, y, 2);
      switch (wgetch (misc.screenwin))
      {
      case 13: //
         snprintf (misc.sound_dev, MAX_STR - 1, "hw:%i", y - 3);
         view_screen ();
         nodelay (misc.screenwin, TRUE);
         return;
      case KEY_DOWN:
         if (++y == n + 3)
            y = 3;
         break;
      case KEY_UP:
         if (--y == 2)
           y = n + 2;
         break;
      default:
         view_screen ();
         nodelay (misc.screenwin, TRUE);
         return;
      } // switch
   } // for
} // select_next_output_device