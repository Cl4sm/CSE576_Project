/* File-Count
   Counts the number of lines in a standard text file. Reads through a
   local read buffer for parsing. Return value is zero if an error occurs,
   or if the file isn't found, etc.. */

   char *buf;
   sizea count = 0;
   FILE *f;

   /* Give up now if the name wasn't given. */
   if(fname != NULL && *fname != '\0') {

      buf = (char *)malloc(LIBJ_READBUF);
      if(buf != NULL) {

         /* Try to open the file. On error return zero. */
         if(NULL != (f = fopen(fname, "r"))) {

            /* Read data until an EOF... */
            while(fgets(buf, LIBJ_READBUF, f) != NULL) {
               if(*(buf + STRLEN(buf) - 1) == '\n') ++count;
            }
            fclose(f);
         }
         free(buf);
      }
   }

   /* Return the final count */
   return(count);

}
