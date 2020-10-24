/* reg_var_add
   Adds a new variable definition to the end of the linked list given.
   The variable will be uninitialised, but its type field will be set.
   This function may fail on an allocation error, or if name/type is
   invalid, etc...  */

   reg_var *var;       /* Newly constructed variable */
   reg_var *cur;       /* Pointer into linked list... */

   /* Make sure all pointers given are valid */
   if(name == NULL) return(NULL);
   if(vars == NULL) return(NULL);

   /* Make sure the name is valid, and not duplicated */
   if(*name == '\0') {
      reg_error(r, "Variable name is empty");
      return(NULL);
   } /* Name empty? */

   /* Create the new variable */
   var = (reg_var *)malloc(sizeof(reg_var));
   if(var == NULL) return(NULL);

   /* Initialise the data members */
   strcopyb(var->name, name, REG_SYMBOL_SIZE);
   var->type         = type;
   var->value.string = NULL;
   var->next         = NULL;

   /* Insert this variable to the list */
   if(*vars == NULL) {
      /* First variable in the list */
      *vars = var;
   } else {
      /* Search for end of list */
      cur = *vars;
      while(cur->next != NULL) cur = cur->next;
      cur->next = var;
   }

   /* Return the constructed variable */
   return(var);

}
