static inline void _sc_land_level(const sc_config *c, sc_land *l, int x, int ht) {
/* sc_land_level
   Internal function to level a column of land to the specified height.
   Caution, X is a real coordinate in this funciton, no sanity checks run.  */

   const int *gnd;   /* Ground indices for gradient */
   const int *sky;   /* Sky indices for gradient */
   int skyflag;      /* Sky type flag */
   bool dither;      /* Enable dithering? */
   int *lp;          /* Iterate through land */
   int height;       /* Current height */

   /* Set the current height to screen height */
   height = l->height - 1;

   /* Obtain the gradients we will be using */
   gnd = c->colors->gradindex[SC_GRAD_GROUND];
   sky = sc_land_sky_index(c);
   skyflag = sc_land_sky_flag(c);
   dither = c->graphics.gfxdither;

   /* Start at the top, clear everything above and including ht. */
   lp = SC_LAND_XY(l, x, height);
   while(height >= ht) {
      if(!SC_LAND_IS_SKY(*lp)) *lp = skyflag | sc_color_gradient_index(dither, sky, height);
      --lp;
      --height;
   } /* Clear to sky */
   /* Continue, this time filling everything below with ground. */
   while(height >= 0) {
      if(!SC_LAND_IS_GROUND(*lp)) *lp = SC_LAND_GROUND | sc_color_gradient_index(dither, gnd, height);
      --lp;
      --height;
   } /* Clear to ground. */

}
