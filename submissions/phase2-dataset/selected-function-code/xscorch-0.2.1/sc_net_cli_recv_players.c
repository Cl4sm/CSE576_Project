bool sc_net_cli_recv_players(sc_config *c, sc_client *cli, sc_packet *packet) {

   ubyte *p;            /* Pointer to packet data */
   int i;               /* Iterator variable */

   /* Sanity checks */
   if(c == NULL || cli == NULL || packet == NULL) return(false);
   if(!sc_net_check_size(packet, SC_PACKET_PLAYER_SIZE * SC_MAX_PLAYERS, "cli_recv_players")) return(false);

   /* Iterate through player data */
   for(i = 0; i < SC_MAX_PLAYERS; ++i) {
      p = packet->data + SC_PACKET_PLAYER_SIZE * i;
      memcpy(c->players[i]->name, p, SC_PLAYER_NAME_LENGTH);
      c->players[i]->aitype = ntohl(*(dword *)(p + SC_PLAYER_NAME_LENGTH));
   }

   /* We are done */
   sc_net_set_info("cli_recv_players", "player structure received");
   SC_CONN_CLEAR_FLAGS(cli->server, SC_CONN_NEED_PLAYERS);

   return(true);

}
