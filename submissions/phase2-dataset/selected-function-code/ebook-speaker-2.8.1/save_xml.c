void save_xml ()
{
   int x = 0;
   char str[MAX_STR];
   xmlTextWriterPtr writer;
   struct passwd *pw;

   pw = getpwuid (geteuid ());
   snprintf (str, MAX_STR - 1, "%s/.eBook-speaker.xml", pw->pw_dir);
   if (! (writer = xmlNewTextWriterFilename (str, 0)))
      return;
   xmlTextWriterStartDocument (writer, NULL, NULL, NULL);
   xmlTextWriterStartElement (writer, BAD_CAST "eBook-speaker");
   xmlTextWriterWriteString (writer, BAD_CAST "\n   ");
   xmlTextWriterStartElement (writer, BAD_CAST "prefs");
   xmlTextWriterWriteFormatAttribute
      (writer, BAD_CAST "sound_dev", "%s", misc.sound_dev);
   xmlTextWriterWriteFormatAttribute
      (writer, BAD_CAST "ocr_language", "%s", misc.ocr_language);
   xmlTextWriterEndElement (writer);
   xmlTextWriterWriteString (writer, BAD_CAST "\n   ");
   xmlTextWriterStartElement (writer, BAD_CAST "voices");
   while (1)
   {
      char str[MAX_STR];

      xmlTextWriterWriteString (writer, BAD_CAST "\n      ");
      xmlTextWriterStartElement (writer, BAD_CAST "tts");
      snprintf (str, MAX_STR - 1, "\n         %s\n      ", misc.tts[x]);
      xmlTextWriterWriteString (writer, BAD_CAST str);
      xmlTextWriterEndElement (writer);
      if (! *misc.tts[++x])
         break;
   } // while
   xmlTextWriterWriteString (writer, BAD_CAST "\n   ");
   xmlTextWriterEndElement (writer);
   xmlTextWriterWriteString (writer, BAD_CAST "\n");
   xmlTextWriterEndElement (writer);
   xmlTextWriterEndDocument (writer);
   xmlFreeTextWriter (writer);
} // save_xml