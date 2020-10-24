void parse_manifest (char *name, char *id_ptr)
{
   xmlTextReaderPtr manifest;
   xmlDocPtr doc;
   char *id, *toc;

   id = strdup (id_ptr);
   if (! *id)
   {
      free (id);
      return;
   } // if   
   toc = strdup (my_attribute.toc);
   doc = xmlRecoverFile (name);
   if (! (manifest = xmlReaderWalker (doc)))
   {
      endwin ();
      beep ();
      printf (gettext ("\nCannot read %s\n"), name);
      fflush (stdout);
      _exit (1);
   } // if
   do
   {
      if (! get_tag_or_label (manifest))
         break;
      if (strcasecmp (misc.tag, "dc:language") == 0  && ! *misc.daisy_language)
      {
         do
         {
            if (! get_tag_or_label (manifest))
               break;
            if (*misc.label)
               break;
         } while (strcasecmp (misc.tag, "/dc:language") != 0);
         strncpy (misc.daisy_language, misc.label, MAX_STR - 1);
      } // if dc:language
      if (strcasecmp (misc.tag, "dc:title") == 0 && ! *misc.daisy_title)
      {
         do
         {
            if (! get_tag_or_label (manifest))
               break;
            if (*misc.label)
               break;
         } while (strcasecmp (misc.tag, "/dc:title") != 0);
         strncpy (misc.daisy_title, misc.label, MAX_STR - 1);
         strncpy (misc.bookmark_title, misc.label, MAX_STR - 1);
         if (strchr (misc.bookmark_title, '/'))
            *strchr (misc.bookmark_title, '/') = '-';
      } // if dc:title
   } while (strcasecmp (misc.tag, "manifest") != 0);
   while (1)
   {
      if (! get_tag_or_label (manifest))
         break;
      if (*toc && strcasecmp (my_attribute.id, id) == 0)
      {
         parse_ncx (my_attribute.href);
         xmlTextReaderClose (manifest);
         xmlFreeDoc (doc);
         return;
      } // if toc
      if (*id && strcasecmp (my_attribute.id, id) == 0)
      {
         strncpy (daisy[misc.current].smil_file, my_attribute.href,
                  MAX_STR - 1);
         snprintf (daisy[misc.current].label, MAX_STR - 1, "%d", misc.current + 1);
         daisy[misc.current].screen = misc.current / misc.max_y;
         daisy[misc.current].y = misc.current - (daisy[misc.current].screen * misc.max_y);
         daisy[misc.current].x = 1;
         misc.current++;
      } // if (strcasecmp (my_attribute.id, id) == 0)
   } // while
   xmlTextReaderClose (manifest);
   xmlFreeDoc (doc);
} // parse_manifest