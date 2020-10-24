bool sc_net_server_run(sc_config *c, sc_server *srv) {

   sc_connection *client;
   addr address;
   socklen_t addrsize;
   int connid;
   int socket;
   bool tryagain;

   /* Sanity checks */
   if(c == NULL || srv == NULL) return(false);

   /* Look for new incoming connections */
   do {
      addrsize = sizeof(addr);
      socket = accept(srv->linein, (struct sockaddr *)&address, &addrsize);
      if(socket != -1) _sc_net_incoming_connect(c, srv, socket, &address, addrsize);
   } while(socket != -1);

   /* Check for incoming packets from existing clients */
   for(connid = 0; connid < srv->connections; ++connid) {
      srv->current = connid;
      client = &srv->clients[connid];
      tryagain = !SC_CONN_IS_DEAD(*client);
      while(tryagain) {
         tryagain = sc_net_recv_packet(client, c, (void *)srv, &sc_net_server_handle_packet);
         if(SC_CONN_IS_DEAD(*client)) tryagain = false;
      }
   }

   return(true);

}
