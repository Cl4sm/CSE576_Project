/* _sc_net_cli_recv_shields
   Similar to the orders packet, except we are receiving a player request
   to activate their shields. Again, returns true on success. */

   sc_accessory_info *info;/* Shield requested for activation */
   sc_player *pl;          /* Player structure */
   sdword playerid;        /* Player ID to shield */
   dword *p;               /* counter */

   /* Check the packet size */
   if(!sc_net_check_size( packet, 
                          2 * sizeof(dword), 
                          "cli_recv_shields" )) return(false);
   p = (dword *)packet->data;

   /* Get the player ID */
   playerid = ntohl(*p++);
   if(playerid < 0 || playerid >= c->numplayers) {
      sc_net_set_error("cli_recv_shields", "Invalid playerid received");
      return(false);
   }
   pl = c->players[playerid];

   /* Test that the shield is in inventory */
   info = sc_accessory_lookup(c->accessories, ntohl(*p++), SC_ACCESSORY_LIMIT_NONE);
   if(info == NULL || info->inventories[playerid] <= 0) {
      sc_net_set_error("cli_recv_shields", "Shield to activate not in inventory");
      return(false);
   }

   /* Activate shields if needed */
   if(pl->aitype == SC_AI_NETWORK) {
      /* Setting params for a network player */
      sc_player_activate_shield(c, pl);
   }

   /* Return with success */
   sc_net_set_info("cli_recv_shields", "Received shield status from server");
   return(true);

}
