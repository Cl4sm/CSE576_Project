/* _sc_net_sync_all

   I think you want to read snetclient.c (search for "_sync") before looking
   through this function, since it's not entirely obvious what all is going
   on here.  This code is a bit dense, but that's because we have to figure
   out _what_ the sync event is, then we have to figure out if any clients
   are not on the same event.

   Note we will send out the expected next random value for the server, so
   the client may check and see if they are actually on the correct value. 
   If the client is off, it is up to them to report the discrepancy back to
   the server and try to negotiate the error with us.  Otherwise, their
   orders will be run with respect to OUR map, and they will probably be
   very unhappy...  

   This returns true if a sync occurred (all clients were ready) and false
   if we are still waiting on a client to sync with us. */

   sc_packet packet;    /* New outgoing packet data */
   dword *p;            /* Random pointer into data */
   dword type = 0;      /* Sync event type - 0 == don't know */
   int rnd  = 0;        /* Next random value or bogus == wrong event */
   int i;               /* Iterate through client connections */

   for(i = 0; i < srv->connections; ++i) {
      /* Make sure every client is ready to sync */
      if(!SC_CONN_IS_SYNC(srv->clients[i])) return(false);

      /* If server, we can find out what the sync type is */
      if((SC_CONN_GET_ARG(srv->clients[i]) & SC_CONN_SYNC_SERV) != 0) {
         /* The server knows what type of event we're syncing */
         type = SC_CONN_GET_ARG(srv->clients[i]) & (~SC_CONN_SYNC_SERV);
      }
   }

   for(i = 0; i < srv->connections; ++i) {
      /* Sanity check to make sure everyone is syncing to the same event */
      rnd = game_rand_peek();
      if((SC_CONN_GET_ARG(srv->clients[i]) & (~SC_CONN_SYNC_SERV)) != type) {
         /* This client is not syncing to the right event.  We send back an
            invalid random value so the client will report ``random values
            out of alignment'' and hopefully call back to us to try to
            straighten things out. This is a hack of sorts, we'll handle
            this more gracefully later. */
         sc_net_set_error("svr_relay_orders", "Player was waiting for invalid sync");
         rnd = -1;
      }

      /* Prepare a packet to send out to the client */      
      if(sc_net_packet_init(&packet, SC_NET_SVR_SYNC_RESP, 2 * sizeof(dword))) {
         p = (dword *)packet.data;
         *p++ = htonl(type);
         *p++ = htonl(rnd);
         sc_net_send_packet_now(&srv->clients[i], &packet);
         SC_CONN_CLEAR_FLAGS(srv->clients[i], SC_CONN_WAIT_SYNC);
         sc_net_packet_release(&packet);
      }
   }

   /* IF we made it here, then something went right... */
   sc_net_set_info("svr_sync_all", "All clients sync; advancing state");
   return(true);

}
