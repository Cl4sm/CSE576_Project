static inline char *__getargb(char *d, const char *s, sizea argnum, char sep, sizea maxlen) {
/* internal-Get-Argument-Countbuffer
   returns the argnum'th argument in '|'-delimited cmd string (or
   you can optionally specify your own separator to use). This command
   will stop processing at a null character. If argnum is not valid,
   an empty string is returned. The target buffer should be at least
   <maxlen> characters (counting NULL); any trailing characters will be
   truncated, and a NULL will always be squeezed in as needed. If
   <maxlen> is <= zero, then the target buffer should be at least as
   large as <s> buffer.
   If <s> = <NULL>, then this function will laugh at you. If <d> is
   NULL, you're screwed. */

   char *p;
   sizea size;

   if(d != NULL) {
      *d = '\0';

      /* Search for the <argnum>'th argument. */
      char *s_ptr = (char *)s;
      size = __getargpt(&p, s_ptr, argnum, sep);
      if(p != NULL && size > 0) {
         /* Copy the argument and exit */
         ++size;
         if(maxlen > 0 && size > maxlen) size = maxlen;
         STRNCPY(d, p, size);
      }

      /* Always return the destination */
      return(d);

   /* Destination was NULL */
   } else return(NULL);

}
