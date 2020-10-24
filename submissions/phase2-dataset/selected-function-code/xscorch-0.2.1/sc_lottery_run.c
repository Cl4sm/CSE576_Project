/* sc_lottery_run
   Give a random player a random weapon. */

   sc_weapon_info *info;
   int player = game_lrand(c->numplayers);
   int weapon = game_lrand(sc_weapon_count(c->weapons, SC_WEAPON_LIMIT_ALL));

   if(c == NULL) return;

   /* Get the first weapon. */
   info = sc_weapon_first(c->weapons, SC_WEAPON_LIMIT_ALL);

   /* Count up to the weapon specified. */
   while(info != NULL && weapon-- >= 0)
      info = sc_weapon_next(c->weapons, info, SC_WEAPON_LIMIT_ALL);

   /* Find the first non-useless, non-infinite weapon. */
   weapon = sc_weapon_count(c->weapons, SC_WEAPON_LIMIT_ALL);
   while(info != NULL && (SC_WEAPON_IS_USELESS(info) || SC_WEAPON_IS_INFINITE(info)) && --weapon > 0)
      info = sc_weapon_next(c->weapons, info, SC_WEAPON_LIMIT_ALL);

   /* Tell the user and bail if we just can't do it. */
   if(info == NULL || weapon == 0) {
      printf("lottery: unable to award a weapon because all are useless\n");
      return;
   }

   /* Give the player his award, if we can. */
   c->lottery->displayed = false;
   if(sc_inventory_award_weapon(info, player)) {
      /* Record the award for posterity. */
      c->lottery->winner = c->players[player];
      c->lottery->stake  = info;
   } else {
      /* No award to announce. */
      c->lottery->winner = NULL;
      c->lottery->stake  = NULL;
   }

   return;

}
