static bool _sc_net_cli_recv_battery(sc_config *c, sc_packet *packet) {
/* _sc_net_cli_recv_battery
   Similar to the orders packet, except we are receiving a player request
   to activate one of their batteries. Again, returns true on success. */

   sc_player *pl;          /* Player structure */
   sdword playerid;        /* Player ID to shield */

   /* Check the packet size */
   if(!sc_net_check_size( packet, 
                          sizeof(dword), 
                          "cli_recv_batterys" )) return(false);

   /* Get the player ID */
   playerid = ntohl(*(dword *)packet->data);
   if(playerid < 0 || playerid >= c->numplayers) {
      sc_net_set_error("cli_recv_battery", "Invalid playerid received");
      return(false);
   }
   pl = c->players[playerid];

   /* Activate batteries if needed */
   if(pl->aitype == SC_AI_NETWORK) {
      /* Setting params for a network player */
      sc_player_activate_battery(c, pl);
   }

   /* Return with success */
   sc_net_set_info("cli_recv_battery", "Received battery command from server");
   return(true);

}
