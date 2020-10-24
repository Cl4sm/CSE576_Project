/* sc_ai_buy_shield_sappers
   Buy up Shield Sappers, which have no yield,
   but AIs can use them against shielded players. */

   sc_weapon_info *info;        /* Weapon information */
   int count = sc_weapon_count(c->weapons, SC_WEAPON_LIMIT_ALL);
   int sapbudget = *budget / 4; /* Sappers shouldn't drain the budget */
   int inner, outer;            /* Iterators */
   bool result = false;

/* TEMP - We're not using this for now.  See the comment in the
   shield sappers section of saiturn.c for more information...  */
return(result);

   for(outer = SC_AI_BUY_MAX_OF_WEAPON; outer > 0; --outer) {
      info = sc_weapon_first(c->weapons, SC_WEAPON_LIMIT_ALL | SC_WEAPON_SCAN_REVERSE);
      for(inner = count; inner > 0; --inner) {
         if(sc_inventory_can_buy_weapon(p, info, sapbudget) && SC_WEAPON_IS_SAPPER(info)) {
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
