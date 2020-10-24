static bool _sc_net_cli_recv_flags(sc_config *c, sc_packet *packet) {
/* _sc_net_cli_recv_flags
   Similar to the orders packet, except we are receiving a player's
   set of command flags.  Again, returns true on success. */

   sc_player *pl;          /* Player structure */
   sdword playerid;        /* Player ID to shield */
   dword *p;               /* Packet data pointer */

   /* Check the packet size */
   if(!sc_net_check_size( packet, 
                          3 * sizeof(dword), 
                          "cli_recv_flags" )) return(false);

   /* Get the player ID */
   p = (dword *)packet->data;
   playerid = ntohl(*p++);
   if(playerid < 0 || playerid >= c->numplayers) {
      sc_net_set_error("cli_recv_flags", "Invalid playerid received");
      return(false);
   }
   pl = c->players[playerid];

   /* Get command flags */
   if(pl->aitype == SC_AI_NETWORK) {
      /* Setting params for a network player */
      pl->contacttriggers = ntohl(*p++);
      pl->ac_state        = ntohl(*p++);
   } else {
      /* Verify configuration */
      sc_net_check_param(pl->contacttriggers, ntohl(*p++), "cli_recv_flags", "contact triggers");
      sc_net_check_param(pl->ac_state,        ntohl(*p++), "cli_recv_flags", "accessory state flags");
   } /* Local or remote? */

   /* Return with success */
   sc_net_set_info("cli_recv_flags", "Received command flags from server");
   return(true);

}
