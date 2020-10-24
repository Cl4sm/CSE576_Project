INT read_arc_head(void)         // searches for the archive header and reads it
{
   INT  i,
        flags,
        buf_pos = 0;
   LONG arc_head_pos,
        old_fpos,
        fpos = 0;
   struct stat st;

   fstat(archan, &st);

   memset(buf, 0, size_buf);

   while (lseek(archan, 0, SEEK_CUR)<st.st_size && fpos < max_sfx_size)
   {
      old_fpos = fpos;
      fpos += read(archan, &buf[buf_pos], size_buf - buf_pos);

      for (i = 0; i < size_buf; i++)    // look for the acesign
      {                         
         if (!memcmp(acesign, &buf[i], acesign_len))
         {             
                                        // seek to the probable begin 
                                        // of the archive
            arc_head_pos = old_fpos + i - buf_pos -  bytes_before_acesign;
            lseek(archan, arc_head_pos, SEEK_SET);
            if (read_header(0))         // try to read archive header
            {                           
               flags = mhead.HEAD_FLAGS;
               adat.sol     = (flags & ACE_SOLID) > 0;
               adat.vol     = (flags & ACE_MULT_VOL) > 0;
               adat.vol_num = mhead.VOL_NUM;
               adat.time_cr = mhead.TIME_CR;
               return 1;
            }
         }
      }
                                        // was no archive header,
                                        // continue search
      lseek(archan, fpos, SEEK_SET);
      memcpy(buf, &buf[size_buf - 512], 512);
      buf_pos = 512;                    // keep 512 old bytes
   }
   return 0;
}