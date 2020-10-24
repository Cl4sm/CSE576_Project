void help_list ()
{
   wclear (misc.screenwin);
   waddstr (misc.screenwin, gettext ("\nThese commands are available in this version:\n"));
   waddstr (misc.screenwin, "========================================");
   waddstr (misc.screenwin, "========================================\n");
   waddstr (misc.screenwin, gettext ("cursor down     - move cursor to the next item\n"));
   waddstr (misc.screenwin, gettext ("cursor up       - move cursor to the previous item\n"));
   waddstr (misc.screenwin, gettext ("cursor right    - open this directory or file\n"));
   waddstr (misc.screenwin, gettext ("cursor left     - open previous directory\n"));
   waddstr (misc.screenwin, gettext ("page-down       - view next page\n"));
   waddstr (misc.screenwin, gettext ("page-up         - view previous page\n"));
   waddstr (misc.screenwin,
            gettext ("enter           - open this directory or file\n"));
   waddstr (misc.screenwin, gettext ("/               - search for a label\n"));
   waddstr (misc.screenwin, gettext ("B               - move cursor to the last item\n"));
   waddstr (misc.screenwin, gettext ("h or ?          - give this help\n"));
   waddstr (misc.screenwin, gettext ("H               - display \"hidden\" files on/off\n"));
   waddstr (misc.screenwin, gettext ("n               - search forwards\n"));
   waddstr (misc.screenwin, gettext ("N               - search backwards\n"));
   waddstr (misc.screenwin, gettext ("q               - quit eBook-speaker\n"));
   waddstr (misc.screenwin, gettext ("T               - move cursor to the first item\n"));
   waddstr (misc.screenwin, gettext ("\nPress any key to leave help..."));
   nodelay (misc.screenwin, FALSE);
   wgetch (misc.screenwin);
   nodelay (misc.screenwin, TRUE);
} // help_list