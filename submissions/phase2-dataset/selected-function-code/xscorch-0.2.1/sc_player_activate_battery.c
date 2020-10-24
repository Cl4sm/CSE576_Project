/* sc_player_activate_battery
   Activates a player battery, if any are available.  This will
   allow the player to partially recharge their tank.  */

   int count;        /* Iterator variable */
   int maxlife;      /* Max life, scaled to hardness */
   sc_accessory_info *info;

   /* Sanity check */
   if(c == NULL || p == NULL) return(false);
   maxlife = SC_PLAYER_MAX_LIFE * p->tank->hardness;
   if(p->life >= maxlife) return(false);  /* No recharge needed */

   /* Search for batteries in the inventory */
   count = sc_accessory_count(c->accessories, SC_ACCESSORY_LIMIT_ALL);
   info = sc_accessory_first(c->accessories, SC_ACCESSORY_LIMIT_ALL);
   for(; count > 0; --count) {
      if(SC_ACCESSORY_IS_BATTERY(info) && info->inventories[p->index] > 0) {
         /* Activate this battery */
         --info->inventories[p->index];
         p->life += SC_BATTERY_RECHARGE_PERCT * maxlife / 100;
         if(p->life > maxlife) p->life = maxlife;
         /* Update state */
         #if USE_NETWORK
            sc_net_client_send_battery(c, c->client, p->index);
         #endif
         sc_status_update(c->window, p);
         return(true);
      }
      info = sc_accessory_next(c->accessories, info, SC_ACCESSORY_LIMIT_ALL);
   }

   /* Failure, sorry */
   return(false);

}
