
   int i;

   /* Clear the player location table */
   for(i = 0; i < SC_MAX_PLAYERS; ++i) {
      c->field_position[i] = -1;
   }

   sc_physics_init_round(c->physics);
   sc_land_setup(c->land, c->fieldwidth, c->fieldheight, sc_land_flags(c));
   sc_land_generate(c, c->land);
   for(i = 0; i < c->numplayers; ++i) {
      sc_player_init_round(c, c->players[i]);
   }
   ++c->curround;

   if(c->curround == 0) {
      sc_player_random_order(c, c->plorder);
   } else switch(c->options.order) {
      case SC_CONFIG_ORDER_RANDOM:
         sc_player_random_order(c, c->plorder);
         break;
      case SC_CONFIG_ORDER_ROUND_ROBIN:
         _sc_config_rotate_players(c);
         break;
      case SC_CONFIG_ORDER_WINNERS_FIRST:
         sc_player_winner_order(c, c->plorder);
         break;
      case SC_CONFIG_ORDER_LOSERS_FIRST:
         sc_player_loser_order(c, c->plorder);
         break;
   }

   /* Run the lotto, if it's turned on. */
   if(c->economics->lottery)
      sc_lottery_run(c);

   c->componly = false;

}
