void sc_dialog_error(const char *s) {

   char *err;
   char *buf;
   int length;

   err = strerror(errno);
   if(err == NULL) {
      sc_dialog_message("Error", s);
      return;
   }

   length = strlenn(s) + strlenn(err) + 4;

   buf = (char *)malloc(length);
   if(buf == NULL) {
      sc_dialog_message("Error", s);
      return;
   }

   sbprintf(buf, length, "%s: %s", s, err);
   sc_dialog_message("Error", buf);
   free(buf);
   return;

}
