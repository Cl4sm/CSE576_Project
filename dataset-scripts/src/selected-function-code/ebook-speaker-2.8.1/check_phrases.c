void check_phrases ()
{
   int item;

   misc.total_phrases = 0;
   for (item = 0; item < misc.total_items; item++)
   {
      strncpy (daisy[item].smil_file,
               re_organize (daisy[item].smil_file), MAX_STR - 1);
   } // for
   for (item = 0; item < misc.total_items; item++)
   {
      daisy[item].n_phrases =
         count_phrases (daisy[item].smil_file, daisy[item].anchor,
                        daisy[item + 1].smil_file,
                        daisy[item + 1].anchor) - 1;
      misc.total_phrases += daisy[item].n_phrases;
   } // for

// remove items with 0 phrases
   if (daisy[0].n_phrases == 0)
   {
      int i, x;

      for (i = 1; i < misc.total_items; i++)
         if (daisy[i].n_phrases > 0)
            break;
      item = 0;
      for (x = i; x < misc.total_items; x++)
      {
         strncpy (daisy[item].smil_file, daisy[x].smil_file, MAX_STR - 1);
         daisy[item].n_phrases = daisy[x].n_phrases;
         item++;
      } // for
      misc.total_items = misc.total_items - i;
   } // if
} // check_phrases