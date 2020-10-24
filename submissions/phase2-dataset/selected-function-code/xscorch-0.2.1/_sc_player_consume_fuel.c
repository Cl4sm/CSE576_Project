/* _sc_player_consume_fuel
   Used to eat up fuel, based on tank type and such. */

   sc_accessory_info *info;
   int count;

   if(p == NULL) return(false);

   /* We are basically looking to move one pixel at this point.
      Thus, we bring the fuel up to at least one unit positive.
      If we actually have enough, that is. */
   count = sc_accessory_count(ac, SC_ACCESSORY_LIMIT_ALL);
   info = sc_accessory_first(ac, SC_ACCESSORY_LIMIT_ALL);
   for(; count > 0; --count) {
      if(p->fuel >= SC_TANK_NORMAL_EFFICIENCY) break;
      if(SC_ACCESSORY_IS_FUEL(info)) {
         while(info->inventories[p->index] > 0 && p->fuel < SC_TANK_NORMAL_EFFICIENCY) {
            /* Scale game fuel to this tank's fuel rate. */
            p->fuel += info->fuel * p->tank->efficiency;
            --info->inventories[p->index];
         }
      }
      info = sc_accessory_next(ac, info, SC_ACCESSORY_LIMIT_ALL);
   }

   /* Actually consume some fuel if we can. */
   if(p->fuel < SC_TANK_NORMAL_EFFICIENCY) return(false);
   p->fuel -= SC_TANK_NORMAL_EFFICIENCY;
   return(true);

}
