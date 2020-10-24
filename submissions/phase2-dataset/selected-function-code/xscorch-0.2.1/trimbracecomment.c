/* Trim-Brace-Comment
   Trims off the comment in string <s>, which is specified as anything
   following the character specified in <c1>, up to the occurrence of
   <c2>. */

   char *p = s;
   char *p1;

   if(s != NULL) {
      while(*p != '\0') {
         if(*p == c1) {
            p1 = p;
            while(*p != '\0' && *p != c2) p++;
            if(*p != '\0') {
               p++;
               STRCPY(p1, p);
               p = p1;
            } else *p1 = '\0';
         } else p++;
      }
   }
   return(s);

}
