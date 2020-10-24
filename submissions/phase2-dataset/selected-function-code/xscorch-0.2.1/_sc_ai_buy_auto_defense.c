/* sc_ai_buy_auto_defense
   Buy auto-defense, only if this is a sequential game. */

   sc_accessory_info *info;  /* Item information */
   int count = sc_accessory_count(c->accessories, SC_ACCESSORY_LIMIT_ALL);

   /* Check if this is a sequential game. */
   if(c->options.mode == SC_CONFIG_MODE_SYNCHRONOUS) return(false);

   /* If this is a sequential game, make damn sure we buy autodefense */
   info = sc_accessory_first(c->accessories, SC_ACCESSORY_LIMIT_ALL | SC_ACCESSORY_SCAN_FORWARD);
   for(; count > 0; --count) {
      if(sc_inventory_can_buy_accessory(p, info, *budget)) {
         /* Make sure the damn thing is auto-defence. */
         if(SC_ACCESSORY_IS_AUTO_DEF(info)) {
            /* Buy some of this autodefense */
            sc_inventory_buy_accessory(p, info);
            *budget -= info->price;
            if(SC_AI_DEBUG_BUY) {
               printf("   AI_buy:   %s, %s bought %s.  $%d\n", sc_ai_name(p->aitype), p->name, info->name, *budget);
            }
            return(true);
         } /* Is accessory auto-defense? */
      } /* Can we buy? */
      info = sc_accessory_next(c->accessories, info, SC_ACCESSORY_LIMIT_ALL | SC_ACCESSORY_SCAN_FORWARD);
   } /* Iterate through */

   return(false);

}
