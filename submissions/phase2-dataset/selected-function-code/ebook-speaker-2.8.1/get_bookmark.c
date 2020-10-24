xmlTextReaderPtr get_bookmark (xmlTextReaderPtr reader, xmlDocPtr doc)
{
   char str[MAX_STR];
   xmlTextReaderPtr local_reader;
   xmlDocPtr local_doc;
   int start;
   struct passwd *pw;;

   pw = getpwuid (geteuid ());
   snprintf (str, MAX_STR - 1, "%s/.eBook-speaker/%s",
             pw->pw_dir, misc.bookmark_title);
   local_doc = xmlRecoverFile (str);
   if (! (local_reader = xmlReaderWalker (local_doc)))
   {
      xmlFreeDoc (local_doc);
      return NULL;
   } // if
   do
   {
      if (! get_tag_or_label (local_reader))
         break;
   } while (strcasecmp (misc.tag, "bookmark") != 0);
   xmlTextReaderClose (local_reader);
   xmlFreeDoc (local_doc);
   if (misc.current < 0 || misc.current == misc.total_items)
      misc.current = 0;
   if (! *daisy[misc.current].smil_file)
      return NULL;
   reader = open_text_file (reader, doc,
                            daisy[misc.current].smil_file, daisy[misc.current].anchor);
   if (misc.phrase_nr < 0)
      misc.phrase_nr = 0;
   start = misc.phrase_nr;
   misc.phrase_nr = 0;
   if (misc.level < 1 || misc.level > misc.depth)
      misc.level = 1;
   misc.displaying = misc.playing = misc.current;
   misc.just_this_item = -1;
   if (misc.tts_flag > -1)
      misc.tts_no = misc.tts_flag;
   while (1)
   {
      if (! get_tag_or_label (reader))
         break;
      if (! *misc.label)
         continue;
      misc.phrase_nr++;
      if (misc.phrase_nr == start)
         break;
   } // while
   return reader;               
} // get_bookmark