bool sc_net_client_send_orders(const sc_config *c, sc_client *cli, int playerid) {
/* sc_net_client_send_orders
   Send our orders to the server. The server will hopefully accept
   our orders and send a broadcast packet to all players containing
   the composite of player orders. I hope. At any rate, if we are
   able to send the packet, then we'll return true. This packet is
   not directly relayed to other clients. */

   const sc_player *pl;    /* Player data structure */
   sc_packet packet;       /* Packet to send */
   dword *p;               /* Pointer into data */

   /* Sanity checks */
   if(c == NULL || cli == NULL) return(false);
   pl = c->players[playerid];
   if(!pl || pl->aitype == SC_AI_NETWORK) return(false);

   /* Initialise the packet */
   if(!sc_net_packet_init( &packet, 
                           SC_NET_CLI_ORDERS, 
                           7 * sizeof(dword) )) return(false);
   p = (dword *)packet.data;
   *p++ = htonl(playerid);
   *p++ = htonl(pl->turret);
   *p++ = htonl(pl->power);
   *p++ = htonl(pl->selweapon->ident);
   *p++ = htonl(pl->selshield->ident);
   *p++ = htonl(pl->x);
   *p++ = htonl(pl->y);

   /* Attempt to send the packet */
   sc_net_set_info("client_send_orders", "sending our game orders");
   if(!sc_net_send_packet_now(&cli->server, &packet)) return(false);
   sc_net_packet_release(&packet);
   return(true);

}
