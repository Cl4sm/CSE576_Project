/* sc_ai_buy_last_shields
   Buy the last shields on the list that we can afford.  This is a rather
   naive approach, in assuming that shields at the end of the list must
   surely be better?  */

   sc_accessory_info *info;  /* Shield information */
   int count = sc_accessory_count(c->accessories, SC_ACCESSORY_LIMIT_ALL);

   /* Buy some shielding */
   info = sc_accessory_first(c->accessories, SC_ACCESSORY_LIMIT_ALL | SC_ACCESSORY_SCAN_REVERSE);
   for(; count > 0; --count) {
      if(sc_inventory_can_buy_accessory(p, info, *budget)) {
         /* Make sure the damn thing is a shield. */
         if(SC_ACCESSORY_IS_SHIELD(info)) {
            /* Buy some of this shield */
            sc_inventory_buy_accessory(p, info);
            *budget -= info->price;
            if(SC_AI_DEBUG_BUY) {
               printf("   AI_buy:   %s, %s bought %s.  $%d\n", sc_ai_name(p->aitype), p->name, info->name, *budget);
            }
            return(true);  /* That's all, for now */
         } /* Is accessory a shield? */
      } /* Can we buy? */
      info = sc_accessory_next(c->accessories, info, SC_ACCESSORY_LIMIT_ALL | SC_ACCESSORY_SCAN_REVERSE);
   } /* Iterate up the accessory list. */

   return(false);

}
