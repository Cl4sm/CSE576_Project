void extract_files(int nopath, int test)
{
   CHAR file[PATH_MAX];

   while (!cancel() && read_header(1))
   {
      if (head.HEAD_TYPE == FILE_BLK)
      {
         comment_out("File comment:");   // show file comment
         ace_fname(file, &head, nopath, sizeof(file)); // get file name
         printf("\n%s", file);
         flush;
         dcpr_init_file();               // initialize decompression of file
         if (!f_err)
         {
            if (test || 
                (wrhan = create_dest_file(file, (INT) fhead.ATTR))<0)
            {
               if (test || adat.sol)
                  analyze_file();        // analyze file
            }
            else
            {
               extract_file();           // extract it
#ifdef DOS                               // set file time
               _dos_setftime(wrhan, (USHORT) (fhead.FTIME >> 16), (USHORT) fhead.FTIME);
#endif
               close(wrhan);
#ifdef DOS                               // set file attributes
               _dos_setfileattr(file, (UINT) fhead.ATTR);
#endif
#ifdef AMIGA
               {                         // set file date and time
                  struct DateTime dt;
                  char Date[9], Time[9];
                  ULONG tstamp=fhead.FTIME;

                  sprintf(Date, "%02d-%02d-%02d", ts_year(tstamp)-1900, ts_month(tstamp), ts_day(tstamp));
                  sprintf(Time, "%02d:%02d:%02d", ts_hour(tstamp), ts_min(tstamp), ts_sec(tstamp));

                  dt.dat_Format = FORMAT_INT;
                  dt.dat_Flags  = 0;
                  dt.dat_StrDate= Date;
                  dt.dat_StrTime= Time;

                  if (StrToDate(&dt))
                     SetFileDate(file, &dt.dat_Stamp);
               }
#endif
               if (f_err)
                  remove(file);
            }
         }
      }
   }
}