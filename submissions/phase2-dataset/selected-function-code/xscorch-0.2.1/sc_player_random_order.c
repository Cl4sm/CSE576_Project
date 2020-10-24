/* sc_player_random_order */

   int order[SC_MAX_PLAYERS];
   int i;
   int j;

   for(i = 0; i < c->numplayers; ++i) {
      order[i] = i;
   }

   for(i = 0; i < c->numplayers; ++i) {
      for(j = 0; j < c->numplayers; ++j) {
         if(i != j && game_lrand(100) < 50) {
            order[i] += order[j];
            order[j] = order[i] - order[j];
            order[i] = order[i] - order[j];
         }
      }
   }

   for(i = 0; i < c->numplayers; ++i) {
      playerlist[i] = c->players[order[i]];
   }

   return(playerlist);

}
