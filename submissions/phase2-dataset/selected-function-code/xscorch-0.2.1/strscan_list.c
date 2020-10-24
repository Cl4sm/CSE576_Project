char *strscan_list(char *s, const char *const*patlist, sizea *index) {
/* strscan_list
   Searches the string s for the first occurence of any string in the
   pattern list given (where pattern-list is terminated by a NULL entry).
   On success, *index is set to the matching pattern and the location in
   the string is returned.  This is currently using the naive algorithm
   which applies rabin-karp on every pattern and takes the smallest res. */

   sizea best_index = (sizea)-1;
   char *best_p = NULL;
   const char *const*patlistp;
   char *p;

   if(s == NULL || patlist == NULL) return(NULL);

   /* Start scanning each pattern */
   patlistp = patlist;
   while(*patlistp != NULL && **patlistp != '\0') {
      p = strscan(s, *patlistp);
      if(p != NULL && (best_p == NULL || p < best_p)) {
         best_p = p;
         best_index = patlistp - patlist;
      }
      ++patlistp;
   }

   /* Return best result */
   if(index != NULL) *index = best_index;
   return(best_p);

}
