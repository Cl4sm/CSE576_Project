static void _trim_line(char *buf) {

   bool countedspace = true;
   char *d = buf;
   char *s = buf;
   while(*s != '\0') {
      if(*s == ' ' || *s == '\t' || *s == '\n') {
         if(!countedspace) {
            countedspace = true;
            *d = ' ';
            ++d;
         }
      } else {
         countedspace = false;
         *d = *s;
         ++d;
      }
      ++s;
   }
   if(countedspace && d > buf) --d;
   *d = '\0';

}
