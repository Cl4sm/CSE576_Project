char *rkstrpat(rkstate *rk, char *s, const char *d) {
/* Rabin-Karp-String-Pattern
   Searches for substring <d> in source string <s> using the Rabin Karp
   algorithm. */

   unsigned char *sp = (unsigned char *)s;
   const unsigned char *dp = (const unsigned char *)d;
   unsigned char *rk_source;
   const unsigned char *rk_pattern;
   sizea  rk_i;
   sdword rk_t;
   sdword rk_p;
   sdword rk_h;
   sizea  rk_slen;
   sizea  rk_dlen;

   if(s != NULL) {
      rk_p = 0;
      rk_t = 0;
      rk_h = 1;
      rk_i = 0;
      rk_source = (unsigned char *)s;
      rk_pattern = (const unsigned char *)d;
      if(d != NULL && *d != '\0') {

         /* Scan the strings */
         while(*sp != '\0' && *dp != '\0') {
            rk_t = (rk_t << 8) + *sp;
            if(rk_t >= LIBJ_RK_PRIME) rk_t %= LIBJ_RK_PRIME;
            rk_p = (rk_p << 8) + *dp;
            if(rk_p >= LIBJ_RK_PRIME) rk_p %= LIBJ_RK_PRIME;
            ++sp;
            ++dp;
         }
         if(*sp != '\0' || *dp == '\0') {

            /* Calculate the lengths of the strings */
            while(*sp != '\0') ++sp;
            rk_slen = (sp - (unsigned char *)s);
            rk_dlen = (dp - (const unsigned char *)d);

            /* Precalc the infamous "h" value.  */
            if(rk_dlen != 0) {
               rk_i = rk_dlen - 1;
               while(rk_i != 0) {
                  rk_h = (rk_h << 8);
                  if(rk_h >= LIBJ_RK_PRIME) rk_h %= LIBJ_RK_PRIME;
                  --rk_i;
               }
            }

            /* Pattern matching loop */
            rk_i = (rk_slen - rk_dlen) + 1;
            rk_source = (unsigned char *)s;
            while(rk_i) {
               if(rk_p == rk_t) {
                  if(MEMEQL(rk_source, rk_pattern, rk_dlen)) {
                     if(rk != NULL) {
                        rk->rk_i = rk_i;
                        rk->rk_t = rk_t;
                        rk->rk_p = rk_p;
                        rk->rk_h = rk_h;
                        rk->rk_source  = (char *)rk_source;
                        rk->rk_pattern = (const char *)rk_pattern;
                        rk->rk_dlen = rk_dlen;
                     }
                     return((char *)rk_source);
                  }
               }
               --rk_i;
               if(rk_i != 0) {
                  rk_t = (((rk_t - (((unsigned char)*rk_source) * rk_h)) << 8)
                       + ((unsigned char)*(rk_source + rk_dlen))) % LIBJ_RK_PRIME;
                  if(rk_t < 0) rk_t += LIBJ_RK_PRIME;
               }
               ++rk_source;
            }
         } /* Did we reach end of source before end of pattern? */
      } else {
         /* destination buffer started out empty */
         if(rk != NULL) {
            rk->rk_source  = (char *)rk_source;
            rk->rk_pattern = (const char *)rk_pattern;
         }
         return((char *)rk_source);
      }

   }

   if(rk != NULL) rk->rk_source = NULL;
   return(NULL);

}
