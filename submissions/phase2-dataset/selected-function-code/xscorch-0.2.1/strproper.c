char *strproper(char *s) {
/* String-Proper
   converts string <s> to propercase character */

   char *d = s;
   bool whitespace = true;

   if(d != NULL) {
      while(*d != '\0') {
         if(WHITESPACE(*d)) whitespace = true;
         else if(whitespace) {
            if(LOWER(*d)) *d -= 0x20;
            whitespace = false;
         } else if(UPPER(*d)) *d += 0x20;
         d++;
      }
      return(s);
   } /* Destination wasn't null */
   return(NULL);

}
