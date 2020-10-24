char *unitocharc(char *d, const wchar *s, unsigned int maxlen) {
/* Unicode-to-Character-Countbuffer
   Converts a source unicode string into a character string by stripping
   off the high bits of the source string. The target buffer should be
   at least wide_strlen(source) + 1 bytes long if <maxlen> = 0, otherwise
   <maxlen> will specify the size of the target buffer. */

   const unsigned char *ps = (const unsigned char *)s;
   unsigned char *pd = (unsigned char *)d;

   if(!s || !d) return(NULL);

   /* Convert a unicode string to char, stripping the most significant bytes */
   while(*ps && --maxlen) {
      *pd = *ps;
      ps += 2;
      pd++;
   }
   *pd = '\0';
   return(d);

}
