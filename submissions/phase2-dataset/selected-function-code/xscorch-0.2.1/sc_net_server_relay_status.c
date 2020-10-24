bool sc_net_server_relay_status(sc_server *srv, sc_packet *incoming, int connid) {
/* sc_net_server_relay_status
   Relay a status packet from a client to all players currently connected. */

   sc_connection *conn;
   sc_packet packet;
   dword *p;
   int i;

   /* Fill in the blanks in this data structure */
   p = (dword *)incoming->data;
   conn = &srv->clients[connid];
   *p++ = htonl(connid);
    p  += 2;
   *p++ = htonl(conn->flags);
   *p++ = htonl(conn->syncarg);
   
   /* Fill in the client address */
   memcpy(p, &conn->address, sizeof(addr));
   
   /* Relay the modified packet to all connected clients */
   for(i = 0; i < srv->connections; ++i) {
      if(!SC_CONN_IS_DEAD(srv->clients[i])) {
         memcpy(&packet, incoming, sizeof(sc_packet));
         sc_net_send_packet_now(&srv->clients[i], &packet);
      }
   }
   
   /* Return success */
   return(true);

}
