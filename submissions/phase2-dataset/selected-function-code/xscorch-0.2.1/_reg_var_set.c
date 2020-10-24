/* reg_var_set */

   reg_var *var;       /* Variable created or to set */
   reg_type regtype;   /* Variable type required by vars */
   reg_block *blk;     /* Block data */

   /* Make sure type agrees with vars (if applicable) */
   blk = b->value.block;
   regtype = reg_var_info_lookup(blk->klass, name);
   if(regtype != REG_ANY && regtype != type) {
      /* Sorry, types don't match */
      reg_error1(r, "Type must match vars for \"%s\"", name);
      return(NULL);
   } /* Does type agree with vars? */

   /* Check if the variable has already been defined */
   var = reg_var_lookup(blk->vars, name);
   if(var != NULL) {
      /* Variable already exists */
      _reg_var_clear(var);
      var->type = type;
   } else {
      /* Variable did not exist; create it */
      var = _reg_var_add(r, &blk->vars, name, type);
   }

   /* Return the variable */
   return(var);

}
