bool reg_set_string_from_values(reg *r, reg_var *v, const char *path, int value,
                                const char **names, const unsigned int *values) {
/* reg_set_string_from_values
   Write the enumerated value <value>, as its corresponding string to
   the variable <path> (relative to <v>).  The enumerated types and
   corresponding names are also given.

   If <path> == NULL, then modifications occur directly on <v>.  */

   int index;              /* Index to matching name-value pair */

   /* Attempt to get index matching up with value */
   index = _reg_value_to_index(value, names, values);
   if(index >= 0) {
      /* We have it; try to set this as a string value. */
      return(reg_set_string(r, v, path, names[index]));
   } /* Match found? */

   /* No matches; cannot handle this case */
   return(false);

}
