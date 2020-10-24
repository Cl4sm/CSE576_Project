void list_files(int verbose)
{
   unsigned files=0;
   ULONG    size =0,
            psize=0,
            tpsize;
   CHAR     file[PATH_MAX];

   printf("Date    |Time |Packed     |Size     |Ratio|File\n");

   while (!cancel() && read_header(1))
   {
      if (head.HEAD_TYPE == FILE_BLK)
      {
         ULONG ti=fhead.FTIME;
         ace_fname(file, &head, verbose ? 0 : 1, sizeof(file)); // get file name

         size  += fhead.SIZE;
         psize +=
         tpsize = fhead.PSIZE;
         files++;

         while (head.HEAD_FLAGS & ACE_SP_AFTER)
         {
            skipsize=0;
            if (!proc_next_vol())
               break;
            psize += fhead.PSIZE;
            tpsize+= fhead.PSIZE;
         }
         if (!f_err)
            printf("%02u.%02u.%02u|%02u:%02u|%c%c%9lu|%9lu|%4u%%|%c%s\n",
                   ts_day (ti), ts_month(ti), ts_year(ti)%100,
                   ts_hour(ti), ts_min  (ti),
                   fhead.HEAD_FLAGS & ACE_SP_BEF   ? '<' : ' ',
                   fhead.HEAD_FLAGS & ACE_SP_AFTER ? '>' : ' ',
                   tpsize, fhead.SIZE, percentage(tpsize, fhead.SIZE),
                   fhead.HEAD_FLAGS & ACE_PASSW    ? '*'    : ' ',
                   file
                  );
      }
   }
   if (!f_err)
   {
      printf("\n                 %9lu|%9lu|%4u%%| %u file%s",
             psize,
             size,
             percentage(psize, size),
             files,
             (char*)(files == 1 ? "" : "s")
            );
   }
}