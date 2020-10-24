/* reg_block_resolve_container
   Resolve the container which holds the specified variable.
   Lookup rules are the same as for reg_block_resolve().  The
   final variable name may be copied into <varname> (which
   must be of size REG_SYMBOL_SIZE) if successful.

   Caution:  If <path> is NULL, then a container _cannot_ be
   returned for this variable.  This is a bug; I should be
   keeping a backpointer around, but I don't.  Oops.  */

   char firstelem[REG_SYMBOL_SIZE];
   reg_var *match;
   const char *p;

   if(v == NULL) v = r->top;
   if(v == NULL || path == NULL) return(NULL);
   if(v->type != REG_BLOCK) return(NULL);

   p = path;
   while(*p != '\0' && *p != '/') ++p;
   if(*p == '\0') {
      /* Found the container */
      if(varname != NULL) {
         strcopynb(varname, path, p - path, REG_SYMBOL_SIZE);
      }
      return(v);
   }

   strcopynb(firstelem, path, p - path, REG_SYMBOL_SIZE);
   match = reg_var_lookup(v->value.block->vars, firstelem);

   if(match == NULL) return(NULL);
   return(reg_block_resolve_container(r, match, p + 1, varname));

}
