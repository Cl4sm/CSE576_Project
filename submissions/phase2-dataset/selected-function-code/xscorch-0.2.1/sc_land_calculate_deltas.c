bool sc_land_calculate_deltas(const sc_land *l, int *deltax, int *deltay, int x1, int y1, int x2, int y2) {
/* sc_land_calculate_deltas
   Calculates the short-path deltaX, deltaY between two virtual coordinates,
   taking boundaries into effect. This fixes issues with delta calculation
   that arise when wrapping boundaries are used.  In all other cases, the
   delta calculated is the expected delta after the coordinates are
   translated to real coordinates (so the screen delta, in effect).  Returns
   false if the translations failed for some reason.  If x2 > x1, then dx
   will be positive.  CAUTION:  This function does NOT concern itself with
   points that are out-of-bounds, this is to allow explosions centered off
   the screen to work properly.  As a result, if translation fails, then
   success still occurs!  */

   /* NOTE: changes to this code should also be made to the double code. */

   int dx;        /* Normal DX */
   int dy;        /* Normal DY */
   int d2x;       /* Alternate DX */

   /* Sanity check */
   if(l == NULL) return(false);

   /* translate the coordinates */
   _sc_land_translate_x(l, &x1);
   _sc_land_translate_x(l, &x2);
   _sc_land_translate_y(l, &y1);
   _sc_land_translate_y(l, &y2);

   /* Calculate the `normal' deltas */
   dx = x2 - x1;
   dy = y2 - y1;

   /* If land is wrapping, then calculate an alternate */
   if(l->flags & SC_LAND_WRAP_X) {
      d2x = dx + l->width;
      if(abs(d2x) < abs(dx)) dx = d2x;
   }

   /* Set pointers and return success. */
   if(deltax != NULL) *deltax = dx;
   if(deltay != NULL) *deltay = dy;
   return(true);

}
