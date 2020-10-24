char *re_organize (char *fname)
{
   xmlTextReaderPtr local_reader;
   static char tmp[MAX_STR];
   char *p, text[max_phrase_len];
   xmlDocPtr doc;
   FILE *w;
   int phrases;

   doc = xmlRecoverFile (fname);
   if (! (local_reader = xmlReaderWalker (doc)))
   {
      int e;

      e = errno;
      endwin ();
      printf ("%s: %s\n", fname, strerror (e));
      beep ();
      fflush (stdout);
      _exit (1);
   } // if
   snprintf (tmp, MAX_STR - 1, "%s/%s.tmp.xhtml", misc.tmp_dir, basename (fname));
   if (! (w = fopen (tmp, "w")))
   {
      int e;

      e = errno;
      endwin ();
      printf ("%s\n", strerror (e));
      beep ();
      fflush (stdout);
      _exit (1);
   } /* if */
   fprintf (w, "<html>\n<head>\n</head>\n<body>\n");

   while (1)
   {
// discard header
      if (! get_tag_or_label (local_reader))
         break;
      if (! strcasecmp (misc.tag, "/head"))
         break;
   } // while

   while (1)
   {
      if (! get_tag_or_label (local_reader))
         break;
      if (strcasecmp (misc.tag, "a") == 0)
         continue;
      if (strcasecmp (misc.tag, "/a") == 0)
         continue;
      if (strcasecmp (misc.tag, "h1") == 0 ||
          strcasecmp (misc.tag, "h2") == 0 ||
          strcasecmp (misc.tag, "h3") == 0 ||
          strcasecmp (misc.tag, "h4") == 0 ||
          strcasecmp (misc.tag, "h5") == 0 ||
          strcasecmp (misc.tag, "h6") == 0)
      {
         fprintf (w, "<%s id=\"%s\">\n<br />\n", misc.tag, my_attribute.id);
         continue;
      } // if
      if (strcasecmp (misc.tag, "/h1") == 0 ||
          strcasecmp (misc.tag, "/h2") == 0 ||
          strcasecmp (misc.tag, "/h3") == 0 ||
          strcasecmp (misc.tag, "/h4") == 0 ||
          strcasecmp (misc.tag, "/h5") == 0 ||
          strcasecmp (misc.tag, "/h6") == 0)
      {
         fprintf (w, "<%s>\n<br />\n", misc.tag);
         continue;
      } // if
      if (! *misc.label)
         continue;
      p = misc.label;
      *text = 0;
      phrases = 0;
      while (1)
      {
         if (*p == 0)
         {
            char *t;

            t = text;
            while (1)
            {
               if (*t == ' ')
                  t++;
               if (*t != ' ')
                  break;
            } // while
            if (*t)
            {
               fprintf (w, "%s\n<br />\n", t);
               phrases++;
            } // if
            *text = 0;
            break;
         } // if
         if (*p == '\"' || *p == '\'' || *p == '`')
         {
            strncat (text, " ", 1);
            p++;
            continue;
         } // if
         if (*p == '&')
         {
            strncat (text, " ampersand ", 11);
            p++;
            continue;
         } // if
         if (*p == '.' || *p == ',' || *p == '!' || *p == '?' ||
             *p == ':' || *p == ';' || *p == '-' || *p == '*')
         {
         // split point; start here a new phrase
            strncat (text, p++, 1);
            if (strlen (text) > 1)
            {
               fprintf (w, "%s\n", text);
               phrases++;
            } // if
            fprintf (w, "<br />\n");
            *text = 0;
            continue;
         } // if
         strncat (text, p++, 1);
      } // while
   } // while
   fprintf (w, "</body>\n</html>\n");
   xmlTextReaderClose (local_reader);
   xmlFreeDoc (doc);
   fclose (w);
   return tmp;
} // re_organize