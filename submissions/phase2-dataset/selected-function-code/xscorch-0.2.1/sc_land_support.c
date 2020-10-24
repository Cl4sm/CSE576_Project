/* sc_land_support
   Returns the offset the tank may be shifted over if it is not properly
   supported on one side.  The tank is at coordinates (x,y) with a base
   radius of r.  If the tank is sufficiently supported on both sides, then 0
   is returned.  Otherwise, a positive value implies the tank will slide
   down the slope to the right, and a negative value implies that the tank
   will slide down the slope to the left.  */

   int delta;  /* Distance to slide over */

   /* If already on the floor, the tank cannot slide any further */
   if(l == NULL || --y < 0) return(0);

   /* Check if the tank can slide in either direction */
   delta = _sc_land_support_shift_right(c, l, x, y, r, s);
   if(delta != 0) return(delta);/* Slide to the right */
   delta = _sc_land_support_shift_left(c, l, x, y, r, s);
   if(delta != 0) return(-delta);/* Slide to the left */

   /* Tank is sufficiently supported. */
   return(0);

}
