
   reg_var *top;
   reg_var *subblock;
   char buf[REG_BUFFER];
   char var[REG_BUFFER];
   char val[REG_BUFFER];

   top = r->top;

   while(fgets(buf, REG_BUFFER, r->handle) != NULL) {
      ++r->line;
      escaped_chop(buf, ';');
      trim(buf);

      if(_sc_line_is_block_begin(r, buf, var, val)) {
         subblock = _reg_load_block(r, var, val);
         if(subblock == NULL) {
            reg_error_line1(r, "   in block \"%s\"", top->name);
            return(false);
         }
         if(!reg_var_merge_block(r, top, var, &subblock)) {
            reg_error_line2(r, "Failed to add subblock \"%s\" to top block \"%s\" (continuable error)",
                            var, top->name);
         }

      } else if(_sc_line_is_block_end(r, buf)) {
         reg_error_line1(r, "Cannot exit top block, \"%s\" (continuable error)", top->name);

      } else if(_sc_line_is_assignment(buf, var, val)) {
         if(!reg_var_set_by_value(r, top, var, val)) {
            reg_error_line2(r, "Failed to add variable \"%s\" to block \"%s\" (continuable error)",
                            var, top->name);
         }

      } else if(*buf == '\0') {
         /* Do nothing */

      } else {
         /* Invalid line */
         reg_error_line1(r, "Parse error in block \"%s\"", top->name);
         return(false);

      } /* What type of line is this? */

   } /* Reading lines from the file ... */

   return(true);

}
