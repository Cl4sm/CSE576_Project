INT unstore(CHAR * buf, UINT len)
{
   UINT rd = 0,
        i,
        pos = 0;

   while ((i = read_adds_blk((CHAR *) buf_rd, (INT) ((i = ((len > dcpr_size) ? dcpr_size : len)) > size_rdb ? size_rdb : i))) != 0)
   {
      rd += i;
      len -= i;
      dcpr_size -= i;
      memcpy(&buf[pos], buf_rd, i);
      pos += i;
   }
   for (i = 0; i < rd; i++)
   {
      dcpr_text[dcpr_dpos] = buf[i];
      dcpr_dpos++;
      dcpr_dpos &= dcpr_dican;
   }
   return (INT)rd;
}