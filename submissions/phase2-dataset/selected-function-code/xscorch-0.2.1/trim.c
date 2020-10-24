/* Trim
   trims off leading and trailing whitespace, \n, \t, etc off of the
   string passed in <cmd>. Since this function only removes characters,
   sizeof(final_<s>) <= sizeof(init_<s>), so we won't have to worry
   about overflowing memory or anything :) */

   /* Pointers into the string (used to parse whitespace out) */
   char *d = s;
   whitespace ws;

   if(s != NULL) {

      SKIM_WHITESPACE(s);
      SET_FIRST_NWS(ws, s);
      SET_LAST_NWS(ws, s);

      /* Copy the non-ws characters in <s>. */
      if(ws.fnws > d) MEMMOVE(d, ws.fnws, NWS_SIZE(ws));
      *(d + NWS_SIZE(ws)) = '\0';
      return(d);

   }
   return(NULL);

}
