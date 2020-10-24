/* internal-Escape-CGIstring
   This function will escape characters in a string to their
   safe CGI representation.  Note that the target string must
   be capable of holding 3*strlen(s) characters (also the NULL
   terminator).  */

   sizea len = STRLENN(s);
   char *in = s + 2 * len;
   char *out = s;
   char ch;
   sizea num;

   if(s == NULL) return(NULL);

   /* Move the working copy of the string elsewhere, for now */
   MEMCPY(in, s, len + 1);

   /* Iterate */
   while(*in != '\0') {
      ch = *in;
      if((ch >= 'a' && ch <= 'z') ||
         (ch >= 'A' && ch <= 'Z') ||
         (ch >= '0' && ch <= '9') ||
         (ch == '.' || ch == '/' || ch == '_') || ch == '-') {
         /* Safe character */
         *out = ch;
         ++out;
      } else {
         /* Unsafe character */
         *out = '%';
         num = (unsigned)ch / 16;
         if(num < 10) *(out + 1) = num + '0';
         else         *(out + 1) = num + 'A' - 10;
         num = (unsigned)ch % 16;
         if(num < 10) *(out + 2) = num + '0';
         else         *(out + 2) = num + 'A' - 10;
         out += 3;
      }
      ++in;
   }
   *out = '\0';

   return(s);

}
