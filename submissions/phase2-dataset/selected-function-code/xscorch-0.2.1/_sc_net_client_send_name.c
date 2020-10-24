/* _sc_net_client_send_name
   Send the name of this player to the server. This packet is usually sent
   out immediately after the ACCEPTED packet is received from the server,
   when the player connects to the game.  This code, fortunately, is very
   straightforward. Returns true on success.  */

   sc_packet packet;       /* Data packet to send */

   /* Construct a simple name packet */
   if(!sc_net_packet_init( &packet, 
                           SC_NET_CLI_PLAYER_NAME, 
                           SC_PLAYER_NAME_LENGTH )) return(false);
   memcpy(packet.data, cli->name, SC_PLAYER_NAME_LENGTH);

   /* Attempt to send the packet */
   sc_net_set_info("cli_send_name", "Sending our name to the server");
   if(!sc_net_send_packet_now(&cli->server, &packet)) return(false);
   sc_net_packet_release(&packet);
   return(true);

}
