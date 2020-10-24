void pandoc_to_epub (char *file)
{
   char cmd[MAX_CMD];

   snprintf (cmd, MAX_CMD - 1, "pandoc \"%s\" -o out.epub", file);
   switch (system (cmd))
   {
   case 0:
      break;
   default:
      endwin ();
      beep ();
      puts (misc.copyright);
      puts (gettext ("eBook-speaker needs the pandoc package."));
      fflush (stdout);
      _exit (1);
   } // switch
   if (access ("out.epub", R_OK) != 0)
   {
      endwin ();
      beep ();
      printf (gettext ("Unknown format\n"));
      fflush (stdout);
      _exit (1);
   } // if
   read_epub ("out.epub");
} // pandoc_to_epub