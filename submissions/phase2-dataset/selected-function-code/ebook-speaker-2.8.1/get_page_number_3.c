void get_page_number_3 (xmlTextReaderPtr reader)
{
   xmlDocPtr doc;
   xmlTextReaderPtr page;
   char *anchor = 0;

   do
   {
      if (! get_tag_or_label (reader))
         return;
   } while (strcasecmp (misc.tag, "text") != 0);
   if (strchr (my_attribute.src, '#'))
   {
      anchor = strdup (strchr (my_attribute.src, '#') + 1);
      *strchr (my_attribute.src, '#') = 0;
   } // if
   doc = xmlRecoverFile (my_attribute.src);
   if (! (page = xmlReaderWalker (doc)))
   {
      endwin ();
      beep ();
      printf (gettext ("\nCannot read %s\n"), my_attribute.src);
      fflush (stdout);
      _exit (1);
   } // if
   do
   {
      if (! get_tag_or_label (page))
      {
         xmlTextReaderClose (page);
         xmlFreeDoc (doc);
         return;
      } // if
   } while (strcasecmp (my_attribute.id, anchor) != 0);
   do
   {
      if (! get_tag_or_label (page))
      {
         xmlTextReaderClose (page);
         xmlFreeDoc (doc);
         return;
      } // if
   } while (! *misc.label);
   xmlTextReaderClose (page);
   xmlFreeDoc (doc);
   misc.current_page_number = atoi (misc.label);
} // get_page_number_3