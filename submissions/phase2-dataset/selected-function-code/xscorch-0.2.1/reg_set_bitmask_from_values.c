                                 const char **names, const unsigned int *values) {
/* reg_set_bitmask_from_values
   Write the bitmask <value>, as its corresponding list of strings to the
   variable <path> (relative to <v>).  Enumerated types and corresponding
   names are also given.  If <path> == NULL, then modifications occur
   directly on <v>.  */

   char buf[REG_BUFFER];   /* Temp variable to construct value in */
   bool needcomma;         /* True if need preceding comma */
   int i;                  /* Index of matching element */

   *buf = '\0';
   needcomma = false;

   i = 0;
   while(names[i] != NULL) {
      if((values[i] & value) == values[i]) {
         value = value & (~values[i]);
         if(needcomma) strconcatb(buf, ", ", sizeof(buf));
         strconcatb(buf, names[i], sizeof(buf));
         needcomma = true;
      }
      ++i;
   }

   if(value != 0) return(false);

   return(reg_set_string(r, v, path, buf));

}
