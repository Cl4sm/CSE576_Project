void get_page_number (char *NCX)
{
   xmlTextReaderPtr fd;
   xmlDocPtr doc;

   doc = xmlRecoverFile (NCX);
   fd = xmlReaderWalker (doc);
   do
   {
      if (! get_tag_or_label (fd))
         break;
   } while (atoi (my_attribute.playorder) != misc.current);
   do
   {
      if (! get_tag_or_label (fd))
         break;
   } while (! *misc.label);
   misc.current_page_number = atoi (misc.label);
   xmlTextReaderClose (fd);
   xmlFreeDoc (doc);
} // get_page_number