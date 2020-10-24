char *get_input_file (char *src_dir)
{
   switch (chdir (src_dir))
   {
   default:
      break;
   } // switch
   struct dirent **namelist;
   char *file_type[50], search_str[MAX_STR];
   int n, tot, page, show_hidden_files = 0;
   static char name[MAX_STR];
       
   nodelay (misc.screenwin, FALSE);
   wclear (misc.titlewin);
   mvwprintw (misc.titlewin, 0, 0,
              gettext ("%s - Choose an input-file"), misc.copyright);
   wrefresh (misc.titlewin);
   if (show_hidden_files)
      tot = scandir (src_dir, &namelist, NULL, alphasort) - 1;
   else
      tot = scandir (src_dir, &namelist, &hidden_files, alphasort) - 1;
   fill_ls (tot, namelist, file_type);
   n = 0;
   *search_str = 0;
   while (1)
   {
      int search_flag;

      mvwprintw (misc.titlewin, 1,  0, "----------------------------------------");
      waddstr (misc.titlewin, "----------------------------------------");
      mvwprintw (misc.titlewin, 1, 0, gettext ("'h' for help "));
      wprintw (misc.titlewin, "- %s ", get_current_dir_name ());
      wrefresh (misc.titlewin);
      ls (n, tot, namelist, file_type);
      switch (wgetch (misc.screenwin))
      {
      case 13: // ENTER
      case KEY_RIGHT:
      case '6':
         if (strncmp (file_type[n], "directory", 9) == 0)
         {
            switch (chdir (namelist[n]->d_name))
            {
            default:
            // discart return-code.
               break;
            } // switch
            free (namelist);
            if (show_hidden_files)
               tot = scandir (src_dir, &namelist, NULL, alphasort) - 1;
            else
               tot = scandir (src_dir, &namelist, &hidden_files, alphasort) - 1;
            fill_ls (tot, namelist, file_type);
            n = 0;
            break;
         } // if
         snprintf (name, MAX_STR - 1, "%s/%s",
                   get_current_dir_name (), namelist[n]->d_name);
         free (namelist);
         return name;
      case KEY_LEFT:
      case '4':
         if (strlen (get_current_dir_name ()) == 1)
         // topdir
         {
            beep ();
            break;
         } // if
         switch (chdir (".."))
         {
         default:
         // discart return-code.
            break;
         } // switch
         free (namelist);
         if (show_hidden_files)
            tot = scandir (src_dir, &namelist, NULL, alphasort) - 1;
         else
            tot = scandir (src_dir, &namelist, &hidden_files, alphasort) - 1;
         fill_ls (tot, namelist, file_type);
         n = 0;
         break;
      case KEY_DOWN:
      case '2':
         if (n == tot)
            beep ();
         else
            n++;
         break;
      case KEY_UP:
      case '8':
         n--;
         if (n < 0)
         {
            n = 0;
            beep ();
         } // if
         break;
      case KEY_NPAGE:
      case '3':
         page = n / 23 + 1;
         if (page * 23 >= tot)
            beep ();
         else
            n = page * 23;
         break;
      case KEY_PPAGE:
      case '9':
         page = n / 23 - 1;
         if (page < 0)
         {
            page = 0;
            beep ();
         }
         else
            n = page * 23;
         break;
      case '/':
         *search_str = 0;
         if ((search_flag = search_in_dir (n + 1, tot, '/',
                                           search_str, namelist)) != -1)
            n = search_flag;
         break;
      case 'B':
         n = tot;
         break;
      case 'h':
      case '?':
         help_list ();
         nodelay (misc.screenwin, FALSE);
         wclear (misc.titlewin);
         mvwprintw (misc.titlewin, 0, 0,
                    gettext ("%s - Choose an input-file"), misc.copyright);
         wrefresh (misc.titlewin);
         if (show_hidden_files)
            tot = scandir (src_dir, &namelist, NULL, alphasort) - 1;
         else
            tot = scandir (src_dir, &namelist, &hidden_files, alphasort) - 1;
         fill_ls (tot, namelist, file_type);
         break;
      case 'H':
      {
         char name[55];
         int found;

         show_hidden_files = 1 - show_hidden_files;
         strncpy (name, namelist[n]->d_name, 50);
         free (namelist);
         if (show_hidden_files)
            tot = scandir (src_dir, &namelist, NULL, alphasort) - 1;
         else
            tot = scandir (src_dir, &namelist, &hidden_files, alphasort) - 1;
         fill_ls (tot, namelist, file_type);
         found = 0;
         for (n = 0; n <= tot; n++)
         {
            if (strncmp (namelist[n]->d_name, name, MAX_STR) == 0)
            {
               found = 1;
               break;
            } // if
         } // for
         if (! found)
            n = 0;
         break;
      }
      case 'n':
         if ((search_flag = search_in_dir (n + 1, tot, 'n',
                                           search_str, namelist)) != -1)
            n = search_flag;
         break;
      case 'N':
         if ((search_flag = search_in_dir (n - 1, tot, 'N', 
                                           search_str, namelist)) != -1)
            n = search_flag;
         break;
      case 'q':
         free (namelist);
         quit_eBook_speaker ();
      case 'T':
         n = 0;
         break;
      default:
         beep ();
         break;
      } // switch
   } // while
} // get_input_file