/* sc_player_advance_weapon
   Jump <delta> weapons forward (negative: jump back by so many weapons). */

   sc_weapon_info *info;

   /* Sanity checks */
   if(c == NULL || p == NULL) return;

   info = p->selweapon;
   if(info == NULL) {
      fprintf(stderr, "warning: player %i has selected invalid weapon, aborting search\n", p->index);
      return;
   }

   /* Cycle through the weapons */
   do {
      if(delta > 0) info = sc_weapon_next(c->weapons, info, SC_WEAPON_LIMIT_ALL | SC_WEAPON_SCAN_FORWARD);
      else          info = sc_weapon_next(c->weapons, info, SC_WEAPON_LIMIT_ALL | SC_WEAPON_SCAN_REVERSE);
      if(info == NULL) {
         fprintf(stderr, "warning: player %i has no weapons (at all!), aborting search\n", p->index);
         break;
      }
      sc_player_set_weapon(c, p, info);
   } while(info->inventories[p->index] <= 0);

   /* Update state */
   #if USE_NETWORK
      sc_net_client_send_orders(c, c->client, p->index);
   #endif
   sc_status_update(c->window, p);

}
