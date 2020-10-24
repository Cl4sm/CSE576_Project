void decompress(void)
{
   INT  c,
        lg,
        i,
        k;
   ULONG dist;

   while (dcpr_do < dcpr_do_max)
   {
      if (!blocksize)
         if (!calc_dectabs())
            return;

      addbits(dcpr_wd_mn[(c = dcpr_code_mn[code_rd >> (32 - maxwd_mn)])]);
      blocksize--;
      if (c > 255)
      {
         if (c > 259)
         {
            if ((c -= 260) > 1)
            {
               dist = (code_rd >> (33 - c)) + (1L << (c - 1));
               addbits(c - 1);
            }
            else
               dist = c;
            dcpr_olddist[(dcpr_oldnum = (dcpr_oldnum + 1) & 3)] = dist;
            i = 2;
            if (dist > maxdis2)
            {
               i++;
               if (dist > maxdis3)
                  i++;
            }
         }
         else
         {
            dist = dcpr_olddist[(dcpr_oldnum - (c &= 255)) & 3];
            for (k = c + 1; k--;)
               dcpr_olddist[(dcpr_oldnum - k) & 3] = dcpr_olddist[(dcpr_oldnum - k + 1) & 3];
            dcpr_olddist[dcpr_oldnum] = dist;
            i = 2;
            if (c > 1)
               i++;
         }
         addbits(dcpr_wd_lg[(lg = dcpr_code_lg[code_rd >> (32 - maxwd_lg)])]);
         dist++;
         lg += i;
         copystr(dist, lg);
      }
      else
         wrchar(c);
   }
}