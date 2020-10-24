void read_xml ()
{
   int x = 0, i;
   char str[MAX_STR], *p;
   struct passwd *pw;;
   xmlTextReaderPtr local_reader;
   xmlDocPtr doc;

   pw = getpwuid (geteuid ());
   snprintf (str, MAX_STR - 1, "%s/.eBook-speaker.xml", pw->pw_dir);
   if ((doc = xmlRecoverFile (str)) == NULL)
   {
// If no TTS; give some examples
      strncpy (misc.tts[0], "espeak -f eBook-speaker.txt -w eBook-speaker.wav",
               MAX_STR - 1);
      strncpy (misc.tts[1], "flite eBook-speaker.txt eBook-speaker.wav",
               MAX_STR - 1);
      strncpy (misc.tts[2],
               "espeak -f eBook-speaker.txt -w eBook-speaker.wav -v mb-nl2",
               MAX_STR - 1);
      strncpy (misc.tts[3], "text2wave eBook-speaker.txt -o eBook-speaker.wav",
               MAX_STR - 1);
      strncpy (misc.tts[4], "swift -n Lawrence -f eBook-speaker.txt -m text -o eBook-speaker.wav",
               MAX_STR - 1);
      save_xml ();
      return;
   } // if
   if ((local_reader = xmlReaderWalker (doc)))
   {
      while (1)
      {
         if (! get_tag_or_label (local_reader))
            break;
         if (strcasecmp (misc.tag, "tts") == 0)
         {
            do
            {
               if (! get_tag_or_label (local_reader))
                  break;
            } while (! *misc.label);
            for (i = 0; misc.label[i]; i++)
               if (misc.label[i] != ' ' && misc.label[i] != '\n')
                  break;;
            p = misc.label + i;
            for (i = strlen (p) - 1; i > 0; i--)
               if (p[i] != ' ' && p[i] != '\n')
                  break;
            p[i + 1] = 0;
            strncpy (misc.tts[x++], p, MAX_STR - 1);
            if (x == 10)
               break;
         } // if
      } // while
      misc.tts_no = x;
      xmlTextReaderClose (local_reader);
      xmlFreeDoc (doc);
   } // if
   if (*misc.tts[0])
      return;

// If no TTS; give some examples
   strncpy (misc.tts[0], "espeak -f eBook-speaker.txt -w eBook-speaker.wav",
            MAX_STR - 1);
   strncpy (misc.tts[1], "flite eBook-speaker.txt eBook-speaker.wav", MAX_STR - 1);
   strncpy (misc.tts[2],
            "espeak -f eBook-speaker.txt -w eBook-speaker.wav -v mb-nl2",
            MAX_STR - 1);
   strncpy (misc.tts[3], "text2wave eBook-speaker.txt -o eBook-speaker.wav",
            MAX_STR - 1);
   strncpy (misc.tts[4],
       "swift -n Lawrence -f eBook-speaker.txt -m text -o eBook-speaker.wav",
            MAX_STR - 1);
} // read_xml