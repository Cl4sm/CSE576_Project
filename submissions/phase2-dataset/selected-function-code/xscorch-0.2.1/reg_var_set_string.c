bool reg_var_set_string(const reg *r, reg_var *b, const char *name, const char *value) {
/* reg_var_set_string
   Adds a new string variable.  */

   reg_var *var;       /* Newly constructed variable */

   /* Construct the variable */
   var = _reg_var_set(r, b, name, REG_STRING);
   if(var == NULL) return(false);

   /* Copy the new string into the variable */
   if(value == NULL) {
      var->value.string = NULL;
   } else {
      /* Source string was not NULL */
      var->value.string = (char *)malloc(strlenn(value) + 1);
      if(var->value.string != NULL) {
         strcopy(var->value.string, value);
      } /* Did malloc() succeed? */
   } /* Was source value nonnull? */
   return(true);

}
