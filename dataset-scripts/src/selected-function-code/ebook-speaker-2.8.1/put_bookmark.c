void put_bookmark ()
{
   char str[MAX_STR];
   xmlTextWriterPtr writer;
   struct passwd *pw;;

   pw = getpwuid (geteuid ());
   snprintf (str, MAX_STR - 1, "%s/.eBook-speaker", pw->pw_dir);
   mkdir (str, 0755);
   snprintf (str, MAX_STR - 1, "%s/.eBook-speaker/%s",
             pw->pw_dir, misc.bookmark_title);
   if (! (writer = xmlNewTextWriterFilename (str, 0)))
      return;
   xmlTextWriterStartDocument (writer, NULL, NULL, NULL);
   xmlTextWriterStartElement (writer, BAD_CAST "bookmark");
   if (misc.playing < 0)
      xmlTextWriterWriteFormatAttribute
         (writer, BAD_CAST "item", "%d", misc.current);
   else                                               
      xmlTextWriterWriteFormatAttribute
         (writer, BAD_CAST "item", "%d", misc.playing);
   xmlTextWriterWriteFormatAttribute
      (writer, BAD_CAST "phrase", "%d", --misc.phrase_nr);
   xmlTextWriterWriteFormatAttribute (writer, BAD_CAST "level", "%d", misc.level);
   xmlTextWriterWriteFormatAttribute (writer, BAD_CAST "tts", "%d", misc.tts_no);
   xmlTextWriterWriteFormatAttribute (writer, BAD_CAST "speed", "%f", misc.speed);
   xmlTextWriterEndElement (writer);
   xmlTextWriterEndDocument (writer);
   xmlFreeTextWriter (writer);
} // put_bookmark