bool sc_net_client_send_inventory(const sc_config *c, sc_client *cli, int playerid) {

   const sc_accessory_info *ai;  /* Accessory info */
   const sc_weapon_info *wi;  /* Weapon info */
   const sc_player *pl;    /* Player data */
   sc_packet packet;       /* Packet to be sent */
   dword *p;               /* Pointer into packet data */
   int acount;             /* Accessory count (total) */
   int wcount;             /* Weapon count (total) */
   int size;               /* Expected packet size */
   int i;                  /* Iterator variable */

   /* Sanity checks */   
   if(c == NULL || cli == NULL) return(false);
   pl = c->players[playerid];
   if(pl->aitype == SC_AI_NETWORK) return(false);

   /* Make sure packet is large enough to hold additional data.
      Packets for weapons and accessories look like this:
      [ integer item_id ], [ integer item_count ]  (repeat) */
   acount = sc_accessory_count(c->accessories, SC_ACCESSORY_LIMIT_NONE);
   wcount = sc_weapon_count(c->weapons, SC_WEAPON_LIMIT_NONE);
   size = (8 + 2 * (wcount + acount)) * sizeof(dword);

   /* Initialise header data */
   if(!sc_net_packet_init(&packet, SC_NET_INVENTORY, size)) return(false);
   p = (dword *)packet.data;
   *p++ = htonl(playerid);
   *p++ = htonl(wcount);
   *p++ = htonl(acount);

   /* Initialise weapons block */
   *p++ = htonl(SC_PACKET_INVENTORY_WEAPONS);
   wi = sc_weapon_first(c->weapons, SC_WEAPON_LIMIT_NONE);
   for(i = 0; i < wcount; ++i) {
      *p++ = htonl(wi->ident);
      *p++ = htonl(wi->inventories[playerid]);
      wi = sc_weapon_next(c->weapons, wi, SC_WEAPON_LIMIT_NONE);
   }

   /* Initialise accessories block */
   *p++ = htonl(SC_PACKET_INVENTORY_ACCESSORIES);
   ai = sc_accessory_first(c->accessories, SC_ACCESSORY_LIMIT_NONE);
   for(i = 0; i < acount; ++i) {
      *p++ = htonl(ai->ident);
      *p++ = htonl(ai->inventories[playerid]);
      ai = sc_accessory_next(c->accessories, ai, SC_ACCESSORY_LIMIT_NONE);
   }

   /* Intiialise miscellaneous block */
   *p++ = htonl(SC_PACKET_INVENTORY_MISC);
   *p++ = htonl(pl->money);
   *p++ = htonl(pl->oldmoney);

   /* Attempt to send the packet */
   sc_net_set_info("client_send_inventory", "sending our inventory");
   if(!sc_net_send_packet_now(&cli->server, &packet)) return(false);
   sc_net_packet_release(&packet);
   return(true);

}
