/* sc_player_winner_order */

   sc_player *tmp;
   int i;
   int j;

   sc_player_random_order(c, playerlist);

   for(i = 1; i < c->numplayers; ++i) {
      for(j = 0; j < i; ++j) {
         if(playerlist[i]->kills - playerlist[i]->suicides > playerlist[j]->kills - playerlist[j]->suicides) {
            tmp = playerlist[i];
            playerlist[i] = playerlist[j];
            playerlist[j] = tmp;
         }
      }
   }

   return(playerlist);

}
