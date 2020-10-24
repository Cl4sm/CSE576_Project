INT  read_header(INT print_err)         // reads any header from archive
{
   USHORT rd,
        head_size,
        need_size,
        crc_ok;
   LONG crc;
   UCHAR *tp=readbuf;

   lseek(archan, skipsize, SEEK_CUR);   // skip ADDSIZE block

   if (read(archan, &head, 4)<4)
      return 0;                         // read CRC and header size

#ifdef HI_LO_BYTE_ORDER
   WORDswap(&head.HEAD_CRC);
   WORDswap(&head.HEAD_SIZE);
#endif
                                        // read size_headrdb bytes into 
   head_size = head.HEAD_SIZE;          // header structure 
   need_size = 3;
   if (need_size > head.HEAD_SIZE)
      return 0;
   rd = (head_size > size_headrdb) ? size_headrdb : head_size;
   if (read(archan, readbuf, rd) < rd)
      return 0;
   head_size -= rd;
   crc = getcrc(CRC_MASK, readbuf, rd);

   while (head_size)                    // skip rest of header
   {                            
      rd = (head_size > size_buf) ? size_buf : head_size;
      if (read(archan, buf, rd) < rd)
         return 0;
      head_size -= rd;
      crc = getcrc(crc, buf, rd);
   }

   head.HEAD_TYPE =*tp++;               // generic buffer to head conversion
   head.HEAD_FLAGS=BUFP2WORD(tp);

   if (head.HEAD_FLAGS & ACE_ADDSIZE)
   {
      need_size += 4;
      if (need_size > head.HEAD_SIZE)
         return 0;
      skipsize = head.ADDSIZE = BUF2LONG(tp);   // get ADDSIZE
   }
   else
      skipsize = 0;

                                                // check header CRC 
   if (!(crc_ok = head.HEAD_CRC == (crc & 0xffff)) && print_err)
      printf("\nError: archive is broken\n");
   else
   switch (head.HEAD_TYPE)              // specific buffer to head conversion
   {
      case MAIN_BLK:
         need_size += 24;
         if (need_size > head.HEAD_SIZE)
            return 0;
         memcpy(mhead.ACESIGN, tp, acesign_len); tp+=acesign_len;
         mhead.VER_MOD=*tp++;
         mhead.VER_CR =*tp++;
         mhead.HOST_CR=*tp++;
         mhead.VOL_NUM=*tp++;
         mhead.TIME_CR=BUFP2LONG(tp);
         mhead.RES1   =BUFP2WORD(tp);
         mhead.RES2   =BUFP2WORD(tp);
         mhead.RES    =BUFP2LONG(tp);
         mhead.AV_SIZE=*tp++;
         if (mhead.AV_SIZE > sizeof(mhead.AV) ||
             mhead.AV_SIZE + need_size > head.HEAD_SIZE)
            return 0;
         memcpy(mhead.AV, tp, mhead.AV_SIZE);
         break;
      case FILE_BLK:
         need_size += 28;
         if (need_size > head.HEAD_SIZE)
            return 0;
         fhead.PSIZE     =BUFP2LONG(tp);
         fhead.SIZE      =BUFP2LONG(tp);
         fhead.FTIME     =BUFP2LONG(tp);
         fhead.ATTR      =BUFP2LONG(tp);
         fhead.CRC32     =BUFP2LONG(tp);
         fhead.TECH.TYPE =*tp++;
         fhead.TECH.QUAL =*tp++;
         fhead.TECH.PARM =BUFP2WORD(tp);
         fhead.RESERVED  =BUFP2WORD(tp);
         fhead.FNAME_SIZE=BUFP2WORD(tp);
         if (fhead.FNAME_SIZE > sizeof(fhead.FNAME) ||
             fhead.FNAME_SIZE + need_size > head.HEAD_SIZE)
            return 0;
         memcpy(fhead.FNAME, tp, fhead.FNAME_SIZE);
         break;
//    default: (REC_BLK and future things): 
//              do nothing 'cause isn't needed for extraction
   }

   return crc_ok;
}