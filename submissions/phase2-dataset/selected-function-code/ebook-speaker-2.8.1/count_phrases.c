int count_phrases (char *f_file, char *f_anchor,
                   char *t_file, char *t_anchor)
{
   int n_phrases;
   xmlTextReaderPtr r;
   xmlDocPtr d;

   if (! *f_file)
      return 0;
   d = xmlRecoverFile (f_file);
   if (! (r = xmlReaderWalker (d)))
   {
      int e;

      e = errno;
      endwin ();
      printf ("%s\n", strerror (e));
      beep ();
      fflush (stdout);
      _exit (1);
   } // if
   n_phrases = 0;
   if (*f_anchor)
   {
// if there is a "from" anchor, look for it
      do
      {
         if (! get_tag_or_label (r))
         {
// if f_anchor couldn't be found, start from the beginning
            xmlTextReaderClose (r);
            xmlFreeDoc (d);
            d = xmlRecoverFile (f_file);
            r = xmlReaderWalker (d);
            break;
         } // if
      } while (strcasecmp (my_attribute.id, f_anchor) != 0);
   } // if

// start counting
   while (1)
   {
      if (! get_tag_or_label (r))
      {
         xmlTextReaderClose (r);
         xmlFreeDoc (d);
         return n_phrases + 1;
      } // if
      if (*misc.label)
      {
         n_phrases++;
         continue;
      } // if
      if (*t_anchor)
      {
         if  (strcasecmp (f_file, t_file) == 0)
         {
            if (strcasecmp (my_attribute.id, t_anchor) == 0)
            {
               xmlTextReaderClose (r);
               xmlFreeDoc (d);
               return n_phrases;
            } // if
         } // if
      } // if
   } // while
} // count_phrases