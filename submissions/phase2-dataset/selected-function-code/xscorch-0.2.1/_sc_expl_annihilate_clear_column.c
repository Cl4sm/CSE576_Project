static inline void _sc_expl_annihilate_clear_column(sc_config *c, sc_land *l,
                                                    int x, int y1, int y2) {
/* sc_expl_annihilate_clear_column
   Clears a column of everything in a column; everything in the column
   is set to SKY tiles.  This clears column x, from y1 up to y2 (y1 < y2)
   (inclusive of both endpoints).  This is an internal function only.  */

   const int *gradient; /* Sky gradient */
   int gradientflag;    /* Sky gradient flag */
   bool dither;         /* Enable dithering? */
   int *lp;             /* Pointer into land structure at (x, y) */
   int y;               /* Current Y coordinate */

   /* Get the sky gradient */
   gradient = sc_land_sky_index(c);
   gradientflag = sc_land_sky_flag(c);
   dither = c->graphics.gfxdither;

   /* Boundary checks */
   if(!sc_land_translate_x(l, &x)) return;
   if(y1 < 0) y1 = 0;
   if(y2 >= c->fieldheight) y2 = c->fieldheight - 1;

   /* Boundary checks have already been performed */
   y = y1;
   lp = SC_LAND_XY(l, x, y);
   while(y <= y2) {
      *lp = gradientflag | sc_color_gradient_index(dither, gradient, y);
      ++lp;
      ++y;
   }

}
