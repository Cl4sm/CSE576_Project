
   sc_player *p;
   const char *msg;

   sc_status_message(c->window, "");
   if(g->substate >= c->numplayers) {
      sc_game_set_state(g, SC_STATE_RUN_WEAPONS, SC_STATE_FAST(c, SC_STATE_MACHINE_INTERVAL));
   } else {
      p = c->players[g->substate];
      if(!p->dead && p->life <= 0) {
         /* This player just died */
         g->activity = true;
         msg = sc_player_death_talk(c, p);
         if(msg != NULL) {
            sc_status_player_message(c->window, p, msg);
         } else {
            sc_status_player_message(c->window, p, "");
         } /* Should player give farewell message? */
         sc_player_death(c, p, &g->expl_init);
         sc_player_died(c, p);
         sc_window_undraw_tank(c->window, p);
         if(c->options.interleave) {
            ++g->substate;
            sc_game_reinstate_now(c, g);
         } else {
            sc_game_set_state_now(c, g, SC_STATE_RUN_EXPLOSION);
            ++g->substate;
         }
      } else {
         ++g->substate;
         sc_game_reinstate_now(c, g);
      }
   }

}
