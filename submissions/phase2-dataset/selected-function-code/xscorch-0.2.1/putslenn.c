void putslenn(const char *s, sizea size) {
/* Put-String-Length
   Displays the first <size> characters of string <s> on the console.
   If <s> is smaller than <size> characters, then the remaining characters
   are filled in as spaces. Useful for table-like output. Note: if <s> is
   bigger than <size> then trailing characters will be cut off. */

   if(s != NULL) {
      while(size > 0 && *s != '\0') {
         /* Print out the first relevant characters of s. */
         putchar(*s);
         s++;
         size--;
      }
   }
   while(size > 0) {
      /* s is smaller than size -- fill in some spaces. */
      putchar(' ');
      size--;
   }

}
