void parse_opf (char *name)
{
   xmlTextReaderPtr opf;

   xmlDocPtr doc = xmlRecoverFile (name);
   if (! (opf = xmlReaderWalker (doc)))
   {
      endwin ();
      beep ();
      printf (gettext ("\nCannot read %s\n"), name);
      fflush (stdout);
      _exit (1);
   } // if
   misc.current = misc.displaying = 0;
   while (1)
   {
      if (! get_tag_or_label (opf))
         break;
      if (strcasecmp (misc.tag, "dc:language") == 0)
      {
         do
         {
            if (! get_tag_or_label (opf))
               break;
         } while (! *misc.label);
         strncpy (misc.daisy_language, misc.label, MAX_STR - 1);
      } // if dc:language
      if (strcasecmp (misc.tag, "dtb:totalTime") == 0)
      {
         do
         {
            if (! get_tag_or_label (opf))
               break;
         } while (! *misc.label);
         strncpy (misc.ncc_totalTime, misc.label, MAX_STR - 1);
      } // if (strcasestr (misc.tag, "dtb:totalTime") == 0)
      if (strcasecmp (misc.tag, "dc:title") == 0 && ! *misc.daisy_title)
      {
         do
         {
            if (! get_tag_or_label (opf))
               break;
         } while (! *misc.label);
         strncpy (misc.daisy_title, misc.label, MAX_STR - 1);
         strncpy (misc.bookmark_title, misc.label, MAX_STR - 1);
         if (strchr (misc.bookmark_title, '/'))
            *strchr (misc.bookmark_title, '/') = '-';
      } // if dc:title
      if (strcasecmp (my_attribute.media_type,
                      "application/x-dtbook+xml") == 0)
      {
         parse_xml (my_attribute.href);
         xmlTextReaderClose (opf);
         xmlFreeDoc (doc);
         return;
      } // if "application/x-dtbook+xml"
      if (strcasecmp (misc.tag, "spine") == 0)
      {
         if (*my_attribute.toc)
         {
            parse_ncx (misc.ncx_name);
            if (misc.total_phrases > 0)
            {
               xmlTextReaderClose (opf);
               xmlFreeDoc (doc);
               return;
            } // if
         } // if
         do
         {
            if (! get_tag_or_label (opf))
               break;
            if (strcasecmp (misc.tag, "itemref") == 0)
               parse_manifest (name, my_attribute.idref);
         } while (strcasecmp (misc.tag, "/spine") != 0);
      } // if spine
   } // while
   misc.total_items = misc.current;
   xmlTextReaderClose (opf);
   xmlFreeDoc (doc);
} // parse_opf