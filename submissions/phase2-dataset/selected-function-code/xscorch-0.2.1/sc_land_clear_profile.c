void sc_land_clear_profile(const sc_config *c, sc_land *l, const sc_player *p) {
/* sc_land_clear_profile
   This function is a sanity check used by players, to make certain
   there is sky behind their profile.  */

   const unsigned char *data; /* Profile data */
   const int *sky;   /* Sky indices for gradient */
   int skyflag;      /* Sky type flag */
   int dither;       /* Dither flags */
   int radius;       /* profile radius */
   int width;        /* profile width */
   int tx;           /* Translated X */
   int ty;           /* Translated Y */
   int x;            /* Current offset X */
   int y;            /* Current offset Y */

   /* Sanity check */
   if(c == NULL || l == NULL || p == NULL) return;

   /* Obtain dithering information */
   sky = sc_land_sky_index(c);
   skyflag = sc_land_sky_flag(c);
   dither = c->graphics.gfxdither;

   /* Cache height and width locally, for efficiency */
   radius = p->tank->radius;
   width  = radius + radius + 1;

   /* Scan through all land, replacing profile points with sky */
   data = p->tank->data;
   for(y = radius; y >= 0; --y) {
      for(x = 0; x < width; ++x, ++data) {
         if(*data != SC_TANK_PROFILE_CLEAR) {
            tx = p->x + x - radius;
            ty = p->y + y;
            if(_sc_land_translate_x(l, &tx) && _sc_land_translate_y(l, &ty)) {
               *SC_LAND_XY(l, tx, ty) = skyflag | sc_color_gradient_index(dither, sky, ty);
            } /* Translation ok */
         } /* Point is in profile */
      } /* Loop X */
   } /* Loop Y */

}
