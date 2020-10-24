/* sc_net_client_send_flags
   Let the server know what command flags we have set, e.g. contact
   trigger status.  Returns true if the packet makes it out the door;
   from there it's anyone's game... */

   sc_packet packet;       /* Packet to be sent */
   dword *p;               /* Packet data pointer */

   /* Sanity checks */
   if(c == NULL || cli == NULL) return(false);
   if(c->players[playerid]->aitype == SC_AI_NETWORK) return(false);

   /* Initialise the packet */
   if(!sc_net_packet_init(&packet, SC_NET_PLFLAGS, 3 * sizeof(dword))) return(false);
   p = (dword *)packet.data;
   *p++ = htonl(playerid);
   *p++ = htonl(c->players[playerid]->contacttriggers);
   *p++ = htonl(c->players[playerid]->ac_state);

   /* Attempt to send the packet */
   sc_net_set_info("client_send_flags", "sending our command flags");
   if(!sc_net_send_packet_now(&cli->server, &packet)) return(false);
   sc_net_packet_release(&packet);
   return(true);

}
