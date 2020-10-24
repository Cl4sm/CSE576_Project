void start_tesseract (char *file)
{
   char cmd[MAX_CMD];

   if (*misc.ocr_language)
      snprintf (cmd, MAX_CMD - 1,
                "tesseract \"%s\" out -l %s 2> /dev/null", file,
                misc.ocr_language);
   else
      snprintf (cmd, MAX_CMD - 1,
                "tesseract \"%s\" out 2> /dev/null", file);
   switch (system (cmd))
   {
   case 0:
      break;
   default:
      endwin ();
      beep ();
      puts (misc.copyright);
      printf (gettext (
  "Language code \"%s\" is not a valid tesseract code.\n"), misc.ocr_language);
      puts (gettext ("See the tesseract manual for valid codes."));
      fflush (stdout);
      _exit (1);
   } // switch
   pandoc_to_epub ("out.txt");
} // start_tesseract