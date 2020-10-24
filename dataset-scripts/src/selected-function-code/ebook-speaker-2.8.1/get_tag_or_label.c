int get_tag_or_label (xmlTextReaderPtr reader)
{
   int type;

   *misc.tag =  *misc.label = 0;
   *my_attribute.class = *my_attribute.clip_begin = *my_attribute.clip_end =
   *my_attribute.href = *my_attribute.id = *my_attribute.media_type =
   *my_attribute.playorder = * my_attribute.smilref = *my_attribute.src =
   *my_attribute.toc = 0, *my_attribute.value = 0;

   if (! reader)
      return 0;
   switch (xmlTextReaderRead (reader))
   {
   case -1:
   {
      int e;

      e = errno;
      endwin ();
      printf ("%s\n", strerror (e));
      printf ("Can't handle this DTB structure!\n");
      printf ("Don't know how to handle it yet, sorry. :-(\n");
      beep ();
      fflush (stdout);
      _exit (1);
   }
   case 0:
      return 0;
   case 1:
      break;
   } // swwitch
   type = xmlTextReaderNodeType (reader);
   switch (type)
   {
   int n, i;

   case -1:
      endwin ();
      beep ();
      printf (gettext ("\nCannot read type: %d\n"), type);
      fflush (stdout);
      _exit (1);
   case XML_READER_TYPE_ELEMENT:
      strncpy (misc.tag, (char *) xmlTextReaderConstName (reader),
               MAX_TAG - 1);
      n = xmlTextReaderAttributeCount (reader);
      for (i = 0; i < n; i++)
         get_attributes (reader);
      return 1;
   case XML_READER_TYPE_END_ELEMENT:
      snprintf (misc.tag, MAX_TAG - 1, "/%s",
                (char *) xmlTextReaderName (reader));
      return 1;
   case XML_READER_TYPE_TEXT:
   {
      int x;

      x = 0;
      while (1)
      {
         if (isspace (xmlTextReaderConstValue (reader)[x]))
            x++;
         else
            break;
      } // while
      strncpy (misc.label, (char *) xmlTextReaderConstValue (reader) + x,
                      max_phrase_len);
      for (x = strlen (misc.label) - 1; x >= 0 && isspace (misc.label[x]); x--)
         misc.label[x] = 0;
      for (x = 0; misc.label[x] > 0; x++)
         if (! isprint (misc.label[x]))
            misc.label[x] = ' ';
      return 1;
   }
   case XML_READER_TYPE_ENTITY_REFERENCE:
   case XML_READER_TYPE_DOCUMENT_TYPE:
   case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
      snprintf (misc.tag, MAX_TAG - 1, "/%s",
                (char *) xmlTextReaderName (reader));
      return 1;
   default:
      return 1;
   } // switch
   return 0;
} // get_tag_or_label