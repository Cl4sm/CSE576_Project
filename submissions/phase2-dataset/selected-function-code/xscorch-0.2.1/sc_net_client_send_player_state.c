
   const sc_player *pl;    /* Player data */
   sc_packet packet;       /* Packet to send */
   dword *p;               /* Pointer to packet data */
   int size;               /* Expected packet size */
   int i;                  /* Iterator variable */

   /* Sanity checks */
   if(c == NULL || cli == NULL) return(false);

   /* Initialise packet data */
   size = (1 + 11 * c->numplayers) * sizeof(dword);
   if(!sc_net_packet_init(&packet, SC_NET_PLAYER_STATE, size)) return(false);
   p = (dword *)packet.data;
   *p++ = htonl(c->numplayers);
   for(i = 0; i < c->numplayers; ++i) {
      /* First field is authority flag */
      pl = c->players[i];
      *p++ = htonl(pl == SC_AI_HUMAN);
      *p++ = htonl(pl->index);
      *p++ = htonl(pl->x);
      *p++ = htonl(pl->y);
      *p++ = htonl(pl->fuel);
      *p++ = htonl(pl->turret);
      *p++ = htonl(pl->power);
      *p++ = htonl(pl->life);
      *p++ = htonl(pl->dead);
      *p++ = htonl(pl->money);
      *p++ = htonl(pl->selweapon->ident);
   }

   /* Attempt to send */
   sc_net_set_info("client_send_player_state", "sending our player state");
   if(!sc_net_send_packet_now(&cli->server, &packet)) return(false);
   sc_net_packet_release(&packet);

   /* Send a status update */
   sc_net_client_update_status(cli);
   return(true);

}
