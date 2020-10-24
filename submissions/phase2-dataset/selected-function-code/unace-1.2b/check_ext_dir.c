void check_ext_dir(CHAR * f)        // checks/creates path of file
{
   CHAR *cp,
        d[PATH_MAX];
   unsigned int i;

   d[0] = 0;

   if (is_directory_traversal(f))
   {
      f_err = ERR_WRITE;
      printf("\n    Directory traversal attempt:  %s\n", f);
      return;
   }

   for (;;)
   {
      if ((cp = (CHAR *) strchr(&f[strlen(d) + 1], DIRSEP))!=NULL)
      {
         i = cp - f;
         if (i > (PATH_MAX - 1))
           i = PATH_MAX - 1;
         strncpy(d, f, i);
         d[i] = 0;
      }
      else
         return;

      if (!fileexists(d))
         if (mkdir(d))
         {
            f_err = ERR_WRITE;
            printf("\n    Error while creating directory.\n");
         }
   }
}