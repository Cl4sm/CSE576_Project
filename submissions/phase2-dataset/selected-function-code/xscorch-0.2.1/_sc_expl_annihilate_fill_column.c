                                                   int x, int y1, int y2) {
/* sc_expl_annihilate_fill_column
   Fills a column with land.  This fills any clear tiles in column x
   with GROUND tiles, from y1 up to y2 (y1 < y2) (inclusive of both
   endpoints).  This is an internal function only.  */

   const int *gradient; /* Sky gradient */
   bool dither;         /* Enable dithering? */
   int *lp;             /* Pointer into land structure at (x, y) */
   int y;               /* Current Y coordinate */

   /* Get the sky gradient */
   gradient = c->colors->gradindex[SC_GRAD_GROUND];
   dither = c->graphics.gfxdither;

   /* Boundary checks */
   if(!sc_land_translate_x(l, &x)) return;
   if(y1 < 0) y1 = 0;
   if(y2 >= c->fieldheight) y2 = c->fieldheight - 1;

   /* Boundary checks have already been performed */
   y = y1;
   lp = SC_LAND_XY(l, x, y);
   while(y <= y2) {
      if(SC_LAND_IS_SKY(*lp)) {
         *lp = SC_LAND_GROUND | sc_color_gradient_index(dither, gradient, y);
      } /* Was the tile originally sky? */
      ++lp;
      ++y;
   }

}
