void get_attributes (xmlTextReaderPtr reader)
{
   char attr[MAX_STR];

   snprintf (attr, MAX_STR - 1, "%s", (char*)
        xmlTextReaderGetAttribute (reader, (const xmlChar *) "class"));
   if (strcmp (attr, "(null)"))
      snprintf (my_attribute.class, MAX_STR - 1, "%s", attr);
   snprintf (attr, MAX_STR - 1, "%s", (char*)
       xmlTextReaderGetAttribute (reader, (const xmlChar *) "clip-begin"));
   if (strcmp (attr, "(null)"))
      snprintf (my_attribute.clip_begin, MAX_STR - 1,
                "%s", attr);
   snprintf (attr, MAX_STR - 1, "%s", (char*)
             xmlTextReaderGetAttribute (reader,
                    (const xmlChar *) "clipBegin"));
   if (strcmp (attr, "(null)"))
      snprintf (my_attribute.clip_begin, MAX_STR - 1, "%s", attr);
   snprintf (attr, MAX_STR - 1, "%s", (char*)
       xmlTextReaderGetAttribute (reader, (const xmlChar *) "clip-end"));
   if (strcmp (attr, "(null)"))
      snprintf (my_attribute.clip_end, MAX_STR - 1, "%s", attr);
   snprintf (attr, MAX_STR - 1, "%s", (char*)
       xmlTextReaderGetAttribute (reader, (const xmlChar *) "clipEnd"));
   if (strcmp (attr, "(null)"))
      snprintf (my_attribute.clip_end, MAX_STR - 1, "%s", attr);
   snprintf (attr, MAX_STR - 1, "%s", (char*)
       xmlTextReaderGetAttribute (reader, (const xmlChar *) "href"));
   if (strcmp (attr, "(null)"))
      snprintf (my_attribute.href, MAX_STR - 1, "%s", attr);
   snprintf (attr, MAX_STR - 1, "%s", (char*)
       xmlTextReaderGetAttribute (reader, (const xmlChar *) "id"));
   if (strcmp (attr, "(null)"))
      snprintf (my_attribute.id, MAX_STR - 1, "%s", attr);
   snprintf (attr, MAX_STR - 1, "%s", (char*)
       xmlTextReaderGetAttribute (reader, (const xmlChar *) "idref"));
   if (strcmp (attr, "(null)"))
      snprintf (my_attribute.idref, MAX_STR - 1, "%s", attr);
   snprintf (attr, MAX_STR - 1, "%s", (char*)
       xmlTextReaderGetAttribute (reader, (const xmlChar *) "item"));
   if (strcmp (attr, "(null)"))
      misc.current = atoi (attr);
   snprintf (attr, MAX_STR - 1, "%s", (char*)
       xmlTextReaderGetAttribute (reader, (const xmlChar *) "level"));
   if (strcmp (attr, "(null)"))
      misc.level = atoi ((char *) attr);
   snprintf (attr, MAX_STR - 1, "%s", (char*)
       xmlTextReaderGetAttribute (reader, (const xmlChar *) "media-type"));
   if (strcmp (attr, "(null)"))
      snprintf (my_attribute.media_type, MAX_STR - 1, "%s", attr);
   snprintf (attr, MAX_STR - 1, "%s", (char*)
       xmlTextReaderGetAttribute (reader, (const xmlChar *) "name"));
   if (strcmp (attr, "(null)"))
   {
      char name[MAX_STR], content[MAX_STR];

      *name = 0;
      snprintf (attr, MAX_STR - 1, "%s", (char *)
                xmlTextReaderGetAttribute (reader, (const xmlChar *) "name"));
      if (strcmp (attr, "(null)"))
         snprintf (name, MAX_STR - 1, "%s", attr);
      *content = 0;
      snprintf (attr, MAX_STR - 1, "%s", (char *)
                xmlTextReaderGetAttribute (reader, (const xmlChar *) "content"));
      if (strcmp (attr, "(null)"))
         snprintf (content, MAX_STR - 1, "%s", attr);
      if (strcasestr (name, "dc:format"))
         snprintf (misc.daisy_version, MAX_STR - 1, "%s", content);
      if (strcasestr (name, "dc:title") && ! *misc.daisy_title)
      {
         snprintf (misc.daisy_title, MAX_STR - 1, "%s", content);
         snprintf (misc.bookmark_title, MAX_STR - 1, "%s", content);
         if (strchr (misc.bookmark_title, '/'))
            *strchr (misc.bookmark_title, '/') = '-';
      } // if
/* don't use it
      if (strcasestr (name, "dtb:misc.depth"))
         misc.depth = atoi (content);
*/
      if (strcasestr (name, "dtb:totalPageCount"))
         misc.total_pages = atoi (content);
      if (strcasestr (name, "ncc:misc.depth"))
         misc.depth = atoi (content);
      if (strcasestr (name, "ncc:maxPageNormal"))
         misc.total_pages = atoi (content);
      if (strcasestr (name, "ncc:pageNormal"))
         misc.total_pages = atoi (content);
      if (strcasestr (name, "ncc:page-normal"))
         misc.total_pages = atoi (content);
      if (strcasestr (name, "dtb:totalTime")  ||
          strcasestr (name, "ncc:totalTime"))
      {
         snprintf (misc.ncc_totalTime, MAX_STR - 1, "%s", content);
         if (strchr (misc.ncc_totalTime, '.'))
            *strchr (misc.ncc_totalTime, '.') = 0;
      } // if
   } // if
   snprintf (attr, MAX_STR - 1, "%s", (char*)
       xmlTextReaderGetAttribute (reader, (const xmlChar *) "playorder"));
   if (strcmp (attr, "(null)"))
      snprintf (my_attribute.playorder, MAX_STR - 1, "%s", attr);
   snprintf (attr, MAX_STR - 1, "%s", (char*)
       xmlTextReaderGetAttribute (reader, (const xmlChar *) "phrase"));
   if (strcmp (attr, "(null)"))
      misc.phrase_nr = atoi ((char *) attr);
   snprintf (attr, MAX_STR - 1, "%s", (char*)
       xmlTextReaderGetAttribute (reader, (const xmlChar *) "seconds"));
   if (strcmp (attr, "(null)"))
   {
      misc.seconds = atoi (attr);
      if (misc.seconds < 0)
         misc.seconds = 0;
   } // if
   snprintf (attr, MAX_STR - 1, "%s", (char*)
          xmlTextReaderGetAttribute (reader, (const xmlChar *) "smilref"));
   if (strcmp (attr, "(null)"))
      snprintf (my_attribute.smilref, MAX_STR - 1, "%s", attr);
   snprintf (attr, MAX_STR - 1, "%s", (char*)
       xmlTextReaderGetAttribute (reader, (const xmlChar *) "sound_dev"));
   if (strcmp (attr, "(null)"))
      snprintf (misc.sound_dev, MAX_STR, "%s", attr);
   snprintf (attr, MAX_STR - 1, "%s", (char*)
       xmlTextReaderGetAttribute (reader, (const xmlChar *) "ocr_language"));
   if (strcmp (attr, "(null)"))
      snprintf (misc.ocr_language, 5, "%s", attr);
   snprintf (attr, MAX_STR - 1, "%s", (char*)
       xmlTextReaderGetAttribute (reader, (const xmlChar *) "cd_dev"));
   if (strcmp (attr, "(null)"))
      snprintf (misc.cd_dev, MAX_STR, "%s", attr);
   snprintf (attr, MAX_STR - 1, "%s", (char*)
       xmlTextReaderGetAttribute (reader, (const xmlChar *) "cddb_flag"));
   if (strcmp (attr, "(null)"))
      misc.cddb_flag = (char) attr[0];
   snprintf (attr, MAX_STR - 1, "%s", (char*)
       xmlTextReaderGetAttribute (reader, (const xmlChar *) "speed"));
   if (strcmp (attr, "(null)"))
      misc.speed = atof (attr);
   snprintf (attr, MAX_STR - 1, "%s", (char*)
       xmlTextReaderGetAttribute (reader, (const xmlChar *) "src"));
   if (strcmp (attr, "(null)"))
      snprintf (my_attribute.src, MAX_STR - 1, "%s", attr);
   snprintf (attr, MAX_STR - 1, "%s", (char*)
       xmlTextReaderGetAttribute (reader, (const xmlChar *) "tts"));
   if (strcmp (attr, "(null)"))
      misc.tts_no = atof ((char *) attr);
   snprintf (attr, MAX_STR - 1, "%s", (char*)
       xmlTextReaderGetAttribute (reader, (const xmlChar *) "toc"));
   if (strcmp (attr, "(null)"))
      snprintf (my_attribute.toc, MAX_STR - 1, "%s", attr);
   snprintf (attr, MAX_STR - 1, "%s", (char*)
       xmlTextReaderGetAttribute (reader, (const xmlChar *) "value"));
   if (strcmp (attr, "(null)"))
      snprintf (my_attribute.value, MAX_STR - 1, "%s", attr);
} // get_attributes