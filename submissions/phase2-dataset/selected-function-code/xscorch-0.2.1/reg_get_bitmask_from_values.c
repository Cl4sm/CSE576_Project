int reg_get_bitmask_from_values(reg *r, reg_var *v, const char *path, int defvalue,
                                const char **names, const unsigned int *values) {
/* reg_get_bitmask_from_values
   Read the variable <path>, relative to <v>, and attempt to translate its
   string content to a bitmask value.  If the value cannot be obtained
   (variable undefined, not a string, etc) then <defvalue> will be returned.
   The enumerated types and corresponding names are given as arguments to
   this function.  If <path> == NULL, then <v> is read.  */

   char buf[REG_BUFFER];   /* Temporary read-buffer */
   char *ps;               /* Start of `current' bit */
   char *p;                /* Start of `next' bit */
   bool negate;            /* True if this bit negated */
   int  index;             /* Matching name-value pair */

   /* Attempt to retrieve the string */
   if(reg_get_string(r, v, path, buf, REG_BUFFER)) {
      /* Trim-o-rama, and loop while the string isn't empty */
      trim(buf);
      p = buf;
      while(*p != '\0') {
         /* Look for a comma or end-of-string */
         ps = p;
         while(*p != '\0' && *p != ',') ++p;

         /* If we hit a comma, there is another bit to parse */
         if(*p == ',') *(p++) = '\0';

         /* Look for a negation flag (if present) */
         trim(ps);
         negate = (*buf == '!');
         if(negate) trim(++ps);

         /* Attempt to find bitvalue corresponding to text */
         index = _reg_text_to_index(ps, names);
         if(index >= 0) {
            /* We have a matching bitvalue; mix it into default */
            index = values[index];
            if(negate) defvalue = defvalue & (~index);
            else defvalue = defvalue | index;
         } /* Found a matching bitvalue? */

      } /* Looping through named bits */
   } /* Was variable a valid string? */

   /* Return the newly constructed bitfield */
   return(defvalue);

}
