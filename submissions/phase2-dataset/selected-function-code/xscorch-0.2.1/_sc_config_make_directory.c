/* sc_config_make_directory
   Construct a new local directory for config files, etc. */

   char dirname[SC_FILENAME_LENGTH];   /* Name of the directory */
   struct stat s;                      /* For the stat() calls */

   /* Figure out the appropriate directory name */
   sbprintf(dirname, sizeof(dirname), "%s/%s", getenv("HOME"), SC_LOCAL_CONFIG_DIR);

   /* Check if the directory exists; try to make it otherwise. */
   if(stat(dirname, &s) < 0) {
      if(mkdir(dirname, 0755) < 0) {
         fprintf(stderr, "warning:  cannot create configuration directory \"%s\"\n", dirname);
         return(false);
      } /* Made a new directory? */
   } /* Did the directory exist? */
   
   /* Return success */   
   return(true);

}
