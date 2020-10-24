void putsrlenn(const char *s, sizea size) {
/* Put-String-Rightaligned-Length
   Displays the first <size> characters of string <s> on the console.
   If <s> is smaller than <size> characters, then preceding characters
   are filled in as spaces. Useful for table-like output. Note: if <s> is
   bigger than <size> then trailing characters will be cut off. */

   sizea count = STRLENN(s);
   while(count < size) {
      /* We will have some preceding spaces. */
      putchar(' ');
      size--;
   }
   if(s != NULL) {
      while(size > 0 && *s != '\0') {
         /* Print out the first relevant characters of s. */
         putchar(*s);
         s++;
         size--;
      }
   }

}
