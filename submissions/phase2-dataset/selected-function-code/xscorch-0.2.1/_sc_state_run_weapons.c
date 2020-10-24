
   switch(sc_weapon_track_all(c, &g->expl_init)) {
      case SC_WEAPON_TRACK_NO_ACTION:
         /* Weapons have all terminated */
         if(c->options.interleave && g->activity) {
            g->activity = false;
            sc_game_set_state_now(c, g, SC_STATE_RUN_EXPLOSION);
         } else {
            sc_game_set_state_now(c, g, SC_STATE_RUN_END);
         }
         break;

      case SC_WEAPON_TRACK_NEED_RECURSE:
         /* Weapons are still flying, but no detonations yet */
         if(SC_STATE_ANY_EXPL(g)) {
            sc_game_set_state_now(c, g, SC_STATE_RUN_EXPLOSION);
         } else {
            sc_game_reinstate(g, SC_STATE_FAST(c, SC_STATE_MACHINE_INTERVAL));
         }
         break;

      case SC_WEAPON_TRACK_DETONATE:
         /* Weapons may still be flying; a detonation occurred */
         /* Move all explosions in init to the draw queue */
         sc_game_set_state_now(c, g, SC_STATE_RUN_EXPLOSION);
         break;

      default:
         /* Oops */;
   }

}
