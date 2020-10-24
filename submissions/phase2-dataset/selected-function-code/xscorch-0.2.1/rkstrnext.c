/* Rabin-Karp-String-Next
   Searches for the next substring <d> in source string <s> using the
   Rabin Karp algorithm. Returns NULL if no more entries were found, or
   if there was no successful call to rkstrpat() to initiate the search. */

   unsigned char *rk_source;
   sizea  rk_i;
   sdword rk_t;
   sdword rk_p;
   sdword rk_h;
   sizea  rk_dlen;
   bool   need_to_skip;

   if(rk != NULL && rk->rk_source != NULL) {

      /* We need to update the calculations to reflect the first shift.
         As a result, we set rk_source equal to the location of the
         last match; we'll explicitly ignore it further down... */
      rk_source  = (unsigned char *)rk->rk_source;
      need_to_skip = true;
      if(*rk_source != '\0') {
         if(rk->rk_pattern != NULL && *rk->rk_pattern != '\0') {

            /* Pattern matching loop */
            rk_i = rk->rk_i;
            rk_t = rk->rk_t;
            rk_p = rk->rk_p;
            rk_h = rk->rk_h;
            rk_dlen = rk->rk_dlen;
            while(rk_i != 0) {
               if(rk_p == rk_t) {
                  if(!need_to_skip && MEMEQL(rk_source, rk->rk_pattern, rk_dlen)) {
                     rk->rk_i = rk_i;
                     rk->rk_t = rk_t;
                     rk->rk_source = (char *)rk_source;
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
               need_to_skip = false;
            }

         } else return(rk->rk_source);

      } /* Source is not an empty buffer? */

      /* Assign source to NULL (we've reached end of search) */
      rk->rk_source = NULL;

   } /* rk was not null */

   return(NULL);

}
