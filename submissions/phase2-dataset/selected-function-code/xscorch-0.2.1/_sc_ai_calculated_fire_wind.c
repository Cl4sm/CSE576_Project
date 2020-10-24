static const sc_player *_sc_ai_calculated_fire_wind(const sc_config *c, sc_player *p) {
/* sc_ai_calculated_fire_wind
   See if we can shoot anyone with the above two trajectory mechanisms.  We
   check players in random order, and if we can reach any of them, then fire
   we go.  Otherwise, we fall back on random firing scheme.  This mechanism
   considers gravity and wind, which CALCULATER needs.  */

   const sc_player *playerlist[SC_MAX_PLAYERS]; /* List of players, random order */
   const sc_player *vp; /* Current "victim" */
   int victim;          /* Index to "victim" */

   assert(c != NULL && p != NULL);

   /* Get a random ordering of players */
   sc_player_random_order((sc_config *)c, (sc_player **)playerlist);
   _sc_ai_target_practice(c, (sc_player **)playerlist);

   /* Try to find a victim. */   
   victim = 0;
   while(victim < c->numplayers) {
      vp = playerlist[victim];
      if(p->index != vp->index && SC_PLAYER_IS_ALIVE(vp)) {
         /* "Victim" isn't ourself (that would suck) */
         if(_sc_ai_line_of_sight(c, p, vp, sc_ai_trajectory_line_wind, sc_ai_trajectory_wind)) {
            /* Victim selected */
            return(vp);
         }
      } /* Make sure victim isn't self */
      ++victim;   /* Next please */
   } /* Looking for a victim ... */

   /* No suitable victim; just fire at random. */
   return(_sc_ai_random_fire(c, p));

}
