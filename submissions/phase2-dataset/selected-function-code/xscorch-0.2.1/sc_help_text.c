/* sc_help_text */

   char buf[0x1000];
   char key[0x1000];
   int keysize;
   bool match;
   bool done;
   int offset = 0;
 
   /* Sanity checks */
   if(res == NULL || size <= 0 || idphrase == NULL) return;

   /* Prep and size the ID string */
   sbprintf(key, sizeof(key), "\\fB%s\\fP", idphrase);
   keysize = strblenn(key, sizeof(key));

   /* Scan help file for the requested text */
   match = false;
   while(!match && sgetline(buf, sizeof(buf), data_xscorch_man, &offset) != NULL) {
      match = strequaln(buf, key, keysize);
   }

   /* Make sure we found something */
   if(!match) {
      sbprintf(res, size, "Cannot find help text for %s.", idphrase);
      return;
   }

   /* Start writing data to the result buffer until we reach a blank line */
   done = false;
   strcopyb(res, idphrase, size);
   strconcatb(res, ": ", size);
   while(!done && sgetline(buf, sizeof(buf), data_xscorch_man, &offset) != NULL) {
      if(*buf == '.' && !strequaln(buf, ".\\\"", 3)) done = true;
      else {
         /* Copy this line (unmunged) into the help buffer */
         strconcatb(res, buf, size);
      }
   }

   /* We are done! */
   _unmunge_line(res);
   _trim_line(res);

}
