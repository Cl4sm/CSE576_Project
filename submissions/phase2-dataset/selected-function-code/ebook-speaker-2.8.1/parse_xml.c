void parse_xml (char *name)
{
   xmlTextReaderPtr xml;
   int indent = 0;
   char xml_name[MAX_STR];

   strncpy (xml_name, name, MAX_STR - 1);
   xmlDocPtr doc = xmlRecoverFile (misc.ncx_name);
   if (! (xml = xmlReaderWalker (doc)))
   {
      endwin ();
      beep ();
      printf (gettext ("\nCannot read %s\n"), misc.ncx_name);
      fflush (stdout);
      _exit (1);
   } // if
   misc.total_pages = misc.depth = 0;
   while (1)
   {
// get dtb:totalPageCount
      if (! get_tag_or_label (xml))
         break;
   } // while
   xmlTextReaderClose (xml);
   xmlFreeDoc (doc);

   doc = xmlRecoverFile (xml_name);
   if (! (xml = xmlReaderWalker (doc)))
   {
      endwin ();
      beep ();
      printf (gettext ("\nCannot read %s\n"), xml_name);
      fflush (stdout);
      _exit (1);
   } // if
   misc.current = 0;
   while (1)
   {
      if (! get_tag_or_label (xml))
         break;
      if (strcasecmp (misc.tag, "pagenum") == 0)
      {
         do
         {
            if (! get_tag_or_label (xml))
               break;
         } while (! *misc.label);
         daisy[misc.current].page_number = atoi (misc.label);
      } // if pagenum
      if (strcasecmp (misc.tag, "h1") == 0 ||
          strcasecmp (misc.tag, "h2") == 0 ||
          strcasecmp (misc.tag, "h3") == 0 ||
          strcasecmp (misc.tag, "h4") == 0 ||
          strcasecmp (misc.tag, "h5") == 0 ||
          strcasecmp (misc.tag, "h6") == 0)
      {                       
         int l;

         l = misc.tag[1] - '0';
         if (l > misc.depth)
            misc.depth = l;
         daisy[misc.current].level = l;
         daisy[misc.current].x = l + 3 - 1;
         indent = daisy[misc.current].x = (l - 1) * 3 + 1;
         if (! *my_attribute.smilref)
            continue;
         strncpy (daisy[misc.current].smil_file,
                  xml_name, MAX_STR - 1);
         if (strchr (my_attribute.smilref, '#'))
         {
            strncpy (daisy[misc.current].anchor,
                     strchr (my_attribute.smilref, '#') + 1,
                     MAX_STR - 1);
         } // if
         do
         {
            if (! get_tag_or_label (xml))
               break;
         } while (*misc.label == 0);
         get_label (misc.current, indent);
         misc.current++;
         misc.displaying++;
      } // if (strcasecmp (misc.tag, "h1") == 0 || ...
   } // while
   xmlTextReaderClose (xml);
   xmlFreeDoc (doc);
} // parse_xml