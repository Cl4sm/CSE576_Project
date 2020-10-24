/* Character-to-Unicode-Countbuffer
   Converts a source character string into a unicode string by appending
   a leading 0 byte to each byte in the source string. The target buffer
   should be at least strlen(source) * 2 + 2 bytes long if <maxlen> = 0,
   otherwise <maxlen> will specify the size of the target buffer IN
   2-BYTE INCREMENTS! */

   const unsigned char *ps = (const unsigned char *)s;
   unsigned char *pd = (unsigned char *)d;

   if(!s || !d) return(NULL);

   /* Convert a char string to ASCII characters in unicode */
   while(*ps && --maxlen) {
      *pd = *ps;
      *(pd + 1) = '\0';
      ps++;
      pd += 2;
   }
   *pd = '\0';
   *(pd + 1) = '\0';
   return(d);

}
