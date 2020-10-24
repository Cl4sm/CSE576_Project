xmlTextReaderPtr skip_left (xmlTextReaderPtr reader, xmlDocPtr doc)
{
   int old_phrase_nr;

   if (misc.playing == -1)
   {
      beep ();
      return NULL;
   } // if
   old_phrase_nr = misc.phrase_nr;
   kill_player ();
   if (misc.phrase_nr <= 2)
   {
      if (misc.playing == 0)
         return reader;
      misc.playing--;
      misc.displaying = misc.current = misc.playing;
      reader = open_text_file (reader, doc, daisy[misc.playing].smil_file,
                               daisy[misc.playing].anchor);
      misc.phrase_nr = 0;
      while (1)
      {
         if (! get_tag_or_label (reader))
            return NULL;
         if (! *misc.label)
            continue;
         if (misc.phrase_nr >= daisy[misc.playing].n_phrases - 2)
            return reader;
         misc.phrase_nr++;
      } // while
   } // if
   reader = open_text_file (reader, doc, daisy[misc.playing].smil_file,
                            daisy[misc.playing].anchor);
   misc.phrase_nr = 0;
   while (1)
   {
      if (! get_tag_or_label (reader))
         return NULL;
      if (! *misc.label)
         continue;
      if (++misc.phrase_nr >= old_phrase_nr - 2)
         return reader;
   } // while
} // skip_left