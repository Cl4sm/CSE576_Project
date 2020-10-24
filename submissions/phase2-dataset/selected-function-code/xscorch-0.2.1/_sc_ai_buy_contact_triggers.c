static bool _sc_ai_buy_contact_triggers(const sc_config *c, sc_player *p, int *budget) {
/* sc_ai_buy_contact_triggers
   Buy contact triggers if tunneling is allowed. */

   sc_accessory_info *info;  /* Item information */
   int count = sc_accessory_count(c->accessories, SC_ACCESSORY_LIMIT_ALL);

   /* Only buy if tunneling is allowed. */
   if(!c->weapons->tunneling) return(false);

   /* Buy contact triggers if we can. */
   info = sc_accessory_first(c->accessories, SC_ACCESSORY_LIMIT_ALL | SC_ACCESSORY_SCAN_FORWARD);
   for(; count > 0; --count) {
      /* Make sure the damn thing is a contact trigger. */
      if(SC_ACCESSORY_IS_TRIGGER(info)) {
         while(sc_inventory_can_buy_accessory(p, info, *budget)) {
            /* Buy some of these contact triggers */
            sc_inventory_buy_accessory(p, info);
            *budget -= info->price;
            if(SC_AI_DEBUG_BUY) {
               printf("   AI_buy:   %s, %s bought %s.  $%d\n", sc_ai_name(p->aitype), p->name, info->name, *budget);
            }
            return(true);
         } /* Can we buy? */
      } /* Is accessory a contact trigger? */
      info = sc_accessory_next(c->accessories, info, SC_ACCESSORY_LIMIT_ALL | SC_ACCESSORY_SCAN_FORWARD);
   } /* Iterate through */

   return(false);

}
