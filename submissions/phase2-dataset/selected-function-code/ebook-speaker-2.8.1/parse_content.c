void parse_content (char *src)
{
   int found_anchor, found_text;
   char text_file[MAX_STR];
   xmlTextReaderPtr content;

   strncpy (daisy[misc.current].smil_file, src, MAX_STR - 1);
   if (strchr (daisy[misc.current].smil_file, '#'))
   {
      strncpy (daisy[misc.current].anchor,
               strchr (daisy[misc.current].smil_file, '#') + 1, MAX_STR - 1);
      *strchr (daisy[misc.current].smil_file, '#') = 0;
   } // if
   xmlDocPtr doc = xmlRecoverFile (daisy[misc.current].smil_file);
   if (! (content = xmlReaderWalker (doc)))
   {
      endwin ();
      beep ();
      printf (gettext ("\nCannot read %s\n"), daisy[misc.current].smil_file);
      fflush (stdout);
      _exit (1);
   } // if
   found_anchor = found_text = 0;
   while (1)
   {
      if (! get_tag_or_label (content))
      {
         xmlTextReaderClose (content);
         xmlFreeDoc (doc);
         return;
      } // if
      if (strcasecmp (misc.tag, "text") == 0)
      {
         found_text = 1;
         strncpy (text_file, my_attribute.src, MAX_STR - 1);
      } // if
      if (strcasecmp (my_attribute.id, daisy[misc.current].anchor) == 0)
         found_anchor = 1;
      if (found_anchor && found_text)
         break;
   } // while
   parse_text_file (text_file);
   xmlTextReaderClose (content);
   xmlFreeDoc (doc);
} // parse_content