/* sc_player_init_game
   Game initialization for this player */

   /* Initialise the player's currency and inventory */
   p->money = c->economics->initialcash;

   /* No wins (yet) */
   p->numwins = 0;
   p->kills = 0;
   p->suicides = 0;
   p->ac_state = 0;
   p->turret = 0;
   p->power  = 0;

   /* Set player arms level */
   p->armslevel = c->weapons->armslevel;

   /* Initialise AI */
   sc_ai_init_game(c, p);

}
