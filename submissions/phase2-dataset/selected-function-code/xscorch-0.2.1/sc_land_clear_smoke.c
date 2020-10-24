void sc_land_clear_smoke(const sc_config *c, sc_land *l) {
/* sc_land_clear_smoke
   Clears the smoke off the land given. */

   const int *sky;   /* Sky indices for gradient */
   int skyflag;      /* Sky type flag */
   int dither;       /* Dither flags */
   int height;       /* Land height */
   int width;        /* Land width */
   int *lp;          /* Land pointer */
   int x;            /* Current real X */
   int y;            /* Current real Y */

   /* Sanity check */
   if(c == NULL || l == NULL) return;

   /* Obtain dithering information */
   sky = sc_land_sky_index(c);
   skyflag = sc_land_sky_flag(c);
   dither = c->graphics.gfxdither;

   /* Cache height and width locally, for efficiency */
   height = l->height;
   width  = l->width;

   /* Scan through all land, replacing smoke with sky */
   for(x = 0; x < width; ++x) {
      lp = SC_LAND_XY(l, x, 0);
      for(y = 0; y < height; ++lp, ++y) {
         if(SC_LAND_IS_SMOKE(*lp)) *lp = skyflag | sc_color_gradient_index(dither, sky, y);
      } /* Iterating thru Y */
   } /* Iterating thru X */

   /* Redraw the playfield */
   sc_window_paint(c->window, 0, 0, l->width, l->height, SC_REGENERATE_LAND | SC_PAINT_EVERYTHING);

}
