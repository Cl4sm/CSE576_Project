bool sc_net_client_sync(sc_client *cli, dword flag, bool isserver) {
/* sc_net_client_sync

   Okay, this is how this works. The sstate.c code calls this function
   whenever it wants to synchronise with the server (end of inventory, end
   of turn, or end of round confirmation). When this is called, the client
   expects to block until server has confirmed they may continue.  The flag
   indicates what type of network sync event the client is waiting for.

   isserver is relevant because the server is the ``authority'' on sync
   requests, i.e. the server is always keeping proper game. So if we are the
   server, we set isserver to true, which munges a special flag into the
   packet.

   When the server is damn well ready, they will send us a sync response; if
   it is at all useful, it will also include the server's next expected 
   random value.  This allows us to easily check to see if we are in sync
   with the server; if we share the same next random value then we are 
   probably okay, whereas if they differ, we know something has gone
   horribly wrong.

   Note that if we were syncing on one type of event, and the server was
   syncing on a different event, something went wrong -- they will send
   us a bogus random value and from there we will have to play the same
   ``what went wrong?'' game that we must play if random values normally
   go out of alignment.  */

   sc_packet packet;       /* Packet to send */
   dword *p;               /* Pointer to dat */

   /* Make sure we have a client socket */
   if(cli == NULL) return(false);

   /* Set state flags; we'll be waiting now */
   SC_CONN_SET_FLAGS(cli->server, SC_CONN_WAIT_SYNC);
   SC_CONN_SET_ARG(cli->server, flag);

   /* If server, add server sync flag */
   if(isserver) flag |= SC_CONN_SYNC_SERV;
   else flag &= ~SC_CONN_SYNC_SERV;

   /* Setup the packet */
   if(!sc_net_packet_init(&packet, SC_NET_CLI_SYNC_RQST, 2 * sizeof(dword))) return(false);
   p = (dword *)packet.data;
   *p++ = htonl(flag);              /* Send the sync flag to use */
   *p++ = htonl(game_rand_peek());  /* Send our next random value */

   /* Attempt to send the packet */
   sc_net_set_info("client_sync", "syncing with server on game event");
   if(!sc_net_send_packet_now(&cli->server, &packet)) return(false);
   sc_net_packet_release(&packet);

   /* Send a status update */
   sc_net_client_update_status(cli);
   return(true);

}
