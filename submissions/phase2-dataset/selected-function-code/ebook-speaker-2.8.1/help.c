void help ()
{
   int y, x;

   getyx (misc.screenwin, y, x);
   wclear (misc.screenwin);
   waddstr (misc.screenwin, gettext
            ("\nThese commands are available in this version:\n"));
   waddstr (misc.screenwin, "========================================");
   waddstr (misc.screenwin, "========================================\n\n");
   waddstr (misc.screenwin, gettext
            ("cursor down     - move cursor to the next item\n"));
   waddstr (misc.screenwin, gettext
            ("cursor up       - move cursor to the previous item\n"));
   waddstr (misc.screenwin, gettext
            ("cursor right    - skip to next phrase\n"));
   waddstr (misc.screenwin, gettext
            ("cursor left     - skip to previous phrase\n"));
   waddstr (misc.screenwin, gettext ("page-down       - view next page\n"));
   waddstr (misc.screenwin, gettext
            ("page-up         - view previous page\n"));
   waddstr (misc.screenwin, gettext ("enter           - start playing\n"));
   waddstr (misc.screenwin, gettext
            ("space           - pause/resume playing\n"));
   waddstr (misc.screenwin, gettext
            ("home            - play on normal speed\n"));
   waddstr (misc.screenwin, "\n");
   waddstr (misc.screenwin, gettext ("Press any key for next page..."));
   nodelay (misc.screenwin, FALSE);
   wgetch (misc.screenwin);
   nodelay (misc.screenwin, TRUE);
   wclear (misc.screenwin);
   waddstr (misc.screenwin, gettext
            ("\n/               - search for a label\n"));
   waddstr (misc.screenwin, gettext
            ("B               - move cursor to the last item\n"));
   waddstr (misc.screenwin, gettext
            ("d               - store current item to disk in WAV-format\n"));
   waddstr (misc.screenwin, gettext
            ("D               - decrease playing speed\n"));
   waddstr (misc.screenwin, gettext
 ("f               - find the currently playing item and place the cursor there\n"));
   waddstr (misc.screenwin, gettext
            ("g               - go to phrase in current item\n"));
   waddstr (misc.screenwin, gettext ("h or ?          - give this help\n"));
   waddstr (misc.screenwin, gettext
            ("j               - just play current item\n"));
   waddstr (misc.screenwin, gettext
            ("l               - switch to next level\n"));
   waddstr (misc.screenwin, gettext
            ("L               - switch to previous level\n"));
   waddstr (misc.screenwin, gettext ("n               - search forwards\n"));
   waddstr (misc.screenwin, gettext ("N               - search backwards\n"));
   waddstr (misc.screenwin, gettext
            ("o               - select next output sound device\n"));
   waddstr (misc.screenwin, gettext ("p               - place a bookmark\n"));
   waddstr (misc.screenwin, gettext
            ("q               - quit eBook-speaker and place a bookmark\n"));
   if (misc.scan_flag == 1)
      waddstr (misc.screenwin, gettext
               ("r               - rotate scanned document\n"));
   waddstr (misc.screenwin, gettext ("s               - stop playing\n"));
   waddstr (misc.screenwin, gettext ("t               - select next TTS\n"));
   waddstr (misc.screenwin, gettext
            ("T               - move cursor to the first item\n"));
   waddstr (misc.screenwin, gettext
            ("U               - increase playing speed\n"));
   waddstr (misc.screenwin, gettext ("\nPress any key to leave help..."));
   nodelay (misc.screenwin, FALSE);
   wgetch (misc.screenwin);
   nodelay (misc.screenwin, TRUE);
   view_screen ();
   wmove (misc.screenwin, y, x);
} // help