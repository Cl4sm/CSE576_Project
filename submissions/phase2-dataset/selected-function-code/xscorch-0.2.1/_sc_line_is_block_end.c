/* sc_line_is_block_end */

   char *p;

   p = escaped_scan(buf, '}');
   if(p == NULL) return(false);

   *p++ = '\0';
   trim(p);
   if(*p != '\0') {
      reg_error_line(r, "Garbage after '}' (warning only)");
   }

   trim(buf);
   if(*buf != '\0') {
      reg_error_line(r, "Garbage before '}' (warning only)");
   }

   return(true);

}
