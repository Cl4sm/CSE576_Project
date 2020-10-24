INT  open_archive(INT print_err)        // opens archive (or volume)
{
   CHAR av_str[80];
   unsigned int copylen;

   archan = open(aname, O_RDONLY | O_BINARY);   // open file

   if (archan == -1)
   {
      printf("\nError opening file %s", aname);
      return 0;
   }
   if (!read_arc_head())                        // read archive header
   {                            
      if (print_err)
         printf("\nInvalid archive file: %s\n", aname);
      close(archan);
      return 0;
   }

   printf("\nProcessing archive: %s\n\n", aname);
   if (head.HEAD_FLAGS & ACE_AV)
   {
      printf("Authenticity Verification:");   // print the AV
      sprintf(av_str, "\ncreated on %d.%d.%d by ",
              ts_day(adat.time_cr), ts_month(adat.time_cr), ts_year(adat.time_cr));
      printf("%s", av_str);
      copylen = mhead.AV_SIZE;
      if (copylen > 79)
        copylen = 79;
      strncpy(av_str, mhead.AV, copylen);
      av_str[copylen] = 0;
      printf("%s\n\n", av_str);
   }
   comment_out("Main comment:");        // print main comment
   return 1;
}