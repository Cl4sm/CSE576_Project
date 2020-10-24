/* Get-Argument-Pointer
   returns a pointer to the argnum'th argument in '|'-delimited cmd
   string (or you can optionally specify your own separator to use).
   This command will stop processing at a null character. If argnum
   is not valid, a NUILL pointer is returned and the return length
   is zero. Otherwise, the return length is the length of the
   argument. This command makes no effort to trim the argument
   beforehand. */

   if(d != NULL) {
      *d = NULL;

      /* Is <argnum> inside the valid range for this string?  */
      if(s != NULL) {
         /* Search for the <argnum>'th argument.  */
         while(argnum > 0 && *s) {
            if(*s == sep) --argnum;
            ++s;
         }
         if(argnum == 0) {
            /* Start tracking the argument until the end of argument */
            *d = s;
            while(*s != '\0' && *s != sep) ++s;
            return(s - *d);
         }
      }
   }

   return(0);

}
