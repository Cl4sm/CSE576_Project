
   char *d = buf;
   char *s = buf;
   while(*s != '\0') {
      if(strequaln(s, "\\-", 2)) {
         *d = '-';
         ++d;
         ++s;
      } else if(strequaln(s, "\\f", 2)) {
         s += 2;
      } else {
         *d = *s;
         ++d;
      }
      ++s;
   }
   *d = '\0';

}
