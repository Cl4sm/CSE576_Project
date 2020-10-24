static void _sc_expl_cache_annihilate_plasmoid_gtk(GdkImage *image, GdkColor *gradient,
                                                   int gsize, int radius) {
/* sc_expl_annihilate_plasmoid_gtk
   Annihilate a region centered at (cx,cy) for a radius r.  This function
   selects colors to form a plasma manifold pattern, whatever that means.  */

   int dx;           /* Delta X (distance away from cx) - iterator variable */
   int dy;           /* Delta Y (distance away from cy) for _edge_ of circle */
   int rad2;         /* Radius squared */
   int rad2major2;   /* Radius^2 + the major_distance^2 */
   int min2thresh;   /* Minimum threshold to avoid redrawing columns where dx>dy */
   unsigned char *fractal;     /* */
   int fsize;

   /* Allocate a new fractal. */
   fractal = sc_fractal_create(&fsize, radius + radius + 1);
   if(fractal == NULL) {
      _sc_expl_cache_annihilate_rad2_gtk(image, gradient, gsize, radius);
      return;
   }
   
   /* DX = major axis, DY = minor axis */
   dx = 0;           /* DX starts at zero (iterator) */
   dy = radius;      /* DY is one radius away (edge of circle at cx+dx) */
   rad2 = radius * radius; /* Calculate Radius Squared */
   rad2major2 = rad2;/* Radius^2 + major^2, running total */
   min2thresh = rad2 - dy; /* Minimum threshold before need to redraw edges */
   
   /* Should know that, we are incrementing DX every time.  However,
      if we call the transpose method every time as well, then we will
      be filling parts of the circle multiple times.  Hence the 
      min2thresh variable. */
   do {
      _sc_expl_cache_annihilate_column_plasmoid_gtk(image, gradient, gsize, 
                                                    dx, dy, radius, fractal, fsize);
      ++dx;
      rad2major2 -= (dx << 1) - 1;
      if(rad2major2 <= min2thresh) {
         _sc_expl_cache_annihilate_column_plasmoid_gtk(image, gradient, gsize, 
                                                       dy, dx, radius, fractal, fsize);
         --dy;
         min2thresh -= (dy << 1);
      }
   } while(dx <= dy);
   
   free(fractal);
   
}
