void store_to_disk (xmlTextReaderPtr reader, xmlDocPtr doc)
{
   char str[MAX_STR], s2[MAX_STR];
   int i, pause_flag;
   struct passwd *pw;;

   pause_flag = misc.playing;
   wclear (misc.screenwin);
   snprintf (str, MAX_STR - 1, "%s.wav", daisy[misc.current].label);
   for (i = 0; str[i] != 0; i++)
   {
      if (str[i] == '/')
         str[i] = '-';
      if (isspace (str[i]))
         str[i] = '_';
   } // for
   wprintw (misc.screenwin,
            "\nStoring \"%s\" as \"%s\" into your home-folder...",
            daisy[misc.current].label, str);
   wrefresh (misc.screenwin);
   pw = getpwuid (geteuid ());
   snprintf (s2, MAX_STR - 1, "%s/%s", pw->pw_dir, str);
   if (pause_flag > -1)
      pause_resume (reader, doc);
   write_wav (misc.current, s2);
   if (pause_flag > -1)
      pause_resume (reader, doc);
   view_screen ();
} // store_to_disk