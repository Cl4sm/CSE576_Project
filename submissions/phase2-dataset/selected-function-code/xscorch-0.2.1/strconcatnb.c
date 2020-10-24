char *strconcatnb(char *d, const char *s, sizea n, sizea b) {
/* StringN-Concat-Null
   Concatenates 2 strings by copying the first <n> characters from
   s into d, and appends a null character to the string.  The dest
   buffer is <b> bytes long (counting the NULL that is stored in d),
   therefore s might be truncated (we might not be able to copy all
   n characters).  */

   sizea dlen;

   if(d != NULL) {
      dlen = STRLEN(d);
      if(b < dlen) {
         /* Cannot do a thing */
      } else if(b - dlen >= n + 1) {
         /* The buffer can hold all of the requested chars */
         strcopyn(d + dlen, s, n);
      } else if(b - dlen > 0) {
         /* The buffer can only hold some of requested chars */
         strcopyb(d + dlen, s, b - dlen);
      } /* If neither case holds, then we cannot grow */
   }
   return(d);

}
