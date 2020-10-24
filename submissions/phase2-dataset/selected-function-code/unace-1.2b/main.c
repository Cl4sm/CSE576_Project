int main(INT argc, CHAR * argv[])              // processes the archive
{
   INT show_help,
       arg_cnt = 1;

   printf("%s", version);
   show_help=0;

   if (argc < 3 || strlen(argv[1]) > 1 || argv[argc-1][0] == '-')
      show_help=1;

   while (!show_help && argv[++arg_cnt][0] == '-')
   {
      switch (tolower(argv[arg_cnt][1]))
      {
         case 'y':
            f_ovrall    = 1;      // Overwrite all
            f_allvol_pr = 2;      // Process all volumes, and never ask
            break;
         default:
            show_help = 1;
            break;
      }
   }

   if (show_help)
     showhelp();
   else
   {
      CHAR *s;

      init_unace();                              // initialize unace

      strncpy(aname, argv[arg_cnt], sizeof(aname) - 4);  // get archive name
      aname[sizeof(aname) - 5] = '\0';
      if (!(s = (CHAR *) strrchr(aname, DIRSEP)))
         s = aname;
      if (!strrchr(s, '.'))
         strcat(aname, ".ACE");

      if (open_archive(1))                       // open archive to process
      {
         if (adat.vol_num)
            printf("\nFirst volume of archive required!\n");
         else
            switch (tolower(*argv[1]))
            {
               case 'e': extract_files(1, 0); break;  // extract files without path
               case 'x': extract_files(0, 0); break;  // extract files with path
               case 'l': list_files   (0   ); break;  // list files
               case 'v': list_files   (1   ); break;  // list files verbose
               case 't': extract_files(0, 1); break;  // test archive integrity.
               default : showhelp();                  // Wrong command!
            }

         close(archan);
         if (f_err)
         {
            printf("\nError occurred\n");
            if (f_criterr)
               printf("Critical error on drive %c\n", f_criterr);
         }
      }
      else
         f_err = ERR_CLINE;

      done_unace();
   }

   putchar('\n');
   putc   ('\n', stderr);

   if (!f_err && f_err_crc)
   {
      printf("One or more CRC-errors were found.\n");
      f_err = ERR_CRC;
   }
   return f_err;
}