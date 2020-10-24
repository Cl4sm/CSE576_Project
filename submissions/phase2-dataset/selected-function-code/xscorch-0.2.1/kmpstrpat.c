/* Kmith-Morris-Pratt-String-Pattern
   A different algorithm from rabin karp. Best suited for algorithms with
   longer patterns with a relatively small alphabet. Not a guaranteed
   one-pass algorithm like rabin-karp. Similar interface as in rabin-karp
   rkstrpat() function. */

   sizea dlen = 0;
   sizea *pi = NULL;
   sizea *pip;
   sizea k = 0;
   char *sp = s;
   char *dp = (char *)d;

   if(d == NULL) return(s);

   if(s != NULL && d != NULL) {

      /* Get the pattern string length */
      while(*dp != '\0') ++dp;
      dlen = dp - d;

      /* Allocate the data buffer for pi - calculate pi values */
      if(dlen != 0) {
         pi = (sizea *)malloc(sizeof(sizea) * dlen);
         if(pi != NULL) {
            pip = pi + 1;
            *pi = 0;
            dp = (char *)(d + 1);
            while(*dp != '\0') {
               while(k && d[k] != *dp) k = pi[k];
               if(d[k] == *dp) ++k;
               *pip = k;
               ++dp;
               ++pip;
            }
         } else dlen = 0;
      }

      /* Now, search! Search, my friend, search! */
      k = 0;
      while(*sp != '\0') {
         while(k && d[k] != *sp) k = pi[k];
         if(d[k] == *sp) ++k;
         if(k == dlen) {
            /* Match found! Yay... */
            free(pi);
            return(sp - dlen + 1);
         }
         ++sp;
      }
      free(pi);

   } /* Source, dest weren't NULL */

   return(NULL);

}
