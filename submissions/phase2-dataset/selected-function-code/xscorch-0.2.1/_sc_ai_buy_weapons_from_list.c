/* sc_ai_buy_weapons_from_list
   Buys weapons from a presorted list, beginning at the _end_ of the list.  */

   sc_weapon_info *info;   /* Weapon information. */
   int count = sc_weapon_count(c->weapons, SC_WEAPON_LIMIT_ALL);
   int inner, outer;             /* iterators */
   bool result = false;

   /* Iterate through, beginning with the "best" weapons. */
   for(outer = 0; outer < SC_AI_BUY_MAX_OF_WEAPON; ++outer) {
      for(inner = count - 1; inner >= 0; --inner) {
         info = sc_weapon_lookup(c->weapons, ordering[inner], SC_WEAPON_LIMIT_ALL);
         /* We don't want to buy weapons that won't kill things!
            We also bias slightly in favor of buying weapons we don't own yet. */
         if(sc_inventory_can_buy_weapon(p, info, *budget) &&
            sc_weapon_statistic(c->weapons, info, p, SC_WEAPON_STAT_YIELD) &&
            (outer || (info->inventories[p->index] / info->bundle) < SC_AI_INVENTORY_HIGH)) {
            /* buy some of this weapon. */
            sc_inventory_buy_weapon(p, info);
            *budget -= info->price;
            if(SC_AI_DEBUG_BUY) {
               printf("   AI_buy:   %s, %s bought %s.  $%d\n", sc_ai_name(p->aitype), p->name, info->name, *budget);
            }
            result = true;
         } /* Can we buy this weapon? */
      } /* Buying weapons */
   } /* Iterate multiples of weapons */

   return(result);

}
