void sc_player_advance_shield(const sc_config *c, sc_player *p, int flags) {
/* sc_player_advance_shield
   Jump to the next available shield.  */

   sc_accessory_info *info;
   bool sawone = false;

   /* Sanity checks */
   if(c == NULL || p == NULL) return;

   info = p->selshield;
   if(info == NULL) {
      fprintf(stderr, "warning: player %i has selected invalid shield, aborting search\n", p->index);
      return;
   }
   
   /* Always advance, unless CHECK_CUR is set */
   if(!(flags & SC_PLAYER_SHIELD_CHECK_CUR)) {
      /* If we're not on a shield, try to find one. */
      if(!SC_ACCESSORY_IS_SHIELD(info)) {
         info = sc_shield_find_best(c, p);
      } else {
         info = sc_accessory_next(c->accessories, info, SC_ACCESSORY_LIMIT_ALL);
      }
   }

   /* Cycle through the shields */
   while(info != NULL && (!sawone || info->ident != p->selshield->ident) && 
         (!SC_ACCESSORY_IS_SHIELD(info) || info->inventories[p->index] <= 0)) {
      info = sc_accessory_next(c->accessories, info, SC_ACCESSORY_LIMIT_ALL);
      sawone = true;
   }
   
   /* Were we successful? */
   if(info == NULL || !SC_ACCESSORY_IS_SHIELD(info) || info->inventories[p->index] <= 0) {
      /* No shields left; fall back to the default */
      info = SC_ACCESSORY_DEFAULT(c->accessories);
      if(info == NULL) {
         fprintf(stderr, "warning: no default accessory found! aborting...\n");
         return;
      }
   }

   /* Found a shield which we have in stock! */
   sc_player_set_shield(c, p, info);
   #if USE_NETWORK
      sc_net_client_send_orders(c, c->client, p->index);
   #endif
   sc_status_update(c->window, p);

}
