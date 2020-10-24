xmlTextReaderPtr open_text_file (xmlTextReaderPtr reader, xmlDocPtr doc,
                                 char *text_file, char *anchor)
{
   xmlTextReaderClose (reader);
   xmlFreeDoc (doc);
   doc = xmlRecoverFile (text_file);
   if ((reader = xmlReaderWalker (doc)) == NULL)
   {
      endwin ();
      beep ();
      printf ("open_text_file(): %s\n", text_file);
      fflush (stdout);
      _exit (1);
   } // if
   do
   {
      if (! get_tag_or_label (reader))
         break;
   } while (strcasecmp (misc.tag, "body") != 0);
   if (*anchor == 0)
      return reader;

// look if anchor exists in this text_file
   while (1)
   {
      if (! get_tag_or_label (reader))
      {
// if anchor couldn't be found, start from the beginning
         xmlTextReaderClose (reader);
         xmlFreeDoc (doc);
         doc = xmlRecoverFile (text_file);
         reader = xmlReaderWalker (doc);
         break;
      } // if
      if (strcasecmp (anchor, my_attribute.id) == 0)
         break;
   } // while
   return reader;
} // open_text_file