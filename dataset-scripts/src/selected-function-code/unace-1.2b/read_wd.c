INT  read_wd(UINT maxwd, USHORT * code, UCHAR * wd, INT max_el)
{
   UINT c,
        i,
        j,
        num_el,
        l,
        uplim,
        lolim;

   memset(wd, 0, max_el * sizeof(CHAR));
   memset(code, 0, (1 << maxwd) * sizeof(SHORT));

   num_el = code_rd >> (32 - 9);
   addbits(9);
   if (num_el > max_el)
      num_el = max_el;

   lolim = code_rd >> (32 - 4);
   addbits(4);
   uplim = code_rd >> (32 - 4);
   addbits(4);

   for (i = -1; ++i <= uplim;)
   {
      wd_svwd[i] = code_rd >> (32 - 3);
      addbits(3);
   }
   if (!makecode(maxwd_svwd, uplim, wd_svwd, code))
      return (0);
   j = 0;
   while (j <= num_el)
   {
      c = code[code_rd >> (32 - maxwd_svwd)];
      addbits(wd_svwd[c]);
      if (c < uplim)
         wd[j++] = c;
      else
      {
         l = (code_rd >> 28) + 4;
         addbits(4);
         while (l-- && j <= num_el)
            wd[j++] = 0;
      }
   }
   if (uplim)
      for (i = 0; ++i <= num_el;)
         wd[i] = (wd[i] + wd[i - 1]) % uplim;
   for (i = -1; ++i <= num_el;)
      if (wd[i])
         wd[i] += lolim;

   return (makecode(maxwd, num_el, wd, code));

}