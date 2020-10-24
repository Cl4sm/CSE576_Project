bool sc_net_client_send_battery(const sc_config *c, sc_client *cli, int playerid) {
/* sc_net_client_send_battery
   Let the server know that we are activating a battery. This packet
   may be directly relayed to other clients. Returns true if the packet
   makes it out the door; from there it's anyone's game... */

   sc_packet packet;       /* Packet to be sent */

   /* Sanity checks */
   if(c == NULL || cli == NULL) return(false);
   if(c->players[playerid]->aitype == SC_AI_NETWORK) return(false);

   /* Initialise the packet */
   if(!sc_net_packet_init(&packet, SC_NET_BATTERY, sizeof(dword))) return(false);
   *(dword *)packet.data = htonl(playerid);

   /* Attempt to send the packet */
   sc_net_set_info("client_send_battery", "activating a battery");
   if(!sc_net_send_packet_now(&cli->server, &packet)) return(false);
   sc_net_packet_release(&packet);
   return(true);

}
