void sortrange(INT left, INT right)
{
   INT  zl = left,
        zr = right,
        hyphen;

   hyphen = sort_freq[right];

   //divides by hyphen the given range into 2 parts
   do
   {
      while (sort_freq[zl] > hyphen)
         zl++;
      while (sort_freq[zr] < hyphen)
         zr--;
      //found a too small (left side) and
      //a too big (right side) element-->exchange them
      if (zl <= zr)
      {
         xchg_def(sort_freq[zl], sort_freq[zr]);
         xchg_def(sort_org[zl], sort_org[zr]);
         zl++;
         zr--;
      }
   }
   while (zl < zr);

   //sort partial ranges - when very small, sort directly
   if (left < zr)
   {
      if (left < zr - 1)
         sortrange(left, zr);
      else if (sort_freq[left] < sort_freq[zr])
      {
         xchg_def(sort_freq[left], sort_freq[zr]);
         xchg_def(sort_org[left], sort_org[zr]);
      }
   }

   if (right > zl)
   {
      if (zl < right - 1)
         sortrange(zl, right);
      else if (sort_freq[zl] < sort_freq[right])
      {
         xchg_def(sort_freq[zl], sort_freq[right]);
         xchg_def(sort_org[zl], sort_org[right]);
      }
   }
}