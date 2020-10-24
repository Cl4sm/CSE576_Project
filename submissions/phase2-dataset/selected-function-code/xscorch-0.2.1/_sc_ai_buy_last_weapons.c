/* sc_ai_buy_last_weapons
   Buy the last weapons on the list that we can afford.  This is a rather
   naive approach, in assuming that weapons at the end of the list must
   surely be better?  */

   sc_weapon_info *info;        /* Weapon information */
   int count = sc_weapon_count(c->weapons, SC_WEAPON_LIMIT_ALL);
   int inner, outer;            /* Iterators */
   bool result = false;

   for(outer = SC_AI_BUY_MAX_OF_WEAPON; outer > 0; --outer) {
      info = sc_weapon_first(c->weapons, SC_WEAPON_LIMIT_ALL | SC_WEAPON_SCAN_REVERSE);
      for(inner = count; inner > 0; --inner) {
         if(sc_inventory_can_buy_weapon(p, info, *budget) &&
            sc_weapon_statistic(c->weapons, info, p, SC_WEAPON_STAT_YIELD) > 0) {
            /* Buy some of this weapon. */
            sc_inventory_buy_weapon(p, info);
            *budget -= info->price;
            if(SC_AI_DEBUG_BUY) {
               printf("   AI_buy:   %s, %s bought %s.  $%d\n", sc_ai_name(p->aitype), p->name, info->name, *budget);
            }
            result = true;
         } /* Can we buy this weapon? */
         info = sc_weapon_next(c->weapons, info, SC_WEAPON_LIMIT_ALL | SC_WEAPON_SCAN_REVERSE);
      } /* Iteration on info lists */
   } /* Iterate multiples of weapons */

   return(result);

}
