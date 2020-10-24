/* sc_land_generate
   Generate a new landscape.  The land is generated based on definitions in
   the configuration structure given, and is created using the profile
   generators.  If true is returned, then the land has been successfully
   generated.  If false is returned, the screen is still uninitialised.  */

   /* Land profile variables */
   double unit_x;    /* Column's x-coordinate within unit interval */

   /* Other variables */
   const int *gnd;   /* Ground indices for gradient */
   const int *sky;   /* Sky indices for gradient */
   int skyflag;      /* Sky type flag */
   int *lpointer;    /* Pointer into the screen. */
   int nearly_there; /* Current land height minus zero to two pixels */
   double height;    /* Current land height (Y) */
   bool dither;      /* Enable dithering? */
   int fheight;      /* Land height */
   int width;        /* Land width */
   int x;            /* Current column */
   int y;            /* Iterator down column */

   /* Sanity checks */
   if(c == NULL || l == NULL) return(false);

   /* Initialise landscapes */
   /* JS:  This must be called whenever land is generated, because it
      generates random data.  Random tables will be out-of-sync if
      this is called before network setup!!! */
   sc_lscape_init();

   /* Setup random sky if needed */
   if(l->sky == SC_SKY_RANDOM) l->realsky = game_lrand(SC_SKY_RANDOM);
   else l->realsky = l->sky;

   /* Setup ground and sky */
   gnd = c->colors->gradindex[SC_GRAD_GROUND];
   sky = sc_land_sky_index(c);
   skyflag = sc_land_sky_flag(c);
   dither = c->graphics.gfxdither;

   /* Create a profile for the landscape */
   sc_lscape_setup(c, l);

   /* Okay, here we go ... */
   fheight  = l->height;
   width    = l->width;
   x        = width;
   lpointer = SC_LAND_ORIGIN(l);
   while(x > 0) {
      /* current column's x-coordinate within the unit interval and
         evaluate the profile function at that point. */
      unit_x = (double)x / width;
      height = sc_lscape_eval(unit_x) * c->maxheight;
      nearly_there = (int)height - 2;

      /* Sanity checks */
      if(nearly_there > fheight) nearly_there = fheight;
      if(height > fheight) height = fheight;

      /* Start at the base of the hill, and fill in ground until we reach
         height.  The remaining part of the column should be filled in with
         the current sky.  */
      y = 0;
      while(y < nearly_there) {
         *lpointer = SC_LAND_GROUND | sc_color_gradient_index(dither, gnd, y);
         ++lpointer;
         ++y;
      }
      /* MH: Draw a thin rind in slightly different colour */
      while(y < height) {
         *lpointer = SC_LAND_GROUND | sc_color_gradient_index(dither, gnd, 0);
         ++lpointer;
         ++y;
      } /* Creating ground.. */
      while(y < fheight) {
         *lpointer = skyflag | sc_color_gradient_index(dither, sky, y);
         ++lpointer;
         ++y;
      } /* Creating sky.. */

      /* Next column */
      --x;
   } /* Iterating through each column */

   return(true);

}
