reg_var *reg_block_resolve(reg *r, reg_var *v, const char *path) {
/* reg_block_resolve
   Resolve the specified pathname, relative to <v>.  If <path> == NULL,
   then <v> itself is returned.  Otherwise, the <path> is resolved,
   relative to <v>; if an error occurs along the way, then NULL will
   be returned.  Note, if <v> is not a block, and <path> is not NULL,
   then an error occurs.  */

   char firstelem[REG_SYMBOL_SIZE];
   reg_var *match;
   const char *p;

   if(v == NULL) v = r->top;
   if(v == NULL) return(NULL);
   if(path == NULL) return(v);
   if(v->type != REG_BLOCK) return(NULL);

   p = path;
   while(*p != '\0' && *p != '/') ++p;
   strcopynb(firstelem, path, p - path, REG_SYMBOL_SIZE);

   match = reg_var_lookup(v->value.block->vars, firstelem);
   if(match == NULL) return(NULL);
   if(*p == '\0') return(match);
   return(reg_block_resolve(r, match, p + 1));

}
