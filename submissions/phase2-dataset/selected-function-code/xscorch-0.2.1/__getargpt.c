static inline sizea __getargpt(char **d, char *s, sizea argnum, char sep) {
/* internal-Get-Argument-Pointer-Trim
   returns a pointer to the argnum'th argument in '|'-delimited cmd
   string (or you can optionally specify your own separator to use).
   This command will stop processing at a null character. If argnum
   is not valid, a NULL pointer is returned and the return length
   is zero. Otherwise, the return length is the length of the
   argument. This command will trim the argument beforehand, which
   is reflected in the returned pointer and size. */

   whitespace ws;

   /* Check for validity of argnum (ie, give up if it is too small), d, s */
   if(d != NULL) {
      *d = NULL;
      if(s != NULL) {
         /* Search for the <argnum>'th argument. */
         while(argnum > 0 && *s != '\0') {
            if(*s == sep) --argnum;
            ++s;
         }

         /* If argnum > 0, then argnum supplied was too large. */
         if(argnum == 0) {
            /* Start tracking the argument until the end of argument; we'll
               also trim this argument for user convenience :) */
            SKIM_WHITESPACE_T(s, sep);
            SET_FIRST_NWS(ws, s);
            SET_LAST_NWS_T(ws, s, sep);

            /* Return pointer and size */
            *d = ws.fnws;
            return(NWS_SIZE(ws));
         } /* Did we have an arg to fetch? */
      }
   }

   return(0);

}
