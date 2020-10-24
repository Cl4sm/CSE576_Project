sc_sound *sc_sound_new(int enable, int hqmixer) {

   sc_sound *s;

   if(!enable) return(NULL);

   s = (sc_sound *)malloc(sizeof(sc_sound));
   if(s == NULL) return(NULL);
   s->playing = 0;
   s->module = NULL;

   _sc_sound_mdmode(s, hqmixer);

   /* Beware the kludgery */
   #if LIBMIKMOD_VERSION >= 0x030107
   if(MikMod_Init("")) {
   #else /* Version is old */
   if(MikMod_Init()) {
   #endif /* Libmikmod ok? */
      fprintf(stderr, "Could not initialise sound: \"%s\"\n", MikMod_strerror(MikMod_errno));
      sc_sound_free(&s);
      return(NULL);
   }
   
   return(s);
   
}
