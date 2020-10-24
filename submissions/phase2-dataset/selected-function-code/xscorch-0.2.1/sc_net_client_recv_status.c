bool sc_net_client_recv_status(const sc_config *c, sc_client *cli, sc_packet *packet) {
/* sc_net_client_recv_status
   Update the client status data. */

   sc_net_status *status;
   dword *p;
   int playerid;
   
   /* Check the packet size */
   if(!sc_net_check_size( packet,
                          6 * sizeof(dword),
                          "client_recv_status" )) return(false);

   /* Get the player ID */
   p = (dword *)packet->data;
   playerid = ntohl(*p++);
   if(playerid < 0 || playerid >= c->numplayers) {
      sc_net_set_error("client_recv_status", "Invalid playerid received");
      return(false);
   }
   status = &cli->status[playerid];
   
   /* Load in the new data */
   status->cli_flags   = ntohl(*p++);
   status->cli_syncarg = ntohl(*p++);
   status->srv_flags   = ntohl(*p++);
   status->srv_syncarg = ntohl(*p++);
   
   /* load in the player's address */
   memcpy(&status->address, p, sizeof(addr));
   
   /* Return with success */
   sc_net_set_info("client_recv_status", "Received player status from server");
   return(true);
   
}
