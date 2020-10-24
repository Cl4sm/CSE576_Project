/* reg_var_merge_block
   Merges the given block into an existing block (if available).  If
   necessary, a new block will be created.  Variables in <block> will
   override the old vars in <b> that were set.  WARNING: This function may
   destroy the block pointer given, after it is finished.  */

   reg_var *var;       /* Newly constructed variable */
   reg_var **newlist;  /* New variable list to merge */
   reg_block *blk;     /* Original block object */
   reg_block *newblk;  /* New block object to merge */
   bool advance;       /* Advance pointer? */

   /* Make sure pointers are valid */
   if(b == NULL || block == NULL || *block == NULL) return(false);

   /* Make sure b, block are valid blocks */
   if(b->type != REG_BLOCK || (*block)->type != REG_BLOCK) return(false);
   blk = b->value.block;
   newblk = (*block)->value.block;

   /* Search for an existing block */
   var = reg_var_lookup(blk->vars, name);
   if(var == NULL || var->type != REG_BLOCK || var->value.block->klass != newblk->klass) {
      /* No variable by that name found, or it wasn't a block */
      return(reg_var_set_block(r, b, name, block));
   } /* Does variable exist? Is it a block? */

   /* Variable is a block; we must merge given block into it */
   newlist = &newblk->vars;
   while(*newlist != NULL) {
      advance = true;
      switch((*newlist)->type) {
         case REG_INTEGER:
            if(!reg_var_set_integer(r, var, (*newlist)->name, (*newlist)->value.integer)) return(false);
            break;

         case REG_DOUBLEV:
            if(!reg_var_set_doublev(r, var, (*newlist)->name, (*newlist)->value.doublev)) return(false);
            break;

         case REG_BOOLEAN:
            if(!reg_var_set_boolean(r, var, (*newlist)->name, (*newlist)->value.boolean.value, (*newlist)->value.boolean.format)) return(false);
            break;

         case REG_STRING:
            if(!reg_var_set_string(r, var, (*newlist)->name, (*newlist)->value.string)) return(false);
            break;

         case REG_BLOCK:
            if(!reg_var_merge_block(r, var, (*newlist)->name, newlist)) return(false);
            advance = false;
            break;

         case REG_ANY:
            break;
      }
      if(advance) newlist = &((*newlist)->next);
   } /* Merging ... */

   /* Destroy the block */
   reg_var_free(block);

   /* Return success */
   return(true);

}
