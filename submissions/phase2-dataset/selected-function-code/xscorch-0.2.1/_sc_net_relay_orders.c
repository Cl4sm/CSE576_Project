static bool _sc_net_relay_orders(const sc_config *c, sc_server *srv, const sc_packet *incoming, int connid) {
/* _sc_net_relay_orders
   Push orders from one player to all the other players. */

   const sc_player *pl;
   sdword playerid;
   dword turret;
   dword power;
   dword weapon;
   dword shield;
   dword x;
   dword y;
   sc_packet packet;
   dword *p;
   int size;
   int i;

   if(!sc_net_check_size(incoming, 7 * sizeof(dword), "relay_orders")) return(false);

   p = (dword *)incoming->data;
   playerid = ntohl(*p++);
   turret   = ntohl(*p++);
   power    = ntohl(*p++);
   weapon   = ntohl(*p++);
   shield   = ntohl(*p++);
   x        = ntohl(*p++);
   y        = ntohl(*p++);
   if(playerid < 0 || playerid >= c->numplayers) {
      sc_net_set_error("svr_relay_orders", "PlayerID received was invalid");
      return(false);
   }

   size = (1 + 7 * c->numplayers) * sizeof(dword);
   if(!sc_net_packet_init(&packet, SC_NET_SVR_ORDERS, size)) return(false);
   p = (dword *)packet.data;
   *p++ = htonl(c->numplayers);
   for(i = 0; i < c->numplayers; ++i) {
      *p++ = htonl(i);
      if(i == playerid) {
         *p++ = htonl(turret);
         *p++ = htonl(power);
         *p++ = htonl(weapon);
         *p++ = htonl(shield);
         *p++ = htonl(x);
         *p++ = htonl(y);
      } else {
         pl = c->players[i];
         *p++ = htonl(pl->turret);
         *p++ = htonl(pl->power);
         *p++ = htonl(pl->selweapon->ident);
         *p++ = htonl(pl->selshield->ident);
         *p++ = htonl(pl->x);
         *p++ = htonl(pl->y);
      }
   }

   for(i = 0; i < srv->connections; ++i) {
      if(i != connid && !SC_CONN_IS_DEAD(srv->clients[i])) {
         sc_net_send_packet_now(&srv->clients[i], &packet);
      }
   }

   sc_net_packet_release(&packet);
   return(true);

}
