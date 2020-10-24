                                const char **names, const unsigned int *values) {
/* reg_get_integer_from_values
   Read the variable <path>, relative to <v>, and attempt to translate
   its string content to an enumerated type value.  If the value cannot
   be obtained (variable undefined, not a string, etc) then <defvalue>
   will be returned.  The enumerated types and corresponding names are
   given as arguments to this function.

   If <path> == NULL, then <v> is read.  */

   char buf[REG_BUFFER];   /* Temporary read-buffer */
   int index;              /* Index to matching name-value pair */

   /* Attempt to retrieve the string */
   if(reg_get_string(r, v, path, buf, REG_BUFFER)) {
      /* We have a string to work with, yay.  Does it
         have a corresponding entry in the values list? */
      trim(buf);
      index = _reg_text_to_index(buf, names);
      if(index >= 0) {
         /* Assign the new default value */
         defvalue = values[index];
      } /* Text matches? */
   } /* Is variable a valid string? */

   /* Return the enumerated value */
   return(defvalue);

}
