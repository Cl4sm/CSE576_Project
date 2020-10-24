
   sc_player *pl;          /* Player data structure */
   dword *p;               /* Pointer into packet data */
   sdword x;               /* Miscellaneous variable X */
   sdword y;               /* Miscellaneous variable Y */
   bool b;                 /* Miscellaneous boolean B */
   int i;                  /* Iterator variable */

   /* Check that the packet is large enough */
   if(!sc_net_check_size(packet, (11 * c->numplayers + 1) * sizeof(dword), "cli_recv_player_state")) return(false);

   /* Check that the total player count is accurate */
   p = (dword *)packet->data;
   if(!sc_net_check_param(c->numplayers, ntohl(*p++), "cli_recv_player_state", "player count")) return(false);

   /* For each player ... */
   for(i = 0; i < c->numplayers; ++i) {
      pl = c->players[i];
      /* First field is authority flag */
      if(ntohl(*p++)) {
         /* Remote player claims authority on this player */
         if(pl->aitype != SC_AI_NETWORK) {
            sc_net_set_error("cli_recv_player_state", "Remote player authority on locally controlled player");
            return(false);
         }

         if(!sc_net_check_param(pl->index, ntohl(*p++), "cli_recv_player_state", "index")) return(false);
         x = ntohl(*p++);
         y = ntohl(*p++);
         if(x != pl->x || y != pl->y) {
            sc_net_set_error("cli_recv_player_state", "Discrepancy on remote player position");
            sc_player_set_position(c, pl, x, y);
         }
         x = ntohl(*p++);
         if(x != pl->fuel) {
            sc_net_set_error("cli_recv_player_state", "Discrepancy on remote player fuel level");
            pl->fuel = x;
         }
         x = ntohl(*p++);
         y = ntohl(*p++);
         if(x != pl->turret || y != pl->power) {
            sc_net_set_error("cli_recv_player_state", "Discrepancy on remote player turret/power");
            sc_player_set_turret(c, pl, x);
            sc_player_set_power(c, pl, y);
         }
         x = ntohl(*p++);
         b = ntohl(*p++);
         if(x != pl->life || b != pl->dead) {
            sc_net_set_error("cli_recv_player_state", "Discrepancy on remote player life");
            pl->life = x;
            pl->dead = b;
         }
         x = ntohl(*p++);
         if(x != pl->money) {
            sc_net_set_error("cli_recv_player_state", "Discrepancy on remote player money");
            pl->money = x;
         }
         x = ntohl(*p++);
         if(x != pl->selweapon->ident) {
            sc_net_set_error("cli_recv_player_state", "Discrepancy on remote player selected weapon");
            sc_player_set_weapon(c, pl, sc_weapon_lookup(c->weapons, x, SC_WEAPON_LIMIT_NONE));
         }
      } else {
         /* Player does not claim to be authoritative */
         if(!sc_net_check_param(pl->index, ntohl(*p++), "cli_recv_player_state", "index")) return(false);
         x = ntohl(*p++);
         y = ntohl(*p++);
         if(x != pl->x || y != pl->y) {
            sc_net_set_error("cli_recv_player_state", "Discrepancy on remote player position");
         }
         x = ntohl(*p++);
         if(x != pl->fuel) {
            sc_net_set_error("cli_recv_player_state", "Discrepancy on remote player fuel level");
         }
         x = ntohl(*p++);
         y = ntohl(*p++);
         if(x != pl->turret || y != pl->power) {
            sc_net_set_error("cli_recv_player_state", "Discrepancy on remote player turret/power");
         }
         x = ntohl(*p++);
         b = ntohl(*p++);
         if(x != pl->life || b != pl->dead) {
            sc_net_set_error("cli_recv_player_state", "Discrepancy on remote player life");
         }
         x = ntohl(*p++);
         if(x != pl->money) {
            sc_net_set_error("cli_recv_player_state", "Discrepancy on remote player money");
         }
         x = ntohl(*p++);
         if(x != pl->selweapon->ident) {
            sc_net_set_error("cli_recv_player_state", "Discrepancy on remote player selected weapon");
         }
      }
   }

   /* Okay, we're done with that mess... */
   sc_net_set_info("cli_recv_player_state", "Player state received");
   return(true);

}
