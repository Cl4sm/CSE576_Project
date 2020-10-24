/* sc_preround_lottery
   Run the pre-round display of lottery results.
   Will return true if a wait is expected. */

   bool showstake = false;
   int playerid;

   /* Sanity checks. */
   if(c == NULL || c->lottery == NULL || c->lottery->winner == NULL) return(false);

   /* Decide whether or not to show the stake (local human player). */
   for(playerid = 0; playerid < c->numplayers; ++playerid)
      if(c->players[playerid]->aitype == SC_AI_HUMAN && c->lottery->winner->index == playerid) {
         showstake = true;
         break;
      }

   /* Paste up the window. */
   sc_window_lottery_result(c->window, showstake);
   return(true);

}
