static void _sc_state_turn_pl_done(sc_config *c, sc_game *g) {

   sc_player *p = c->plorder[g->curplayer];

   /* Arm the player's weapon */
   if(!p->dead) {
      p->armed = true;
      sc_window_redraw_tank(c->window, p);
   }

   sc_status_message(c->window, "");

   switch(c->options.mode) {
      case SC_CONFIG_MODE_SEQUENTIAL:
         #if USE_NETWORK
            sc_net_client_send_player_state(c, c->client);
            if(c->client != NULL) sc_status_message(c->window, "Waiting for other players to turn ...");
            sc_net_client_sync(c->client, SC_CONN_SYNC_TURN, c->server != NULL);
         #endif /* Network? */
         sc_game_set_state_asap(g, SC_STATE_RUN_TALK);
         break;
      case SC_CONFIG_MODE_SYNCHRONOUS:
         sc_game_set_state_now(c, g, SC_STATE_TURN_PL_NEXT);
         break;
   }

}
