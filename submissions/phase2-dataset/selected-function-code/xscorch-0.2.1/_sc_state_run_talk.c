static void _sc_state_run_talk(sc_config *c, sc_game *g) {

   const char *msg;
   sc_player *p = c->plorder[g->curplayer];
   int nextstate = 0;

   sc_status_message(c->window, "");

   switch(c->options.mode) {
      case SC_CONFIG_MODE_SEQUENTIAL:
         nextstate = SC_STATE_RUN_CREATION;
         break;
      case SC_CONFIG_MODE_SYNCHRONOUS:
         if(g->curplayer + 1 >= c->numplayers) {
            nextstate = SC_STATE_RUN_CREATION;
         } else {
            nextstate = SC_STATE_RUN_TALK;
         }
         break;
   }

   if(nextstate == SC_STATE_RUN_TALK) ++g->curplayer;

   if(!p->dead && p->armed) {
      msg = sc_player_talk(c, p);
      if(!SC_CONFIG_GFX_FAST(c) && msg != NULL) {
         sc_status_player_message(c->window, p, msg);
         sc_game_set_state(g, nextstate, SC_DELAY_TALK);
         return;
      }
   }

   sc_game_set_state_now(c, g, nextstate);

}
