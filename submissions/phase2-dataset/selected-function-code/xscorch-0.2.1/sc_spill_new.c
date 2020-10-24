                       int size, int centerx, int centery) {
/* sc_spill_new
   Create a new spill structure. This initialises the array that
   will describe the coordinates of the spill, in an order that is
   appropriate for animating the flow of the spill.  Specify the
   explosion coordinates (usually at the topmost layer of the
   ground) where the spill starts, and the number of pixels that
   should be flooded by the spill.  */

   sc_spill *sp;
   int *lp;
   int i;

   /* Sanity checks */
   assert(c != NULL);
   assert(l != NULL);
   assert(size >= 0);

   /* Allocate spill data structures */
   sp = (sc_spill *)malloc(sizeof(sc_spill));
   if(sp == NULL) {
      return(NULL);
   }
   sp->spillx = (int *)malloc(sizeof(int) * size);
   if(sp->spillx == NULL) {
      free(sp);
      return(NULL);
   }
   sp->spilly = (int *)malloc(sizeof(int) * size);
   if(sp->spilly == NULL) {
      free(sp->spillx);
      free(sp);
      return(NULL);
   }

   /* Make sure the centerx, centery coordinate is valid */
   if(!sc_land_translate_xy(l, &centerx, &centery)) {
      /* Offscreen and unable to compensate; the spill is NULL */
      sp->size = 0;
      sp->index = 0;
      sp->count = 0;
      for(i = 0; i < size; ++i) {
         sp->spillx[i] = -1;
         sp->spilly[i] = -1;
      }
      return(sp);
   }

   /* Initialise the spill structure */
   sp->size = size;
   sp->index = 0;
   sp->count = 0;
   for(i = 0; i < size; ++i) {
      sp->spillx[i] = centerx;
      sp->spilly[i] = centery;
   }

   /* Clear land data for the impact crater... */
   lp = SC_LAND_XY(l, centerx, centery);
   *lp = sc_land_sky_flag(c) |
         sc_color_gradient_index(c->graphics.gfxdither,
                                 sc_land_sky_index(c), centery);
   sc_window_paint(c->window, centerx, centery, 1, 1,
                   SC_REGENERATE_LAND | SC_PAINT_EVERYTHING);

   /* Compute the spill; note, we may not be able to spill all
      the requested number of pixels, therefore size must be
      updated after this computation to reflect the actual number
      of pixels flooded (not the requested number). */
   _sc_spill_fill_initial(c, l, sp, centerx, centery);
   sp->size = sp->index;
   sp->index = 0;

   /* Return the spill structure */
   return(sp);

}
