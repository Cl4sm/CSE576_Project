void parse_smil_3 ()
{
   int x;
   xmlTextReaderPtr parse;
   xmlDocPtr doc;

   misc.total_time = 0;
   for (x = 0; x < misc.total_items; x++)
   {
      if (*daisy[x].smil_file == 0)
         continue;
      doc = xmlRecoverFile (daisy[x].smil_file);
      if (! (parse = xmlReaderWalker (doc)))
      {
         endwin ();
         beep ();
         printf (gettext ("\nCannot read %s\n"), daisy[x].smil_file);
         fflush (stdout);
         _exit (1);
      } // if

// parse this smil
      while (1)
      {
         if (! get_tag_or_label (parse))
            break;
         if (strcasecmp (daisy[x].anchor, my_attribute.id) == 0)
            break;
      } // while
      daisy[x].duration = 0;
      while (1)
      {
         if (! get_tag_or_label (parse))
            break;
// get clip_begin
         if (strcasecmp (misc.tag, "audio") == 0)
         {
            get_clips (my_attribute.clip_begin, my_attribute.clip_end);
            daisy[x].begin = misc.clip_begin;
            daisy[x].duration += misc.clip_end - misc.clip_begin;
// get clip_end
            while (1)
            {
               if (! get_tag_or_label (parse))
                  break;
               if (*daisy[x + 1].anchor)
                  if (strcasecmp (my_attribute.id, daisy[x + 1].anchor) == 0)
                     break;
               if (strcasecmp (misc.tag, "audio") == 0)
               {
                  get_clips (my_attribute.clip_begin, my_attribute.clip_end);
                  daisy[x].duration += misc.clip_end - misc.clip_begin;
               } // IF
            } // while
            if (*daisy[x + 1].anchor)
               if (strcasecmp (my_attribute.id, daisy[x + 1].anchor) == 0)
                  break;
         } // if (strcasecmp (misc.tag, "audio") == 0)
      } // while
      misc.total_time += daisy[x].duration;
      xmlTextReaderClose (parse);
      xmlFreeDoc (doc);
   } // for
} // parse_smil_3