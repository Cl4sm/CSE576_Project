/* sc_land_new
   Setup a new landscape.  This function allocates a new land structure and
   initialises it with the flags and size given.  This function does not
   attempt to initialise the land; you must call sc_land_generate() yourself
   to create the land.  This function returns NULL if an error occurred.  */

   sc_land *l;    /* The newly allocated landscape, on success. */

   /* Allocate land pointer */
   l = (sc_land *)malloc(sizeof(sc_land));
   if(l == NULL) return(NULL);

   /* Setup the screens */
   l->screen = NULL;
   if(!sc_land_setup(l, width, height, flags)) {
      free(l);
      return(NULL);
   }

   /* Setup default land generation options */
   l->sky        = SC_SKY_NIGHT;
   l->hostileenv = false;
   l->bumpiness  = SC_LAND_BUMPINESS_DEF;
   l->generator  = SC_LAND_GEN_TRADITIONAL;

   /* Return the land structure */
   return(l);

}
