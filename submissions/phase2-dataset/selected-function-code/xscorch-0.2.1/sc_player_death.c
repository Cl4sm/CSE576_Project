/* sc_player_death
   Nice knowin' ya... Note, this function may add new explosions to the
   queue e that is passed in.  This function does not (directly) modify
   the player state.  */

   sc_explosion *expl;

   assert(c != NULL && p != NULL && e != NULL);

   switch(game_lrand(10)) {

      case 0:
         /* Just say it with napalm ... */
         expl = sc_expl_new(p->x, p->y, c->weapons->scaling * SC_WEAPON_NAPALM_RADIUS,
                            SC_WEAPON_NAPALM_FORCE, p->killedby, SC_EXPLOSION_NAPALM);
         expl->data = sc_spill_new(c, c->land, SC_WEAPON_NAPALM_LIQUID, expl->centerx, expl->centery);
         sc_expl_add(e, expl);
         break;

      default:
         /* Just the usual, 3-stage detonation */
         sc_expl_add(e, sc_expl_new(p->x, p->y, c->weapons->scaling * SC_WEAPON_SMALL_EXPLOSION, 
                                    SC_WEAPON_SMALL_FORCE, p->killedby, SC_EXPLOSION_NORMAL));
         if(game_drand() < 0.5) {
            sc_expl_add(e, sc_expl_new(p->x, p->y, c->weapons->scaling * SC_WEAPON_MEDIUM_EXPLOSION, 
                                       SC_WEAPON_MEDIUM_FORCE, p->killedby, SC_EXPLOSION_NORMAL));
            if(game_drand() < 0.1) {
               sc_expl_add(e, sc_expl_new(p->x, p->y, c->weapons->scaling * SC_WEAPON_LARGE_EXPLOSION, 
                                          SC_WEAPON_LARGE_FORCE, p->killedby, SC_EXPLOSION_PLASMA));
            } /* Do third stage? */
         } /* Do second stage? */
         break;

   } /* End of switch */

}
