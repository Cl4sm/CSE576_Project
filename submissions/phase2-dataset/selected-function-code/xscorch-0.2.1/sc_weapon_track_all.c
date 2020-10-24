sc_weapon_track_result sc_weapon_track_all(sc_config *c, sc_explosion **e) {
/* sc_weapon_track_all
   Track the weapons of every player, in sync.  This function will always
   process weapons for all players; if at least one player created a new
   explosion, it will return DETONATE.  If at least one player still needs
   tracking assistance, then it will return NEED_RECURSE.  This function
   only returns NO_ACTION if ALL players are done tracking.  */

   /* Note: we process all weapons in the chain, even after seeing a
      detonation.  This is to prevent other weapons from ``lagging''
      when one weapon (i.e. chaos) starts a long chain of detonates.
      In other words, every weapon gets its fair timeslice even if
      there are a lot of explosions going on. */

   sc_weapon_track_result result;
   int i;

   assert(c != NULL && e != NULL);

   result = SC_WEAPON_TRACK_NO_ACTION;
   for(i = 0; i < c->numplayers; ++i) {
      switch(_sc_weapon_track_chain(c, &(c->players[i]->weapons), e)) {
         case SC_WEAPON_TRACK_DETONATE:
            result = SC_WEAPON_TRACK_DETONATE;
            break;

         case SC_WEAPON_TRACK_NEED_RECURSE:
            if(result == SC_WEAPON_TRACK_NO_ACTION) {
               result = SC_WEAPON_TRACK_NEED_RECURSE;
            } /* Only if nothing occurred yet */
            break;

         default:
            /* do nothing */;
      }
   }
   return(result);

}
