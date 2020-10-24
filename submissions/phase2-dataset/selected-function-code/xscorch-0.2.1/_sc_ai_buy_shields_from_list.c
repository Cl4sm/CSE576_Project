/* sc_ai_buy_shields_from_list
   Buy shields from the ordered list given; keep trying to
   buy until we find a set we can afford, then stop there.  */

   sc_accessory_info *info;  /* Accessory information. */
   int count = sc_accessory_count(c->accessories, SC_ACCESSORY_LIMIT_ALL);

   /* Iterate through, beginning with the "best" shields. */
   for(--count; count >= 0; --count) {
      /* Make sure the damn thing is a shield. */
      info = sc_accessory_lookup(c->accessories, ordering[count], SC_ACCESSORY_LIMIT_ALL);
      if(SC_ACCESSORY_IS_SHIELD(info)) {
         /* Can we afford this set of shields? */
         if(sc_inventory_can_buy_accessory(p, info, *budget)) {
            /* buy some of this shielding. */
            sc_inventory_buy_accessory(p, info);
            *budget -= info->price;
            if(SC_AI_DEBUG_BUY) {
               printf("   AI_buy:   %s, %s bought %s.  $%d\n", sc_ai_name(p->aitype), p->name, info->name, *budget);
            }
            return(true);  /* That's all, for now */
         } /* Can we buy these shields? */
      } /* Is this accessory a shield? */
   } /* Iterate through the shields */

   return(false);

}
