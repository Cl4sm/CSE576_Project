/* sc_line_is_block_begin */

   char *p;

   p = escaped_scan(buf, '{');
   if(p == NULL) return(false);

   *p++ = '\0';
   trim(p);
   if(*p != '\0') {
      reg_error_line(r, "Garbage after '{' (warning only)");
   }

   if(!_sc_line_is_assignment(buf, name, klass)) {
      reg_error_line(r, "Block needs a name and a class");
      return(false);
   }

   return(true);

}
