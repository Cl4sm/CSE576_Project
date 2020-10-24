static inline sc_weapon_track_result _sc_weapon_track_chain(sc_config *c, sc_weapon **wp, sc_explosion **e) {
/* sc_weapon_track_chain
   Track a weapon chain for a single player.  This processes all
   weapons in the chain; it will return either:
      NO_ACTION, indicating that tracking has completed
      NEED_RECURSE, indicating that nothing of particular interest
         happened but tracking is not completed (we need to call
         this function again),
      DETONATE, indicating that at least one detonation occurred.
         Tracking will resume once the detonation is processed.

   All weapons in the weapon chain wp will be processed, even if some
   weapons at the head of the list explode, and any explosions are
   added to the explosions list e.  */

   sc_weapon_track_result result;

   assert(c != NULL && wp != NULL && e != NULL);

   result = SC_WEAPON_TRACK_NO_ACTION;
   while(*wp != NULL) {
      switch(_sc_weapon_track(c, wp, e)) {
         case SC_WEAPON_TRACK_DETONATE:
            result = SC_WEAPON_TRACK_DETONATE;
         case SC_WEAPON_TRACK_SIZZLE:
            /* In these cases, sc_weapon_track has already removed the
               current weapon from the queue, so wp is already updated
               to point to the next weapon. */
            break;

         case SC_WEAPON_TRACK_NEED_RECURSE:
            if(result == SC_WEAPON_TRACK_NO_ACTION) {
               result = SC_WEAPON_TRACK_NEED_RECURSE;
            } /* Only if nothing occurred yet */
         case SC_WEAPON_TRACK_NO_ACTION:
            /* Advance to the next weapon in the queue. */
            wp = &((*wp)->chain);

      }
   }
   return(result);

}
