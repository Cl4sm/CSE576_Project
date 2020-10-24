/* Escape
   Escapes potentially troublesome characters in the string.  The
   size indicates the maximum size of the buffer; trailing characters
   will be truncated if necessary.  To guarantee the entire string is
   left intact, allocate a buffer twice as large as the string.  */

   char *buf;
   char *src;
   char *dest;

   if(str == NULL || size <= 0) return(NULL);

   buf = (char *)malloc(size + size + 2);
   if(buf == NULL) return(NULL);

   src = str;
   dest = buf;
   while(*src != '\0') {
      if(*src == '\n') {
         *dest++ = '\\';
         *dest++ = 'n';
      } else if(*src == '\r') {
         *dest++ = '\\';
         *dest++ = 'r';
      } else if(*src == '\t') {
         *dest++ = '\\';
         *dest++ = 't';
      } else if(*src == '\\' || *src == '\"') {
         *dest++ = '\\';
         *dest++ = *src;
      } else {
         *dest++ = *src;
      }
      ++src;
   }

   *dest = '\0';
   MEMCPY(str, buf, size);
   str[size - 1] = '\0';
   free(buf);
   return(str);

}
