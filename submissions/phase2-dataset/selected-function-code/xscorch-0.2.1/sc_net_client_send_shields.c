/* sc_net_client_send_shields
   Let the server know that we are activating our shields. This packet
   may be directly relayed to other clients. Returns true if the packet
   makes it out the door; from there it's anyone's game... */

   sc_packet packet;       /* Packet to be sent */
   sc_player *pl;
   dword *p;

   /* Sanity checks */
   if(c == NULL || cli == NULL) return(false);
   pl = c->players[playerid];
   if(pl == NULL || pl->shield == NULL || pl->aitype == SC_AI_NETWORK) return(false);

   /* Initialise the packet */
   if(!sc_net_packet_init(&packet, SC_NET_SHIELDS, 2 * sizeof(dword))) return(false);
   p = (dword *)packet.data;
   *p++ = htonl(playerid);
   *p++ = htonl(pl->shield->info->ident);

   /* Attempt to send the packet */
   sc_net_set_info("client_send_shields", "sending our shield status");
   if(!sc_net_send_packet_now(&cli->server, &packet)) return(false);
   sc_net_packet_release(&packet);
   return(true);

}
