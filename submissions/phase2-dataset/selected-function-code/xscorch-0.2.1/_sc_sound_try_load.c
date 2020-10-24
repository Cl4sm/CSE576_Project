/* sc_sound_try_load
   Attempt to load the song given by the ID.  On success, true is returned
   and s->module will refer to the song.  On failure, false is returned.
   Note, Any previously loaded song must be unloaded before this function 
   is called.  */

   /* Temporary buffer to write filename into */
   char filename[SC_SOUND_BUFFER];

   /* Construct the suspected filename for the song */
   sbprintf(filename, sizeof(filename), "%s/%s/%s", SC_GLOBAL_DIR, SC_SOUND_DIR, _sc_sound_names[id]);
   
   /* Attempt to load the song */
   s->module = Player_Load(filename, 64, 0);
   if(s->module == NULL) {
      /* Failure; see what went wrong, then give up */
      /* TEMP TEMP TEMP TEMP TEMP
         fprintf(stderr, "Could not load module \"%s\": \"%s\"\n", _sc_sound_names[id], MikMod_strerror(MikMod_errno));
       */
      return(0);
   }
   
   /* Success: set module parametres and exit */
   s->module->wrap = 1;
   s->module->loop = 1;
   s->module->reppos = SC_SOUND_REVERB;
   return(!0);

}
