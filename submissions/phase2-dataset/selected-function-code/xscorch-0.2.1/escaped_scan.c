/* Escaped-Scan
   Scans for occurrence of ch in the string.  Returns NULL if the character
   is not found, otherwise the first occurence is returned.  This function
   will ignore characters inside a quoted block, as well as escaped
   characters.  */

   char *p;
   int instring;

   if(src == NULL) return(NULL);

   p = src;
   instring = 0;
   while(*p != '\0') {
      if(*p == '\"') {
         instring = !instring;
      } else if(*p == '\\' && *(p + 1) != '\0') {
         ++p;
      } else if(!instring && *p == ch) return(p);
      ++p;
   }

   return(NULL);

}
