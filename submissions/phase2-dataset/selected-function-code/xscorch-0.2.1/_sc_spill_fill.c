static void _sc_spill_fill(const sc_config *c, const sc_land *l,
                           sc_spill *sp, int curx, int cury, bool allowup) {
/* sc_spill_fill
   Attempt to fill in a region of a spill.  This tries all directions from
   the current location in order to continue a spill; this assumes the current
   location has already been spilled (or is dirt).  */

   /* Randomly try to spill right/left first. */
   int randdir = game_lrand(2) * 2 - 1;

   /* Attempt spill below first; then try to right/left downward */
   _sc_spill_try_fill(c, l, sp, curx, cury - 1, false);
   _sc_spill_try_fill(c, l, sp, curx + randdir, cury - 1, false);
   _sc_spill_try_fill(c, l, sp, curx - randdir, cury - 1, false);

   /* Again attempt a spill, horizontally to right/left. */
   _sc_spill_try_fill(c, l, sp, curx + randdir, cury, false);
   _sc_spill_try_fill(c, l, sp, curx - randdir, cury, false);

   /* If we can go up and are forced to, then spill upward. Note:
      we can only spill upward from the center column, so this is
      the only case where allowup is set. */
   if(allowup) {
      _sc_spill_try_fill(c, l, sp, curx, cury + 1, true);
      _sc_spill_try_fill(c, l, sp, curx + randdir, cury + 1, true);
      _sc_spill_try_fill(c, l, sp, curx - randdir, cury + 1, true);
      /* TEMP JDS:  If napalm is no longer impacting deep in the earth
         then okay. But something occurs to me: it is POSSIBLE that a
         napalm weapon that tunnels will be buried by landfall before
         it gets a chance to spill.  */
      /* TEMP JTL:  It seems okay to me.  And I'm not sure the land-
         slide thing is a real issue for us.  People who don't want
         to risk it should have used contact triggers... */
   }

}
