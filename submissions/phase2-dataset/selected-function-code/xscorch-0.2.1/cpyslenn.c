char *cpyslenn(char *d, const char *s, sizea size) {
/* Copy-String-Length
   Copies the first <size> characters of string <s> to string <d>. String
   <d> must already be allocated to support <size> + 1 characters (this
   will allow for the terminating null character). Remaining characters
   are filled in as spaces. <s> and <d> may point to the same string but
   <d> cannot point inside of <s>. Note: if <s> is bigger than <size> then
   trailing characters will be cut off. This command is just an
   overglorified memset() call if <s> = NULL :) */

   sizea count = STRLENN(s);
   char *dstart = d;       /* Original return pointer */

   if(d != NULL) {

      if(s != NULL) {
         /* Copy the most relevant characters in s. */
         if(count > size) count = size;
         MEMCPY(d, s, count);
         size -= count;
         d += count;
      }

      /* s is smaller than size -- fill in with some spaces. */
      MEMSET(d, ' ', size);
      d += size;
      *d = '\0';
      return(dstart);

   }
   return(NULL);

}
