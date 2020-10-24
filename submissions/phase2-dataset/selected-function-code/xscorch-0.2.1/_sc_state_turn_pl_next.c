static void _sc_state_turn_pl_next(sc_config *c, sc_game *g) {

   ++g->curplayer;
   if(g->curplayer >= c->numplayers) switch(c->options.mode) {
      case SC_CONFIG_MODE_SEQUENTIAL:
         sc_game_set_state_now(c, g, SC_STATE_TURN_END);
         break;
      case SC_CONFIG_MODE_SYNCHRONOUS:
         g->curplayer = 0;
         #if USE_NETWORK
            sc_net_client_send_player_state(c, c->client);
            if(c->client != NULL) sc_status_message(c->window, "Waiting for other players to turn ...");
            sc_net_client_sync(c->client, SC_CONN_SYNC_TURN, c->server != NULL);
         #endif /* Network? */
         sc_game_set_state_asap(g, SC_STATE_RUN_TALK);
         break;
   } else {
      sc_game_set_state_now(c, g, SC_STATE_TURN_PL_BEGIN);
   }

}
