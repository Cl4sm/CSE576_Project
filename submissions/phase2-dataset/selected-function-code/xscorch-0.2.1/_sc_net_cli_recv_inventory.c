
   sc_accessory_info *ai;  /* A temp pointer into info structs */
   sc_weapon_info *wi;     /* A temp pointer into info structs */
   sc_player *pl;          /* Player structure */
   sdword playerid;        /* Player ident */
   dword *p;               /* Pointer into packet data */
   int acount;             /* Accessory count (total) */
   int wcount;             /* Weapon count (total) */
   int size;               /* Section size parametre */
   int i;                  /* Iterator */

   /* Make sure size is large enough to contain the player ID 
      and individual sizes for each section (3 dwords) */
   if(!sc_net_check_size(packet, 3 * sizeof(dword), "cli_recv_inventory")) return(false);

   /* Get the player ID and section sizes */
   p = (dword *)packet->data;
   playerid = ntohl(*p++);
   if(playerid < 0 || playerid >= c->numplayers) {
      sc_net_set_error("cli_recv_inventory", "Invalid playerid received");
      return(false);
   }
   pl = c->players[playerid];
   acount = sc_accessory_count(c->accessories, SC_ACCESSORY_LIMIT_NONE);
   wcount = sc_weapon_count(c->weapons, SC_WEAPON_LIMIT_NONE);
   if(!sc_net_check_param(ntohl(*p++), wcount, "cli_recv_inventory", "weapon count")) return(false);
   if(!sc_net_check_param(ntohl(*p++), acount, "cli_recv_inventory", "accessory count")) return(false);

   /* Make sure packet is large enough to hold additional data.
      Packets for weapons and accessories look like this:
      [ integer item_id ], [ integer item_count ]  (repeat) */
   size = (8 + 2 * (wcount + acount)) * sizeof(dword);
   if(!sc_net_check_size(packet, size, "cli_recv_inventory")) return(false);

   /* Process weapons in packet */
   if(!sc_net_check_param(ntohl(*p++), SC_PACKET_INVENTORY_WEAPONS, "cli_recv_inventory", "weapon marker")) return(false);
   size = wcount;
   if(pl->aitype == SC_AI_NETWORK) {
      /* Setting weapons for a network player */
      for(i = 0; i < size; ++i) {
         wi = sc_weapon_lookup(c->weapons, ntohl(*p++), SC_WEAPON_LIMIT_NONE);
         if(wi == NULL) {
            sc_net_set_error("cli_recv_player_state", "setting inventory of nonexistant weapon");
            return(false);
         } else {
            wi->inventories[playerid] = ntohl(*p++);
         }
      } /* Loop through weapons */
   } else {
      /* Verifying weapons for a local player */
      for(i = 0; i < size; ++i) {
         wi = sc_weapon_lookup(c->weapons, ntohl(*p++), SC_WEAPON_LIMIT_NONE);
         if(wi == NULL) {
            sc_net_set_error("cli_recv_player_state", "checking inventory of nonexistant weapon");
            return(false);
         } else {
            if(!sc_net_check_param(ntohl(*p++), wi->inventories[playerid], "cli_recv_inventory", "weapon"))
               return(false);
         }
      } /* Loop through weapons */
   } /* Network or local player? */

   /* Process accessories in packet */
   if(!sc_net_check_param(ntohl(*p++), SC_PACKET_INVENTORY_ACCESSORIES, "cli_recv_inventory", "accessory marker")) return(false);
   size = acount;
   if(pl->aitype == SC_AI_NETWORK) {
      /* Setting accessories for a network player */
      for(i = 0; i < size; ++i) {
         ai = sc_accessory_lookup(c->accessories, ntohl(*p++), SC_ACCESSORY_LIMIT_NONE);
         if(ai == NULL) {
            sc_net_set_error("cli_recv_player_state", "setting inventory of nonexistant accessory");
            return(false);
         } else {
            ai->inventories[playerid] = ntohl(*p++);
         }
      } /* Loop through accessories */
   } else {
      /* Verifying accessories for a local player */
      for(i = 0; i < size; ++i) {
         ai = sc_accessory_lookup(c->accessories, ntohl(*p++), SC_ACCESSORY_LIMIT_NONE);
         if(ai == NULL) {
            sc_net_set_error("cli_recv_player_state", "checking inventory of nonexistant accessory");
            return(false);
         } else {
            if(!sc_net_check_param(ntohl(*p++), ai->inventories[playerid], "cli_recv_inventory", "accessory"))
               return(false);
         }
      } /* Loop through accessories */
   } /* Network or local player? */

   /* Process miscellaneous section */
   if(!sc_net_check_param(ntohl(*p++), SC_PACKET_INVENTORY_MISC, "cli_recv_inventory", "misc marker")) return(false);
   if(pl->aitype == SC_AI_NETWORK) {
      pl->money = ntohl(*p++);
      pl->oldmoney = ntohl(*p++);
   } else {
      sc_net_check_param(ntohl(*p++), pl->money,    "cli_recv_inventory", "misc: money");
      sc_net_check_param(ntohl(*p++), pl->oldmoney, "cli_recv_inventory", "misc: oldmoney");
   }

   /* All went well */
   sc_net_set_info("cli_recv_inventory", "Received inventory from server");
   return(true);

}
