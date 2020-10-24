bool sc_land_line_of_sight(const sc_config *c, const sc_land *l, int x1, int y1, int x2, int y2) {
/* sc_land_line_of_sight
   Returns nonzero if (x1,y1) can see (x2,y2).  This function will take
   wrapping boundaries and solid walls into account, if they are available.
   x1, y1, x2, y2 are all virtual coordinates.  */

   double x;               /* Current X coordinate */
   double y;               /* Current Y coordinate */
   double stepx;           /* Step X, per iteration */
   double stepy;           /* Step Y, per iteration */
   int numsteps;           /* Total number of iterations */
   int deltax;             /* Total change in X */
   int deltay;             /* Total change in Y */
   int cx;                 /* Current X (rounded) */
   int cy;                 /* Current Y (rounded) */

   /* Sanity checks */
   if(c == NULL || l == NULL) return(false);

   /* Determine the distance between points */
   deltax = x2 - x1;
   deltay = y2 - y1;

   /* If (x1,y1) == (x2,y2), nothing to do! */
   if(deltax == 0 && deltay == 0) return(true);

   /* Which axis is the major axis?  The major axis will determine
      the total number of steps we will need to take.  */
   if(abs(deltax) > abs(deltay)) {
      /* X axis is the major axis. */
      numsteps = abs(deltax);
   } else {
      /* Y axis is the major axis. */
      numsteps = abs(deltay);
   } /* Which axis is major? */

   /* Start at (x1,y1). Setup stepping in each direction. */
   x = x1;
   y = y1;
   stepx = (double)deltax / numsteps;
   stepy = (double)deltay / numsteps;

   /* Iterate! If this iteration completes, then we have
      a direct line of sight between (x1,y1) and (x2,y2). */
   while(numsteps > 0) {
      /* Round current coordinates to nearest integer. */
      cx = rint(x);
      cy = rint(y);

      /* Translate the X, Y coordinates */
      if(_sc_land_translate_x(l, &cx) && _sc_land_translate_y(l, &cy)) {

         /* If we passed the ground, we're going through solid. */
         if(cy < 0) return(false);
         if(!SC_LAND_IS_SKY(*SC_LAND_XY(l, cx, cy))) {
            return(false);
         }

      } /* Translation valid? */

      /* Advance to the next coordinate to be checked. */
      x += stepx;
      y += stepy;
      --numsteps;
   }

   /* WE have a direct line-of-sight! */
   return(true);

}
