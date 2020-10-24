
   bool result;

   #if SC_STATE_DEBUG
   printf("\nrun_explosion:  entering function\n");
   #endif /* SC_STATE_DEBUG */

   do {
      #if SC_STATE_DEBUG
      printf("run_explosion:  queues init %d  draw %d  clear_init %d  clear %d  done %d\n",
             sc_expl_count(g->expl_init),
             sc_expl_count(g->expl_draw),
             sc_expl_count(g->expl_clear_init),
             sc_expl_count(g->expl_clear),
             sc_expl_count(g->expl_done));
      #endif /* SC_STATE_DEBUG */
      if(c->options.interleave) {
         #if SC_STATE_DEBUG
         printf("run_explosion:  doing simultaneous\n");
         #endif /* SC_STATE_DEBUG */
         result = _sc_state_run_explosion_simult(c, g);
      } else {
         #if SC_STATE_DEBUG
         printf("run_explosion:  doing non-simultaneous\n");
         #endif /* SC_STATE_DEBUG */
         result = _sc_state_run_explosion_nosimult(c, g);
      }
   } while(result);

   #if SC_STATE_DEBUG
   printf("run_explosion:  done\n");
   #endif /* SC_STATE_DEBUG */

   /* Update the counters */
   sc_game_expl_queue_update(g->expl_init);
   sc_game_expl_queue_update(g->expl_draw);
   sc_game_expl_queue_update(g->expl_clear_init);
   sc_game_expl_queue_update(g->expl_clear);
   sc_game_expl_queue_update(g->expl_done);

   /* Which state do we go to next? */
   sc_game_sync_timeout(g);
   g->activity = g->activity || SC_STATE_ANY_EXPL(g);
   if(c->options.interleave) {
      sc_game_set_state_now(c, g, SC_STATE_RUN_PLAYER_DROP);
   } else {
      if(SC_STATE_ANY_EXPL(g)) {
         /* Still processing explosions... */
         sc_game_set_state_allow_now(c, g, SC_STATE_RUN_EXPLOSION, SC_STATE_FAST(c, SC_STATE_MACHINE_INTERVAL));
      } else {
         /* No more explosions to process, we may continue. */
         sc_game_set_state_now(c, g, SC_STATE_RUN_PLAYER_DROP);
      } /* Any explosions left? */
   } /* Interleaving? */

}
