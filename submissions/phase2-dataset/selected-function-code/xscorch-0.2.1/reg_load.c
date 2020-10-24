bool reg_load(reg *r) {

   if(r == NULL) return(false);

   r->line = 0;
   r->handle = fopen(r->filename, "r");
   if(r->handle == NULL) {
      reg_error(r, "Cannot open file to load");
      return(false);
   }

   if(!_reg_load_top(r)) {
      reg_error(r, "Nonrecoverable error occurred");
      fclose(r->handle);
      return(false);
   }

   fclose(r->handle);
   return(true);

}
