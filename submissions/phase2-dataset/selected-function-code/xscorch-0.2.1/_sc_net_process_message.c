static bool _sc_net_process_message(sc_config *c, sc_client *cli, sc_packet *packet) {

   char buffer[SC_NET_BUFFER_SIZE];   /* Temporary output buffer */
   char infomsg[SC_NET_BUFFER_SIZE];  /* Text to be displayed to user */

   switch(packet->msg_type) {
   case SC_NET_SVR_ACCEPTED:
      SC_CONN_CLEAR_FLAGS(cli->server, SC_CONN_NEED_ACCEPT);
      _sc_net_client_send_name(cli);
      memcpy(buffer, packet->data + sizeof(dword), packet->data_size - sizeof(dword));
      buffer[packet->data_size - sizeof(dword)] = '\0';
      sbprintf(infomsg, sizeof(infomsg), "We have connected to server, version \"%s\"", buffer);
      sc_net_set_info("process_message", infomsg);
      break;
   case SC_NET_SVR_REJECTED:
      SC_CONN_SET_FLAGS(cli->server, SC_CONN_REJECTED);
      sbprintf(infomsg, sizeof(infomsg), "We were rejected from server:  %s", packet->data);
      sc_net_set_error("process_message", infomsg);
      sc_window_message(c->window, "Client Rejected", sc_net_get_error());
      break;
   case SC_NET_SVR_QUIT:
      SC_CONN_SET_FLAGS(cli->server, SC_CONN_QUIT);
      sc_net_set_info("process_message", "client: server sent QUIT");
      break;
   case SC_NET_CHAT:
      memcpy(buffer, packet->data, min(packet->data_size, SC_NET_BUFFER_SIZE - 1));
      buffer[min(packet->data_size, SC_NET_BUFFER_SIZE - 1)] = '\0';
      sc_net_set_info("chat", buffer);
#if 0 /* TEMP - Chat is broken in GTK 2.  As in, this does not compile. */
      sc_chat_window_update(c->window, buffer);
#endif
      break;
   case SC_NET_SVR_CONFIG_DATA:
      sc_net_cli_recv_config(c, cli, packet);
      break;
   case SC_NET_SVR_PLAYER_DATA:
      sc_net_cli_recv_players(c, cli, packet);
      break;
   case SC_NET_SVR_ORDERS:
      _sc_net_cli_recv_orders(c, packet);
      break;
   case SC_NET_INVENTORY:
      _sc_net_cli_recv_inventory(c, packet);
      break;
   case SC_NET_SHIELDS:
      _sc_net_cli_recv_shields(c, packet);
      break;
   case SC_NET_BATTERY:
      _sc_net_cli_recv_battery(c, packet);
      break;
   case SC_NET_PLFLAGS:
      _sc_net_cli_recv_flags(c, packet);
      break;
   case SC_NET_PLAYER_STATE:
      _sc_net_cli_recv_player_state(c, packet);
      break;
   case SC_NET_SVR_SYNC_RESP:
      _sc_net_cli_recv_sync(c, cli, packet);
      break;
   case SC_NET_CLIENT_STATUS:
      sc_net_client_recv_status(c, cli, packet);
      break;
   default:
      sbprintf(infomsg, sizeof(infomsg), "invalid packet type (%08x) received by client", packet->msg_type);
      sc_net_set_error("process_message", infomsg);
      break;
   }

   return(true);

}
