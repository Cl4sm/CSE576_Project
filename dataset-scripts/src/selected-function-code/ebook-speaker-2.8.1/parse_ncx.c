void parse_ncx (char *name)
{
   int indent, found_page = 0;
   xmlTextReaderPtr ncx;
   xmlDocPtr doc;

   doc = xmlRecoverFile (name);
   if (! (ncx = xmlReaderWalker (doc)))
   {
      endwin ();
      beep ();
      printf (gettext ("\nCannot read %s\n"), name);
      fflush (stdout);
      _exit (1);
   } // if
   misc.current = misc.displaying = misc.level = misc.depth = 0;
   while (1)
   {
      if (! get_tag_or_label (ncx))
         break;
      if (strcasecmp (misc.tag, "docTitle") == 0)
      {
         do
         {
            if (! get_tag_or_label (ncx))
               break;
         } while (! *misc.label);
         strncpy (misc.daisy_title, misc.label, MAX_STR - 1);
         strncpy (misc.bookmark_title, misc.label, MAX_STR - 1);
         if (strchr (misc.bookmark_title, '/'))
            *strchr (misc.bookmark_title, '/') = '-';
      } // if (strcasecmp (misc.tag, "docTitle") == 0)
      if (strcasecmp (misc.tag, "docAuthor") == 0)
      {
         do
         {
            if (! get_tag_or_label (ncx))
               break;
         } while (strcasecmp (misc.tag, "/docAuthor") != 0);
      } // if (strcasecmp (misc.tag, "docAuthor") == 0)
      if (strcasecmp (misc.tag, "navPoint") == 0)
      {
         misc.level++;
         if (misc.level > misc.depth)
            misc.depth = misc.level;
         daisy[misc.current].page_number = 0;
         daisy[misc.current].playorder = atoi (my_attribute.playorder);
         daisy[misc.current].level = misc.level;
         if (daisy[misc.current].level < 1)
            daisy[misc.current].level = 1;
         indent = daisy[misc.current].x = misc.level * 3 - 2;
         strncpy (daisy[misc.current].class, my_attribute.class, MAX_STR - 1);
         while (1)
         {
            if (! get_tag_or_label (ncx))
               break;
            if (strcasecmp (misc.tag, "text") == 0)
            {
               while (1)
               {
                  if (! get_tag_or_label (ncx))
                     break;
                  if (strcasecmp (misc.tag, "/text") == 0)
                     break;
                  if (*misc.label)
                  {
                     get_label (misc.current, indent);
                     break;
                  } // if
               } // while
            } // if (strcasecmp (misc.tag, "text") == 0)
            if (strcasecmp (misc.tag, "content") == 0)
            {
               if (*my_attribute.src)
               {
                  parse_content (my_attribute.src);
                  misc.current++;
               } // if
               break;
            } // if (strcasecmp (misc.tag, "content") == 0)
         } // while
      } // if (strcasecmp (misc.tag, "navPoint") == 0)
      if (strcasecmp (misc.tag, "/navpoint") == 0)
         misc.level--;
      if (strcasecmp (misc.tag, "page") == 0 || found_page)
      {
         found_page = 0;
         strncpy (daisy[++misc.current].label, my_attribute.number,
                  max_phrase_len - 1);
         strncpy (daisy[misc.current].smil_file, name, MAX_STR - 1);
         daisy[misc.current].screen = misc.current / misc.max_y;
         daisy[misc.current].y = misc.current - (daisy[misc.current].screen * misc.max_y);
         daisy[misc.current].x = 2;
      } // if (strcasecmp (misc.tag, "page") == 0)
   } // while
   xmlTextReaderClose (ncx);
   xmlFreeDoc (doc);
   misc.total_items = misc.current;
#ifdef EBOOK_SPEAKER
   check_phrases ();
#endif
} // parse_ncx