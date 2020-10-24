void read_daisy_3 (char *daisy_mp)
{
   char cmd[MAX_CMD], path[MAX_STR];
   FILE *r;
   int ret;

   snprintf (cmd, MAX_CMD - 1, "find -iname \"*.ncx\" -print0");
   *path = 0;
   r = popen (cmd, "r");
   ret = fread (path, MAX_STR - 1, 1, r);
   fclose (r);
   if (*path == 0)
   {
      endwin ();
      printf ("%s\n", gettext (strerror (ENOENT)));
      beep ();
      fflush (stdout);
      _exit (0);
   } // if
   strncpy (misc.ncx_name, basename (path), MAX_STR - 1);
   strncpy (misc.NCC_HTML, misc.ncx_name, MAX_STR - 1);
   snprintf (daisy_mp, MAX_STR - 1, "%s/%s",
             get_current_dir_name (), dirname (path));
   ret = chdir (daisy_mp);
   snprintf (cmd, MAX_CMD - 1, "find -iname \"*.opf\" -print0");
   r = popen (cmd, "r");
   ret = fread (path, MAX_STR - 1, 1, r);
   fclose (r);
   ret = ret; // discard compiler warning
   strncpy (misc.opf_name, basename (path), MAX_STR - 1);
   parse_opf (misc.opf_name);
   misc.total_items = misc.current;
} // read_daisy_3