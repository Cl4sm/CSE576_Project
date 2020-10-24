/* sc_sound_reset
   Reset the mikmod driver - Warning: all sound samples and modules must
   be reloaded after this call.  Otherwise, nasty segfaults will occur.  */

   /*
   if(s->playing) Player_Stop();
   if(s->module != NULL) Player_Free(s->module);
   */

   #if LIBMIKMOD_VERSION >= 0x030107
   if(MikMod_Reset("")) {
   #else /* Version is old */
   if(MikMod_Reset()) {
   #endif /* Libmikmod ok? */
      fprintf(stderr, "Could not reinitialise sound: \"%s\"\n", MikMod_strerror(MikMod_errno));
   }
   s->playing = 0;

}
