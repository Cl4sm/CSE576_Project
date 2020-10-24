INT  read_adds_blk(CHAR * buffer, INT len)      // reads part of ADD_SIZE block
{
   INT  rd = 0,
        l = len;
   LONG i;

   while (!f_err && len && skipsize)
   {
      i = (skipsize > len) ? len : skipsize;
      skipsize -= i;

      errno = 0;
      rd += read(archan, buffer, i);
      if (errno)
      {
         printf("\nRead error\n");
         f_err = ERR_READ;
      }

      buffer += i;
      len -= i;

      if (!skipsize)            // if block is continued on next volume
         if (head.HEAD_FLAGS & ACE_SP_AFTER && !proc_next_vol())
            break;
   }

   return (rd > l ? l : rd);
}