/* Unescape
   This function will take an escaped string and "unescape" it. Standard C
   escape sequence is read. The escape sequences currently recognized are
      \a    bell
      \b    backspace
      \f    formfeed
      \n    newline
      \r    carriage return
      \t    tab
      \v    vertical tab
      \'    single quote
      \"    double quote
      \?    question mark
      \\    backslash
      \0    null
      \ooo  ascii character
      \xhh  ascii character
   If the escape sequence is not valid then the next character after the
   escape will be printed (but not the escape itself). */

   char *d = s;      /* Dest pointer */
   char *dstart = s; /* Original pointer */
   int num;          /* Dummy numerical value */

   if(s != NULL) {
      while(*s != '\0') {
         if(*s == '\\') {
            ++s;

            /* Escape character found */
            switch(*s) {

               /* If null, then copy a null and back up s pointer */
               case '\0':
                  *d = '\0';
                  s--;
                  break;

               /* If return, then remove the return */
               case '\n':
                  d--;
                  break;

               /* Numbers  */
               case '0':
               case '1':
               case '2':
               case '3':
               case '4':
               case '5':
               case '6':
               case '7':
                  num = *s - '0';
                  ++s;
                  if(*s >= '0' && *s <= '7') {
                     num = (num << 3) | (*s - '0');
                     ++s;
                     if(*s >= '0' && *s <= '3') num = (num << 3) | (*s - '0');
                     else s--;
                  } else s--;
                  *d = (unsigned char)num;
                  break;

               /* General escapes */
               case 'a':
                  *d = '\a';
                  break;
               case 'b':
                  *d = '\b';
                  break;
               case 'f':
                  *d = '\f';
                  break;
               case 'n':
                  *d = '\n';
                  break;
               case 'r':
                  *d = '\r';
                  break;
               case 't':
                  *d = '\t';
                  break;
               case 'v':
                  *d = '\v';
                  break;

               /* Hexadecimal numbers case */
               case 'x':
                  ++s;
                  if(*s >= '0' && *s <= '9') {
                     num = *s - '0';
                     ++s;
                     if(*s >= '0' && *s <= '9') num = (num << 4) | (*s - '0');
                     if(*s >= 'a' && *s <= 'f') num = (num << 4) | (*s - 'a' + 10);
                     if(*s >= 'A' && *s <= 'F') num = (num << 4) | (*s - 'A' + 10);
                     else s--;
                     *d = (unsigned char)num;
                  } else if(*s >= 'a' && *s <= 'f') {
                     num = *s - 'a' + 10;
                     ++s;
                     if(*s >= '0' && *s <= '9') num = (num << 4) | (*s - '0');
                     if(*s >= 'a' && *s <= 'f') num = (num << 4) | (*s - 'a' + 10);
                     if(*s >= 'A' && *s <= 'F') num = (num << 4) | (*s - 'A' + 10);
                     else s--;
                     *d = (unsigned char)num;
                  } else if(*s >= 'A' && *s <= 'F') {
                     num = *s - 'A' + 10;
                     ++s;
                     if(*s >= '0' && *s <= '9') num = (num << 4) | (*s - '0');
                     if(*s >= 'a' && *s <= 'f') num = (num << 4) | (*s - 'a' + 10);
                     if(*s >= 'A' && *s <= 'F') num = (num << 4) | (*s - 'A' + 10);
                     else s--;
                     *d = (unsigned char)num;
                  } else {
                     d--;
                     s--;
                  }
                  break;

               /* This will catch everything else */
               default:
                  *d = *s;
                  break;
            }
            ++s;
         } else {
            /* Normal character */
            *d = *s;
            ++s;
         }
         ++d;
      }
      *d = '\0';
      return(dstart);
   } /* source wasn't NULL */
   return(NULL);

}
