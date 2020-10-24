/* sc_land_support_shift_left
   Returns the offset the tank may shift to the LEFT if it is not properly
   supported in that direction.  The tank is at coordinates (x,y+1) with a
   base radius of r.  If the tank is sufficiently supported to the left,
   then 0 is returned.  (x, y) may be virtual coordinates.  */

   int sx;     /* Start X -- first X index/coord which is clear below */
   int fx;     /* Final X -- last X coord which must be clear to slide */
   int cx;     /* Current X index/coordinate */

   /* Make sure Y translates properly. */
   if(!_sc_land_translate_y(l, &y)) return(0);

   /* Check where the "edge" of the land is -- hopefully it
      is within the first SC_TANK_MIN_SHELF_SIZE pixels of
      the left edge; if not, then the tank cannot slide down. */
   cx = 0;     /* 0 is rightmost edge of tank, advancing to the left */
   sx = 0;     /* Set to one before the right edge (0=no land below) */
   while(cx < s - 1) {
      /* If translation fails, assume tank is supported. */
      if(!_sc_land_passable_point(c, NULL, x + r - cx, y)) sx = cx + 1;
      ++cx;
   }

   /* CX is now an actual X coordinate, starting at the
      first suspected clear tile from the right edge,
      and decrementing down to FX.  */
   cx = x + r - sx;
   fx = x - sx - r - 1;

   /* Make sure everything between fx and (x+r-sx) is clear below */
   while(cx >= fx) {
      /* If translation fails, assume tank cannot be moved. */
      if(!_sc_land_passable_point(c, NULL, cx, y)) return(0);
      --cx;
   }

   /* Everything was clear; tank may shift over sx pixels to the right */
   return(sx);

}
