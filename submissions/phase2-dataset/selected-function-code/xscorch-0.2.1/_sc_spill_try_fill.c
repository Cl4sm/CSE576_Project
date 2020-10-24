static bool _sc_spill_try_fill(const sc_config *c, const sc_land *l,
                               sc_spill *sp, int x, int y, bool allowup) {
/* sc_spill_try_fill
   Attempt to fill in a region of a spill.  If allowup is set, then
   if no alternative is available (we are in a valley) we are allowed
   to spill in an upward direction. Otherwise, spill to the left or
   right whenever possible.  True is returned if we were successful
   in spilling somewhere. */

   int i;

   /* Make sure this is a valid index and land location. */
   if(sp->index >= sp->size) {
      return(false);
   }
   if(!sc_land_translate_xy(l, &x, &y)) {
      return(false);
   }

   /* Check that this location is unoccupied. */
   if(SC_LAND_IS_SKY(*SC_LAND_XY(l, x, y))) {
      for(i = 0; i < sp->index; ++i) {
         if(x == sp->spillx[i] && y == sp->spilly[i]) {
            return(false);
         }
      } /* Scan that the current position unoccupied */

      /* We can spill here! */
      sp->spillx[sp->index] = x;
      sp->spilly[sp->index] = y;
      ++sp->index;

      /* Continue spilling... */
      _sc_spill_fill(c, l, sp, x, y, allowup);
      return(true);
   }

   /* We are on ground, cannot spill. */
   return(false);

}
