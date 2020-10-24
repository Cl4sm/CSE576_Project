char *lowriter_to (char *file, char *ext)
{
   char cmd[MAX_CMD];
   DIR *dir;
   struct dirent *dirent;

   snprintf (cmd, MAX_CMD - 1,
         "lowriter --headless --convert-to %s \"%s\" > /dev/null", ext, file);
   switch (system (cmd))
   {
   case 0:
      break;
   default:
      endwin ();
      beep ();
      puts (gettext ("eBook-speaker needs the libreoffice-writer package."));
      fflush (stdout);
      _exit (1);
   } // switch
   if (! (dir = opendir (".")))
   {
      endwin ();
      beep ();
      printf ("\n%s\n", strerror (errno));
      fflush (stdout);
      _exit (1);
   } // if
   while ((dirent = readdir (dir)) != NULL)
   {
      if (strcasecmp (dirent->d_name +
          strlen (dirent->d_name) - 4, ".pdf") == 0)
         break;
   } // while                             
   closedir (dir);
   if (access (dirent->d_name, R_OK) != 0)
   {
      endwin ();
      beep ();
      printf (gettext ("Unknown format\n"));
      fflush (stdout);
      _exit (1);
   } // if                                           
   return dirent->d_name;
} // lowriter_to