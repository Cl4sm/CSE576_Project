/* sc_player_total_fuel
   Used to discover how much fuel a player has, for the movement menu. */

   const sc_accessory_info *info;
   int count;
   int fuel = 0;

   if(p == NULL) return(0);

   /* This translates this tank's tank fuel rate to the game fuel rate. */
   fuel = INT_ROUND_DIV(p->fuel, p->tank->efficiency);

   /* Find unopened fuel in the inventory. */
   count = sc_accessory_count(ac, SC_ACCESSORY_LIMIT_ALL);
   info = sc_accessory_first(ac, SC_ACCESSORY_LIMIT_ALL);
   for(; count > 0; --count) {
      if(SC_ACCESSORY_IS_FUEL(info)) {
         fuel += info->fuel * info->inventories[p->index];
      }
      info = sc_accessory_next(ac, info, SC_ACCESSORY_LIMIT_ALL);
   }

   return(fuel);

}
