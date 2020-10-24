int sc_player_battery_count(const sc_config *c, const sc_player *p) {
/* sc_player_battery_count
   Determine the number of batteries in the player's inventory.  */

   int count;        /* Iterator variable */
   sc_accessory_info *info;

   /* Sanity check */
   if(c == NULL || p == NULL) return(false);

   /* Search a battery in inventory */
   count = sc_accessory_count(c->accessories, SC_ACCESSORY_LIMIT_ALL);
   info = sc_accessory_first(c->accessories, SC_ACCESSORY_LIMIT_ALL);
   for(; count > 0; --count) {
      if(SC_ACCESSORY_IS_BATTERY(info) && info->inventories[p->index] > 0) {
         return(info->inventories[p->index]);
      }
      info = sc_accessory_next(c->accessories, info, SC_ACCESSORY_LIMIT_ALL);
   }

   /* Failure, sorry */
   return(0);

}
