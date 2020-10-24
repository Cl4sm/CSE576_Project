/* R-Trim
   trims off trailing whitespace, \n, \t, etc off of the
   string passed in <cmd>. Since this function only removes characters,
   sizeof(final_<s>) <= sizeof(init_<s>), so we won't have to worry
   about overflowing memory or anything :) */

   /* Pointers into the string (used to parse whitespace out) */
   char *d = s;
   whitespace ws;

   if(s != NULL) {
      ws.lnws = s;
      SET_LAST_NWS(ws, s);
      *ws.lnws = '\0';
      return(d);
   }
   return(NULL);

}
