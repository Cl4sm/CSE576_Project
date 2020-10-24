INT  makecode(UINT maxwd, UINT size1_t, UCHAR * wd, USHORT * code)
{
   UINT maxc,
        size2_t,
        l,
        c,
        i,
        max_make_code;

   memcpy(&sort_freq, wd, (size1_t + 1) * sizeof(CHAR));
   if (size1_t)
      quicksort(size1_t);
   else
      sort_org[0] = 0;
   sort_freq[size1_t + 1] = size2_t = c = 0;
   while (sort_freq[size2_t])
      size2_t++;
   if (size2_t < 2)
   {
      i = sort_org[0];
      wd[i] = 1;
      size2_t += (size2_t == 0);
   }
   size2_t--;

   max_make_code = 1 << maxwd;
   for (i = size2_t + 1; i-- && c < max_make_code;)
   {
      maxc = 1 << (maxwd - sort_freq[i]);
      l = sort_org[i];
      if (c + maxc > max_make_code)
      {
         dcpr_do = dcpr_do_max;
         return (0);
      }
      memset16(&code[c], l, maxc);
      c += maxc;
   }
   return (1);
}