static bool _sc_ai_buy_batteries(const sc_config *c, sc_player *p, int *budget) {
/* sc_ai_buy_batteries
   Buys some batteries for the AI.  */

   sc_accessory_info *info;
   int count = sc_accessory_count(c->accessories, SC_ACCESSORY_LIMIT_ALL);

   /* Buy batteries if we can. */
   info = sc_accessory_first(c->accessories, SC_ACCESSORY_LIMIT_ALL | SC_ACCESSORY_SCAN_FORWARD);
   for(; count > 0; --count) {
      /* Make sure the damn thing is a battery. */
      if(SC_ACCESSORY_IS_BATTERY(info)) {
         while(sc_inventory_can_buy_accessory(p, info, *budget) && sc_player_battery_count(c, p) < SC_AI_MAX_BATTERIES) {
            /* Buy some of these here batteries */
            sc_inventory_buy_accessory(p, info);
            *budget -= info->price;
            if(SC_AI_DEBUG_BUY) {
               printf("   AI_buy:   %s, %s bought %s.  $%d\n", sc_ai_name(p->aitype), p->name, info->name, *budget);
            }
            return(true);
         } /* Can we buy? */
      } /* Is accessory a battery? */
      info = sc_accessory_next(c->accessories, info, SC_ACCESSORY_LIMIT_ALL | SC_ACCESSORY_SCAN_FORWARD);
   } /* Iterate through */

   return(false);

}
