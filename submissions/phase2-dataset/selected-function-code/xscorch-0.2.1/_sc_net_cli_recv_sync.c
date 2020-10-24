static bool _sc_net_cli_recv_sync(sc_config *c, sc_client *cli, sc_packet *packet) {
/* _sc_net_cli_recv_sync

   See sc_net_client_sync() for an overview of the process. This is the tail
   end to a synchronization from the client to the current game event. On
   success, the server will respond to us when all players have reached that
   event, telling us we may now continue with simulation or turn. The server
   will also remind us what TYPE of sync event we were going through.  This
   will normally agree with the type of sync event we sent in to the server.

   The server will also send us a copy of its current random value.  This
   should agree with the next random value we expect to get -- if it does
   then we are okay. If it does not, then something has gone horribly wrong. 
   And I do mean horribly. If that happens, we will have to try to negotiate
   with the server to see what went wrong.

   Otherwise, we may now continue with the game. This function returns true
   if everything worked properly. */

   dword randval;          /* Random value sanity check */
   dword *p;               /* Pointer into packet data */
   bool missync = false;   /* True if we are out of sync */

   /* Check data size, etc. */
   if(!sc_net_check_size( packet, 
                          2 * sizeof(dword), 
                          "cli_recv_sync" )) return(false);

   /* Update flags, sync was successfully processed */
   SC_CONN_CLEAR_FLAGS(cli->server, SC_CONN_WAIT_SYNC);
   sc_net_set_info("cli_recv_sync", "Received sync authorization from server");

   /* Check that the sync message is what we expected */
   p = (dword *)packet->data;
   if(ntohl(*p++) != cli->server.syncarg) {
      sc_net_set_error("cli_recv_sync", "Sync for wrong event received");
      missync = true;
   }

   /* Check that random values are still in sync */
   randval = ntohl(*p++);
   if(randval != game_rand_peek()) {
      sc_net_set_error("cli_recv_sync", "Random values are OUT OF ALIGNMENT!!!");
      missync = true;
   }

   if(missync) _sc_net_cli_missync(c);
   return(true);

}
