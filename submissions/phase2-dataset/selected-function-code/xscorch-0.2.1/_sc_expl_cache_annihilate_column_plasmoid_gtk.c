static inline void _sc_expl_cache_annihilate_column_plasmoid_gtk(GdkImage *image, GdkColor *gradient,
                                                                 int gsize, int dx, int dy, int radius,
                                                                 unsigned char *fractal, int fsize) {
/* sc_expl_cache_annihilate_column_plasmoid_gtk
   Annihilate a column of an explosion.  (cx,cy) is the center coordinate of
   the explosion.  (dx,dy) is the deltax and deltay that this column begins
   at -- that is, the column goes from (cx+dx,cy-dy) to (cx+dx,cy+dy).  rad2
   is the radius, squared, of the entire explosion.  Note, colors are selected
   as a plasmoid, whatever that is.  */

   int dx2y2;        /* dx^2 * y^2, for current value of y */
   int tile;         /* Tile value for explosion */
   int y;            /* Current y (offset from cy, y in [0,dy]) */
   int radminusdx;   /* Value of radius - dx */
   int radplusdx;    /* Value of radius + dx */

   /* Precompute as much as possible. */
   radminusdx = radius - dx;
   radplusdx  = radius + dx;

   /* Iteration for all four columns */
   y = 0;            /* We are starting on the Y axis */
   dx2y2 = dx * dx;  /* This starts off as x^2, since y=0 */
   do {
      /* Warning: the division must remain inlined with the rest of the
         computation; it cannot be lifted out since this is integer-level
         arithmetic. */
   
      /* Calculate tile code (quadrant 1) */
      tile = *(fractal + (radius + y) * fsize + radplusdx)  * gsize / 0x100;
      gdk_image_put_pixel(image, radplusdx,  radius + y, gradient[tile].pixel);

      /* Calculate tile code (quadrant 2) */
      tile = *(fractal + (radius + y) * fsize + radminusdx) * gsize / 0x100;
      gdk_image_put_pixel(image, radminusdx, radius + y, gradient[tile].pixel);

      /* Calculate tile code (quadrant 3) */
      tile = *(fractal + (radius - y) * fsize + radminusdx) * gsize / 0x100;
      gdk_image_put_pixel(image, radminusdx, radius - y, gradient[tile].pixel);

      /* Calculate tile code (quadrant 4) */
      tile = *(fractal + (radius - y) * fsize + radplusdx)  * gsize / 0x100;
      gdk_image_put_pixel(image, radplusdx,  radius - y, gradient[tile].pixel);
   
      /* Adjust dx2y2 for next value of y */
      /* Suggested by JJP */
      dx2y2 += (y << 1) | 1;

      /* Next value of y (one step farther from central axis) */
      ++y;
   } while(y <= dy);

}
