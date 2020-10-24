int main (int argc, char *argv[])
{
   int opt;
   char file[MAX_STR], pwd[MAX_STR], str[MAX_STR];

   fclose (stderr);
   setbuf (stdout, NULL);
   setlocale (LC_ALL, "");
   setlocale (LC_NUMERIC, "C");
   textdomain (PACKAGE);
   snprintf (str, MAX_STR, "%s/", LOCALEDIR);
   bindtextdomain (PACKAGE, str);
   initscr ();
   misc.titlewin = newwin (2, 80,  0, 0);
   misc.screenwin = newwin (23, 80, 2, 0);
   getmaxyx (misc.screenwin, misc.max_y, misc.max_x);
   misc.max_y -= 2;
   keypad (misc.screenwin, TRUE);
   meta (misc.screenwin,       TRUE);
   nonl ();
   noecho ();
   misc.eBook_speaker_pid = getpid ();
   misc.player_pid = -2;
   snprintf (misc.copyright, MAX_STR - 1, gettext
             ("eBook-speaker - Version %s - (C)2014 J. Lemmens"), PACKAGE_VERSION);
   wattron (misc.titlewin, A_BOLD);
   wprintw (misc.titlewin, "%s - %s", misc.copyright, gettext ("Please wait..."));
   wattroff (misc.titlewin, A_BOLD);
   if (access ("/usr/share/doc/espeak", R_OK) != 0)
   {
      endwin ();
      beep ();
      puts (gettext ("eBook-speaker needs the espeak package."));
      printf (gettext ("Please install it and try again.\n"));
      fflush (stdout);
      _exit (1);
   } // if
   if (access ("/usr/share/doc/unar", R_OK) != 0)
   {
      endwin ();
      beep ();
      puts (gettext ("eBook-speaker needs the unar package."));
      printf (gettext ("Please install it and try again.\n"));
      fflush (stdout);
      _exit (1);
   } // if
   misc.speed = 1.0;
   atexit (quit_eBook_speaker);
   signal (SIGCHLD, player_ended);
   signal (SIGTERM, quit_eBook_speaker);
   strncpy (misc.sound_dev, "hw:0", MAX_STR - 1);
   *misc.ocr_language = 0;
   read_xml ();
   misc.tmp_dir = strdup ("/tmp/eBook-speaker.XXXXXX");
   if (! mkdtemp (misc.tmp_dir))
   {
      endwin ();
      printf ("mkdtemp ()\n");
      beep ();
      fflush (stdout);
      _exit (1);
   } // if
   opterr = 0;
   misc.tts_flag = -1, misc.scan_flag = 0;
   while ((opt = getopt (argc, argv, "d:hlo:st:")) != -1)
   {
      switch (opt)
      {
      case 'd':
         strncpy (misc.sound_dev, optarg, 15);
         break;
      case 'h':
         usage ();
      case 'l':
         continue;
      case 'o':
         strncpy (misc.ocr_language, optarg, 3);
         break;
      case 's':
      {
         char cmd[MAX_CMD];

         if (access ("/usr/share/doc/tesseract-ocr", R_OK) != 0)
         {
            endwin ();
            beep ();
            puts (gettext ("eBook-speaker needs the tesseract-ocr package."));
            printf (gettext ("Please install it and try again.\n"));
            fflush (stdout);
            _exit (1);
         } // if
         wrefresh (misc.titlewin);
         misc.scan_flag = 1;
         snprintf (cmd, MAX_CMD,
                   "scanimage --resolution 400 > %s/out.pnm", misc.tmp_dir);
         switch (system (cmd))
         {
         case 0:
            break;
         default:
            endwin ();
            beep ();
            puts (misc.copyright);
            puts (gettext ("eBook-speaker needs the sane-utils package."));
            fflush (stdout);
            _exit (1);
         } // switch
         snprintf (file, MAX_STR, "%s/out.pnm", misc.tmp_dir);
         break;
      }
      case 't':
         strncpy (misc.tts[misc.tts_no], argv[optind], MAX_STR - 1);
         misc.tts_flag = misc.tts_no;
         break;
      } // switch
   } // while

// check if arg is an eBook
   struct stat st_buf;

   if (misc.scan_flag == 0)
   {
      if (! argv[optind])
      // if no arguments are given
      {
         snprintf (file, MAX_STR - 1, "%s", get_input_file ("."));
         execl (*argv, *argv, file, NULL);
      }
      else
      {
         if (*argv[optind] == '/')
            snprintf (file, MAX_STR - 1, "%s", argv[optind]);
         else
            snprintf (file, MAX_STR - 1, "%s/%s", get_current_dir_name (), argv[optind]);
      } // if
   } // if
   if (stat (file, &st_buf) == -1)
   {
      int e;

      e = errno;
      endwin ();
      puts (misc.copyright);
      printf ("%s: %s\n", strerror (e), file);
      beep ();
      fflush (stdout);
      _exit (1);
   } // if
   if (chdir (misc.tmp_dir) == -1)
   {
      endwin ();
      printf ("Can't chdir (\"%s\")\n", misc.tmp_dir);
      beep ();
      fflush (stdout);
      _exit (1);
   } // if
   strncpy (misc.daisy_mp, misc.tmp_dir, MAX_STR - 1);

// determine filetype
   magic_t myt;

BEGIN:
   myt = magic_open (MAGIC_CONTINUE | MAGIC_SYMLINK);
   magic_load (myt, NULL);
   wprintw (misc.titlewin, "\nThis is a %s file", magic_file (myt, file));
   wmove (misc.titlewin, 0, 67);
   wrefresh (misc.titlewin);
   if (strcasestr (magic_file (myt, file), "gzip compressed data"))
   {
      char cmd[512], *begin, *end;
      FILE *p;

      snprintf (cmd, MAX_CMD - 1, "unar \"%s\"", file);
      p = popen (cmd, "r");
      switch ( fread (file, MAX_STR, 1, p))
      {
      default:
         break;
      } // switch
      pclose (p);
      begin = file;
      do
      {
         if (*begin == '"')
            break;
      } while (*++begin != 0);
      begin++;
      end = begin;
      do
      {
         if (*end == '"')
            break;
      } while (++end != 0);
      *end = 0;
      snprintf (file, MAX_STR, "%s/%s", get_current_dir_name (), begin);
      goto BEGIN;
   } // "gzip compressed data"
   if (strcasestr (magic_file (myt, file), "troff or preprocessor input"))
// maybe a man-page
   {
      char cmd[512];

      snprintf (cmd, 500, "man2html \"%s\" > out.html", file);
      switch (system (cmd))
      {
      case 0:
         break;
      default:
         endwin ();
         beep ();
         puts (gettext ("eBook-speaker needs the man2html program."));
         fflush (stdout);
         _exit (1);
      } // switch
      snprintf (file, MAX_STR, "%s/out.html", get_current_dir_name ());
      goto BEGIN;
   } // "troff or preprocessor input"
   else
   if (strcasestr (magic_file (myt, file), "directory"))
   {
      snprintf (pwd, MAX_STR - 1, "%s", file);
      snprintf (file, MAX_STR - 1, "%s",
                get_input_file (pwd));
      execlp (*argv, *argv, file, NULL);
   } // if directory
   else
   if (strcasestr (magic_file (myt, file), "EPUB") ||
       (strcasestr (magic_file (myt, file), "Zip archive data") &&
        ! strcasestr (magic_file (myt, file), "Microsoft Word 2007+")))
   {
      read_epub (file);
   } // if epub
   else
   if (strcasestr (magic_file (myt, file), "HTML document") ||
       strcasestr (magic_file (myt, file), "XML document text"))
   {
      char cmd[MAX_CMD];

      snprintf (cmd, MAX_CMD - 1,
                "html2text -ascii -o out.txt \"%s\"", file);
      switch (system (cmd))
      {
      case 0:
         break;
      default:
         endwin ();
         beep ();
         puts (gettext ("eBook-speaker needs the html2text package."));
         fflush (stdout);
         _exit (1);
      } // switch
      pandoc_to_epub ("out.txt");
   } // if
   else
// use pandoc
   if (strcasestr (magic_file (myt, file), "C source text") ||
       strcasestr (magic_file (myt, file), "ASCII text"))
   {
      pandoc_to_epub (file);
   } // if
   else
   if (strcasestr (magic_file (myt, file), "PDF document"))
   {
      pdf_to_html (file);
      pandoc_to_epub ("out.html");
   } // if
   else
// use lowriter
   if (strcasestr (magic_file (myt, file), "ISO-8859 text") ||
       strcasestr (magic_file (myt, file), "Microsoft Word 2007+") ||
       strcasestr (magic_file (myt, file), "Rich Text Format") ||
       strcasestr (magic_file (myt, file), "(Corel/WP)") ||
       strcasestr (magic_file (myt, file), "Composite Document File") ||
       strcasestr (magic_file (myt, file), "OpenDocument") ||
       strcasestr (magic_file (myt, file), "UTF-8 Unicode"))
   {
      pdf_to_html (lowriter_to (file, "PDF"));
      pandoc_to_epub ("out.html");
   } // if
   else
// use calibre
   if (strcasestr (magic_file (myt, file), "Microsoft Reader eBook") ||
       strcasestr (magic_file (myt, file), "AportisDoc") ||
       strcasestr (magic_file (myt, file), "Mobipocket") ||
       strcasestr (magic_file (myt, file), "BBeB ebook data") ||
       strcasestr (magic_file (myt, file), "GutenPalm zTXT") ||
       strcasestr (magic_file (myt, file), "Plucker") ||
       strcasestr (magic_file (myt, file), "PostScript document") ||
       strcasestr (magic_file (myt, file), "PeanutPress PalmOS") ||
       strcasestr (magic_file (myt, file), "MS Windows HtmlHelp Data"))
   {
      char cmd[512];

      snprintf (cmd, MAX_CMD - 1,
                "ebook-convert \"%s\" out.epub > /dev/null", file);
      switch (system (cmd))
      {
      case 0:
         break;
      default:
         endwin ();
         beep ();
         puts (gettext ("eBook-speaker needs the calibre package."));
         fflush (stdout);
         _exit (1);
      } // switch
      read_epub ("out.epub");
   }
   else
// use tesseract
   if (strcasestr (magic_file (myt, file), "JPEG image")  ||
       strcasestr (magic_file (myt, file), "GIF image")  ||
       strcasestr (magic_file (myt, file), "PNG image")  ||
       strcasestr (magic_file (myt, file), "Netpbm"))
   {
      if (access ("/usr/share/doc/tesseract-ocr", R_OK) != 0)
      {
         endwin ();
         beep ();
         puts (gettext ("eBook-speaker needs the tesseract-ocr package."));
         printf (gettext ("Please install it and try again.\n"));
         fflush (stdout);
         _exit (1);
      } // if
      start_tesseract (file);
   } // if
   else
   {
      endwin ();
      beep ();
      printf ("%s: %s\n", file, magic_file (myt, file));
      printf (gettext ("Unknown format\n"));
      fflush (stdout);
      _exit (1);
   } // if
   magic_close (myt);
   read_daisy_3 (get_current_dir_name ());
   check_phrases ();

   wclear (misc.titlewin);
   mvwprintw (misc.titlewin, 0, 0, misc.copyright);
   if ((int) strlen (misc.daisy_title) + (int)
       strlen (misc.copyright) >= misc.max_x)
      mvwprintw (misc.titlewin, 0, misc.max_x - strlen (misc.daisy_title) - 4, "... ");
   mvwprintw (misc.titlewin, 0, misc.max_x - strlen (misc.daisy_title),
              "%s", misc.daisy_title);
   mvwaddstr (misc.titlewin, 1, 0, "----------------------------------------");
   waddstr (misc.titlewin, "----------------------------------------");
   mvwprintw (misc.titlewin, 1, 0, gettext ("Press 'h' for help "));
   wrefresh (misc.titlewin);
   misc.level = 1;
   *misc.search_str = 0;
   browse (file);
   return 0;
} // main