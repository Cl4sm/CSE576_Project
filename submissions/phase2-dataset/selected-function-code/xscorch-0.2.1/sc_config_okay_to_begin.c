bool sc_config_okay_to_begin(const sc_config *c) {
/* sc_config_okay_to_begin
   Returns true if it is okay to begin a new game. This performs some
   sanity checking (right number of players, network game setup as
   appropriate, etc) and returns true if all sanity checks pass. Note
   this will try to spawn off some error windows if it finds a prob. */

   bool networkplay;
   int  numhuman;
   int  i;

   /* Number of players? */
   if(c->numplayers < 2) {
      sc_window_message( c->window, "Cannot Start", 
                         "There are fewer than 2 players in the game. "\
                         "If this is a network game, wait for someone "\
                         "else to connect. If this is a private game, "\
                         "add some human or AI players." );
      return(false);
   }

   /* No network AI players in nonnetwork game; human count okay? */
   #if USE_NETWORK
   networkplay = (c->client != NULL);
   #else  /* Network not allowed */
   networkplay = false;
   #endif /* Network game? */
   numhuman = 0;

   /* Count human and network players */
   for(i = 0; i < c->numplayers; ++i) {
      if(c->players[i]->aitype == SC_AI_HUMAN) ++numhuman;
      else if(c->players[i]->aitype == SC_AI_NETWORK && !networkplay) {
         sc_window_message( c->window, "Cannot Start",
                            "A network player has been selected, but "\
                            "we are not in a network game. This should "\
                            "not have happened; please go to the Players "\
                            "window and setup the players to all be human "\
                            "or AI players." );
         return(false);
      }
   }

   /* Make sure there is a human player */
   if(numhuman == 0) {
      sc_window_message( c->window, "Cannot Start",
                         "There are no (local) human players in the game. "\
                         "Please go to the Players window and setup some "\
                         "players to be Human." );
      return(false);
   }

   /* Okay, we can start */
   return(true);

}
