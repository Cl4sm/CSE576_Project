bool sc_net_client_update_status(sc_client *cli) {
/* sc_net_client_update_status
   Send current information about our connection to the server. */

   sc_packet packet;
   dword *p;
   int size;
   
   /* Sanity checks */
   if(cli == NULL) return(false);
   
   /* Initialise packet data */
   size = 5 * sizeof(dword) + sizeof(addr);
   if(!sc_net_packet_init(&packet, SC_NET_CLIENT_STATUS, size)) return(false);
   p = (dword *)packet.data;   
   
   /* Write the player ID and client flags */
   *p++ = htonl(0);
   *p++ = htonl(cli->server.flags);
   *p++ = htonl(cli->server.syncarg);
   
   /* Zero the server data and address field */
   memset(p, '\0', 2 * sizeof(dword) + sizeof(addr));
   
   /* Attempt to send the packet */
   sc_net_set_info("client_update_status", "updating the client status");
   if(!sc_net_send_packet_now(&cli->server, &packet)) return(false);
   sc_net_packet_release(&packet);
   return(true);
   
}
