void parse_text_file (char *text_file)
// page-number
{
   xmlTextReaderPtr textptr;
   xmlDocPtr doc;
   char *anchor = 0;

   if (strchr (text_file, '#'))
   {
      anchor = strdup (strchr (text_file, '#') + 1);
      *strchr (text_file, '#') = 0;
   } // if
   doc = xmlRecoverFile (text_file);
   if (! (textptr = xmlReaderWalker (doc)))
   {
      endwin ();
      beep ();
      printf (gettext ("\nCannot read %s\n"), text_file);
      fflush (stdout);
      _exit (1);
   } // if
   if (anchor)
   {
      do
      {
         if (! get_tag_or_label (textptr))
         {
            xmlTextReaderClose (textptr);
            xmlFreeDoc (doc);
            return;
         } // if
      } while (strcasecmp (my_attribute.id, anchor) != 0);
   } // if
   do
   {
      if (! get_tag_or_label (textptr))
      {
         xmlTextReaderClose (textptr);
         xmlFreeDoc (doc);
         return;
      } // if
   } while (! *misc.label);
   daisy[misc.current].page_number = atoi (misc.label);
   xmlTextReaderClose (textptr);
   xmlFreeDoc (doc);
} // parse_text_file