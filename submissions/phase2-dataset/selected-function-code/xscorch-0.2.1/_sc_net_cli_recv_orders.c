static bool _sc_net_cli_recv_orders(sc_config *c, sc_packet *packet) {
/* _sc_net_cli_recv_orders
   Receiving orders from another player in the game. We are given our
   client data structure, and their packet (as relayed from the server).
   We are generally expected to comply with their orders for their tank
   but we will not accept server orders for OUR tank -- we will only 
   verify the orders sent to us for our tank. Returns true on success */

   sc_player *pl;          /* Current player structure */
   dword *p;               /* Pointer into packet data */
   dword x;                /* X variable */
   dword y;                /* Y variable */
   int i;                  /* Iterator */

   /* Check size and sequence numbers */
   if(!sc_net_check_size( packet, 
                          (1 + 7 * c->numplayers) * sizeof(dword), 
                          "cli_recv_orders" )) return(false);

   /* Check that the total player count is accurate */
   p = (dword *)packet->data;
   if(!sc_net_check_param( c->numplayers, 
                           ntohl(*p++), 
                           "cli_recv_orders", 
                           "player count" )) return(false);

   /* Update each player, in turn */
   for(i = 0; i < c->numplayers; ++i) {
      pl = c->players[i];
      if(!sc_net_check_param( i, ntohl(*p++), 
                              "cli_recv_orders", 
                              "marker" )) return(false);
      if(pl->aitype == SC_AI_NETWORK) {
         /* Setting params for a network player */
         sc_player_set_turret(c, pl, ntohl(*p++));
         sc_player_set_power (c, pl, ntohl(*p++));
         sc_player_set_weapon(c, pl, sc_weapon_lookup(c->weapons, ntohl(*p++), SC_WEAPON_LIMIT_NONE));
         sc_player_set_shield(c, pl, sc_accessory_lookup(c->accessories, ntohl(*p++), SC_ACCESSORY_LIMIT_NONE));
         x = ntohl(*p++);
         y = ntohl(*p++);
         sc_player_set_position(c, pl, x, y);
      } else {
         /* Verifying orders for a local player -- note, it is possible we 
            are receiving an old (but valid) orders packet here -- which is
            possible given network latencies if the player is rapidly enter
            new orders. Therefore no action is taken currently if one of
            these assertions fails. */
         sc_net_check_param(pl->turret,           ntohl(*p++), "cli_recv_orders", "turret");
         sc_net_check_param(pl->power,            ntohl(*p++), "cli_recv_orders", "power");
         sc_net_check_param(pl->selweapon->ident, ntohl(*p++), "cli_recv_orders", "weapon");
         sc_net_check_param(pl->selshield->ident, ntohl(*p++), "cli_recv_orders", "shield");
         sc_net_check_param(pl->x,                ntohl(*p++), "cli_recv_orders", "player x");
         sc_net_check_param(pl->y,                ntohl(*p++), "cli_recv_orders", "player y");
      } /* Modifying network or verifying params? */
   } /* Loop through all players */

   /* Return with success */
   sc_net_set_info("cli_recv_orders", "Received orders from server");
   return(true);

}
