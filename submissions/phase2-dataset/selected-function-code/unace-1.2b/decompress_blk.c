INT  decompress_blk(CHAR * buf, UINT len)
{
   LONG old_pos = dcpr_dpos;
   INT  i;

   dcpr_do = 0;
   if ((dcpr_do_max = len - maxlength) > dcpr_size)
      dcpr_do_max = dcpr_size;
   if ((LONG) dcpr_size > 0 && dcpr_do_max)
   {
      decompress();
      if (dcpr_do <= len)
      {
         if (old_pos + dcpr_do > dcpr_dicsiz)
         {
            i = dcpr_dicsiz - old_pos;
            memcpy(buf, &dcpr_text[old_pos], i);
            memcpy(&buf[i], dcpr_text, dcpr_do - i);
         }
         else
            memcpy(buf, &dcpr_text[old_pos], dcpr_do);
      }
   }
   dcpr_size -= dcpr_do;
   return (dcpr_do);
}