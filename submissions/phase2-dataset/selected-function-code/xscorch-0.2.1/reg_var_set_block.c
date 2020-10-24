bool reg_var_set_block(const reg *r, reg_var *b, const char *name, reg_var **block) {
/* reg_var_set_block
   Adds a new block variable.  */

   reg_var *var;       /* Newly constructed variable */
   const reg_class *klass;  /* Block class in vars */
   reg_block *blk;     /* Original block object */
   reg_block *newblk;  /* New block object to assign */

   /* Assert that b,  is a block. */
   if(b == NULL || b->type != REG_BLOCK) return(false);
   if(block == NULL || *block == NULL || (*block)->type != REG_BLOCK) return(false);
   blk = b->value.block;
   newblk = (*block)->value.block;

   /* Make sure class agrees with vars (if applicable) */
   klass = reg_var_info_lookup_class(blk->klass, name);
   if(klass != NULL && newblk->klass != klass) {
      /* Sorry, types don't match */
      reg_error1(r, "Object class must match vars for \"%s\"", name);
      return(false);
   } /* Does type agree with vars? */

   /* Create the variable and assign the block to it */
   var = _reg_var_set(r, b, name, REG_BLOCK);
   if(var == NULL) return(false);
   var->value.block = newblk;

   /* Delete the original */
   (*block)->value.block = NULL;
   reg_var_free(block);
   return(true);

}
