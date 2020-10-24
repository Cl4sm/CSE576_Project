/* Copy-String-Rightaligned-Length
   Copies the first <size> characters of string <s> to string <d>. String
   <d> must already be allocated to support <size> + 1 characters (this
   will allow for the terminating null character). Preceding characters
   are filled in as spaces. Note: if <s> is bigger than <size> then
   trailing characters will be cut off. This command is just an
   overglorified memset() if <s> is NULL. :) */

   sizea count = STRLENN(s);
   char *dstart = d;    /* Original dest pointer. */

   if(d != NULL) {
      if(count < size) {
         /* We will have some preceding spaces. */
         count = size - count;
         MEMSET(d, ' ', count);
         size -= count;
         d += count;
      }
      if(s != NULL) {
         /* Copy the most relevant characters in s. */
         MEMCPY(d, s, size);
         d += size;
      }
      *d = '\0';
      return(dstart);

   }
   return(NULL);

}
