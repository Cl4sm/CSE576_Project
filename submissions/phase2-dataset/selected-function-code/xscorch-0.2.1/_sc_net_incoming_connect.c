static void _sc_net_incoming_connect(sc_config *c, sc_server *srv, int socket, const addr *fromaddr, int fromaddrsize) {

   sc_connection *client;
   sc_player *p;

   /* Make sure socket is nonblocking */
   sc_net_set_nonblocking(socket);

   /* Check that we have space to accept this connection into */
   if(srv->connections >= SC_MAX_PLAYERS) {
      sc_net_shutdown(&socket);
      sc_net_set_error("connect_request", "Incoming connection rejected because server is full");
      return;
   }

   /* Check that a game is not in progress */
   if((c->game->state & SC_STATE_OPTIONS_FLAG) == 0) {
      sc_net_shutdown(&socket);
      sc_net_set_error("connect_request", "Incoming connection rejected because game is already running");
      return;
   }

   /* Register the connection with the TCP NET packet engine. */
   if(!tn_instantiate(&srv->clients[srv->connections].connection, socket)) {
      sc_net_shutdown(&socket);
      sc_net_set_error("connect_request", "Unable to register socket with packet engine");
      return;
   }

   /* Below we set up the connection so we can recieve a configuration packet from the client. */

   /* Add this player to the list of active players */
   client = &srv->clients[srv->connections++];
   client->socket = socket;
   memcpy(&client->address, fromaddr, fromaddrsize);
   client->flags = SC_CONN_SVR_IFLAGS;

   /* Setup player data */
   p = c->players[c->numplayers];
   sbprintf(p->name, sizeof(p->name), "Network Player %d", c->numplayers);
   p->aitype = SC_AI_NETWORK;
   p->tank = sc_tank_profile_lookup(c->tanks, 0);
   ++c->numplayers;

}
