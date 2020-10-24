
   bool exit;
   int startline;
   reg_var *block;
   reg_var *subblock;
   char buf[REG_BUFFER];
   char var[REG_BUFFER];
   char val[REG_BUFFER];

   startline = r->line;
   block = reg_var_new_block(r, name, reg_class_lookup(r->classes, klass));
   if(block == NULL) return(NULL);

   exit = false;
   while(!exit && fgets(buf, REG_BUFFER, r->handle) != NULL) {
      ++r->line;
      escaped_chop(buf, ';');
      trim(buf);

      if(_sc_line_is_block_begin(r, buf, var, val)) {
         subblock = _reg_load_block(r, var, val);
         if(subblock == NULL) {
            reg_error_line2(r, "   in block \"%s\"(%d)", name, startline);
            reg_var_free(&block);
            return(false);
         }
         if(!reg_var_merge_block(r, block, var, &subblock)) {
            reg_error_line3(r, "Failed to add subblock \"%s\" to block \"%s\"(%d) (continuable error)",
                            var, name, startline);
         }

      } else if(_sc_line_is_block_end(r, buf)) {
         exit = true;

      } else if(_sc_line_is_assignment(buf, var, val)) {
         if(!reg_var_set_by_value(r, block, var, val)) {
            reg_error_line3(r, "Failed to add variable \"%s\" to block \"%s\"(%d) (continuable error)",
                            var, name, startline);
         }

      } else if(*buf == '\0') {
         /* Do nothing */

      } else {
         /* Invalid line */
         reg_error_line2(r, "Parse error in block \"%s\"(%d)", name, startline);
         reg_var_free(&block);
         return(false);

      } /* What type of line is this? */

   } /* Reading lines from the file ... */

   if(!exit) {
      reg_error_line2(r, "Premature end-of-block \"%s\"(%d)", name, startline);
      reg_var_free(&block);
      return(false);
   }

   /* Return this block */
   return(block);

}
