/* sc_land_support_shift_right
   Returns the offset the tank may shift to the RIGHT if it is not properly
   supported in that direction.  The tank is at coordinates (x,y+1) with a
   base radius of r.  If the tank is sufficiently supported to the RIGHT,
   then 0 is returned.  (x, y) may be virtual coordinates.  */

   int sx;     /* Start X -- first X index/coord which is clear below */
   int fx;     /* Final X -- last X coord which must be clear to slide */
   int cx;     /* Current X index/coordinate */

   /* Make sure Y translates properly. */
   if(!_sc_land_translate_y(l, &y)) return(0);

   /* Check where the "edge" of the land is -- hopefully it
      is within the first SC_TANK_MIN_SHELF_SIZE pixels of
      the left edge; if not, then the tank cannot slide down. */
   cx = 0;     /* 0 is leftmost edge of tank, advancing right */
   sx = 0;     /* Set to one past the left edge (0=no land below) */
   while(cx < s - 1) {
      /* If translation fails, we assume the tank is supported. */
      if(!_sc_land_passable_point(c, NULL, x - r + cx, y)) sx = cx + 1;
      ++cx;
   } /* Scan for edge of land */

   /* CX is now an actual X coordinate, starting at the
      first suspected clear tile from the left edge.
      Calculate the final X coordinate which must be
      clear for the tank to slide.  */
   cx = sx + x - r;
   fx = x + sx + r + 1;

   /* Make sure everything between (x-r+sx) and fx is clear below */
   while(cx <= fx) {
      /* If translation fails, assume tank cannot be moved. */
      if(!_sc_land_passable_point(c, NULL, cx, y)) return(0);
      ++cx;
   }

   /* Everything was clear; tank may shift over sx pixels to the right */
   return(sx);

}
