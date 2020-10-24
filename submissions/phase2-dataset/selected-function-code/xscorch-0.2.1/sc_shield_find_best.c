/* sc_shield_find_best
   Find the best shield in a player's inventory.
   This is very accurate, but also very sensitive to shield definition changes. */

   int count;        /* Iterator variable */
   sc_accessory_info *info, *best = NULL;

   /* Sanity check */
   if(c == NULL || p == NULL) return(false);

   /* Search for the best shield in inventory */
   count = sc_accessory_count(c->accessories, SC_ACCESSORY_LIMIT_ALL);
   info = sc_accessory_first(c->accessories, SC_ACCESSORY_LIMIT_ALL | SC_ACCESSORY_SCAN_REVERSE);
   for(; count > 0; --count) {
      if(SC_ACCESSORY_IS_SHIELD(info) && info->inventories[p->index] > 0) {
         if(best == NULL) {
            best = info;
         } else {
            if(info->shield > best->shield) {
               best = info;
            } else if(info->shield == best->shield) {
               if(SC_ACCESSORY_SHIELD_IS_FORCE(info)) {
                  best = info;
               } else if(SC_ACCESSORY_SHIELD_IS_STANDARD(info) && SC_ACCESSORY_SHIELD_IS_MAGNETIC(best)) {
                  best = info;
               } else if(SC_ACCESSORY_SHIELD_IS_MAGNETIC(info) && SC_ACCESSORY_SHIELD_IS_MAGNETIC(best)) {
                  if(info->repulsion > best->repulsion) {
                     best = info;
                  } /* shield has more repulsion? */
               } /* shield better by type? */
            } /* shield obviously better, or just maybe better? */
         } /* found a shield already? */
      } /* shield we own? */
      info = sc_accessory_next(c->accessories, info, SC_ACCESSORY_LIMIT_ALL | SC_ACCESSORY_SCAN_REVERSE);
   }

   /* done */
   return(best);

}
