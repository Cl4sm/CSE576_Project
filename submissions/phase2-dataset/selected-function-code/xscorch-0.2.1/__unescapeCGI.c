/* internal-Unescape-CGIstring
   This function will unescape characters in a CGI-encoded string.
   Note that since the target string is always smaller than the
   source string we don't have to worry about buffer overflows
   (unless you were stupid enough not to terminate <s> with a
   null character! :) */

   char *d = s;
   char *dstart = d;
   unsigned char num;

   if(s != NULL) {

      /* This part of the function converts CGI "+" to spaces, expands CGI
         escapes, etc etc etc... */
      while(*s != '\0') {
         if(*s == '+') {
            /* Expand the space */
            *d = ' ';
         } else if(*s == '%') {
            /* Expand that escape! yum yum */
            ++s;
            num = 0;
            if (*s >= 'a' && *s <= 'f')      num = (unsigned)*s - 'a' + 10;
            else if (*s >= 'A' && *s <= 'F') num = (unsigned)*s - 'A' + 10;
            else if (*s >= '0' && *s <= '9') num = (unsigned)*s - '0';
            ++s;
            num *= 16;
            if (*s >= 'a' && *s <= 'f')      num += (unsigned)*s - 'a' + 10;
            else if (*s >= 'A' && *s <= 'F') num += (unsigned)*s - 'A' + 10;
            else if (*s >= '0' && *s <= '9') num += (unsigned)*s - '0';
            *d = num;
         } else *d = *s;
         ++s;
         ++d;
      }
      *d = '\0';
      return(dstart);
   }

   return(NULL);

}
