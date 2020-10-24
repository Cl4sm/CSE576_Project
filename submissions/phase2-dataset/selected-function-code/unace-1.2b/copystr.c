void copystr(LONG d, INT l)
{
   INT  mpos;

   dcpr_do += l;

   mpos = dcpr_dpos - d;
   mpos &= dcpr_dican;

   if ((mpos >= dcpr_dicsiz - maxlength) || (dcpr_dpos >= dcpr_dicsiz - maxlength))
   {
      while (l--)
      {
         dcpr_text[dcpr_dpos] = dcpr_text[mpos];
         dcpr_dpos++;
         dcpr_dpos &= dcpr_dican;
         mpos++;
         mpos &= dcpr_dican;
      }
   }
   else
   {
      while (l--)
         dcpr_text[dcpr_dpos++] = dcpr_text[mpos++];
      dcpr_dpos &= dcpr_dican;
   }
}