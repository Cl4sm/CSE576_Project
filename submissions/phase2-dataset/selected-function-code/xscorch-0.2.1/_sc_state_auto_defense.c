/* _sc_state_auto_defense
   Select the auto defense state, and run it.
   The window code will advance the state for us when it's done. */

   if(g->curplayer >= c->numplayers) {
      /* Send the state machine on to display the next state. */
      g->curplayer = 0;
      sc_game_set_state_now(c, g, SC_STATE_LOTTERY_DISPLAY);
   } else if(c->players[g->curplayer]->aitype != SC_AI_NETWORK) {
      /* Run an auto defense session (maybe display) for the player. */
      if(sc_preround_auto_defense(c, c->players[g->curplayer])) {
         ++g->curplayer;
         sc_game_set_state(g, SC_STATE_AUTO_DEFENSE, SC_DELAY_PROMPT);
      } else {
         ++g->curplayer;
         sc_game_set_state_now(c, g, SC_STATE_AUTO_DEFENSE_LOOP);
      }
   } else {
      /* Try the next player. */
      ++g->curplayer;
      sc_game_set_state_now(c, g, SC_STATE_AUTO_DEFENSE_LOOP);
   }

}
