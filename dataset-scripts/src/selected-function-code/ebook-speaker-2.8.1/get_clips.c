void get_clips (char *orig_begin, char *end)
{
   char begin_str[MAX_STR], *begin;

   if (misc.audiocd == 1)
      return;
   strncpy (begin_str, orig_begin,  MAX_STR - 1);
   begin = begin_str;
   while (! isdigit (*begin))
      begin++;
   if (strchr (begin, 's'))
      *strchr (begin, 's') = 0;
   if (! strchr (begin, ':'))
      misc.clip_begin = (float) atof (begin);
   else
      misc.clip_begin = read_time (begin);

// fill end
   while (! isdigit (*end))
      end++;
   if (strchr (end, 's'))
      *strchr (end, 's') = 0;
   if (! strchr (end, ':'))
      misc.clip_end = (float) atof (end);
   else
      misc.clip_end = read_time (end);
} // get_clips