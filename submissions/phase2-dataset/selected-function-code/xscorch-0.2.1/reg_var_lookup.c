reg_var *reg_var_lookup(reg_var *vars, const char *name) {
/* reg_var_lookup
   Checks to see if the specified variable is already defined.  If it
   is, then we will return it here.  */

   /* Make sure all pointers given are valid */
   if(name == NULL) return(NULL);

   /* Search for the appropriate variable */
   while(vars != NULL) {
      if(strequal(vars->name, name)) {
         /* Found a matching variable */
         return(vars);
      }
      /* Try the next variable... */
      vars = vars->next;
   } /* Searching... */

   /* No match found */
   return(NULL);

}
