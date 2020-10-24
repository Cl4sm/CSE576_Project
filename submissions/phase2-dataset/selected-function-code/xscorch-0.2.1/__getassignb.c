/* internal-Get-Assignment-Countbuffer
   This is a useful function for more than just CGI processing,
   actually. What this will do is it takes the string <s>, which
   is typically an argument out of a CGI string (but can be
   any string, like a line from a config file, f.e.), and it will
   chop off the variable name and the value assign if the string
   is of the form "variable = value". It will also trim off
   whitespace in both the variable name and the value. This
   function returns false if the string <s> is not an assign
   string (e.g., it does not contain a '=' character). The size
   of <var> and <val> can be specified in <lvar> and <lval>,
   respectively. If these values are zero then the <var> and/or
   <val> buffers are assumed to be at least as large as the
   source buffer <s>. Note: if ANY of the buffer pointers are
   NULL then this function will AUTOMATICALLY return false. */

   const char *p = s;
   const char *scanp;
   const_whitespace ws;
   sizea size;

   if(var != NULL && val != NULL && s != NULL) {
      *var = '\0';
      *val = '\0';

      /* Search for the equals sign */
      while(*p != '\0' && *p != '=') ++p;
      if(*p != '\0') {

         /* Get the value. */
         scanp = p + 1;
         SKIM_WHITESPACE(scanp);
         if(*scanp) {
            SET_FIRST_NWS(ws, scanp);
            SET_LAST_NWS(ws, scanp);
            size = NWS_SIZE(ws) + 1;
            if(lval > 0 && size > lval) size = lval;
            STRNCPY(val, ws.fnws, size);
         }

         /* Get the variable name. */
         scanp = s;
         SKIM_WHITESPACE_T(scanp, '=');
         if(scanp != p) {
            SET_FIRST_NWS(ws, scanp);
            SET_LAST_NWS_T(ws, scanp, '=');
            size = NWS_SIZE(ws) + 1;
            if(lvar > 0 && size > lvar) size = lvar;
            STRNCPY(var, ws.fnws, size);
         }
         return(true);

      }
   }

   /* Invalid arguments given */
   return(false);

}
