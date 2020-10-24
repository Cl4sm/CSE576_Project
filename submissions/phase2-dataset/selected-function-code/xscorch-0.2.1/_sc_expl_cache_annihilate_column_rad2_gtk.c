static inline void _sc_expl_cache_annihilate_column_rad2_gtk(GdkImage *image, GdkColor *gradient, 
                                                             int gsize, int dx, int dy, 
                                                             int radius, int rad2) {
/* sc_expl_cache_annihilate_column_rad2_gtk
   Annihilate a column of an explosion.  (cx,cy) is the center coordinate of
   the explosion.  (dx,dy) is the deltax and deltay that this column begins
   at -- that is, the column goes from (cx+dx,cy-dy) to (cx+dx,cy+dy).  rad2
   is the radius, squared, of the entire explosion.  Note, colors fall off 
   with the square of the radius; this makes for more "3-D"istic explosions,
   and also happens to be much faster to calculate.  */

   int dx2y2;        /* dx^2 * y^2, for current value of y */
   int tile;         /* Tile value for explosion */
   int y;            /* Current y (offset from cy, y in [0,dy]) */

   /* Iteration for all four columns */
   y = 0;            /* We are starting on the Y axis */
   dx2y2 = dx * dx;  /* This starts off as x^2, since y=0 */
   do {
      /* Calculate tile code -- any way to optimize this? */
      tile = dx2y2 * gsize / rad2;

      /* Adjust dx2y2 for next value of y */
      /* x^2 + y^2 => x^2 + y^2 + y + (y+1) 
                    = x^2 + y(y+1) + (y+1) 
                    = x^2 + (y+1)^2          */
      /* Suggested by JJP */
      dx2y2 += (y << 1) | 1;

      /* Iterate through the four quadrants to update display */
      _sc_expl_cache_draw_points_gtk(image, gradient[tile].pixel, dx, y, radius);

      /* Next value of y (one step farther from central axis) */
      ++y;
   } while(y <= dy);

}
