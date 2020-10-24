
   if(r == NULL) return(false);

   r->handle = fopen(r->filename, "w");
   if(r->handle == NULL) {
      reg_error(r, "Cannot open file to save");
      return(false);
   }

   fprintf(r->handle, "; This is an automatically generated file\n\n\n");
   fprintf(r->handle, "; Warning: there is _no_ error checking on values in this file.\n");
   fprintf(r->handle, "; This would make for a rather convenient way to set invalid values.\n");
   fprintf(r->handle, "; Maybe it could even be exploited to enable hidden `features'.\n\n\n");
   _reg_save_block(r, r->top, 0);

   fprintf(r->handle, "; End of file\n");
   fclose(r->handle);
   return(true);

}
