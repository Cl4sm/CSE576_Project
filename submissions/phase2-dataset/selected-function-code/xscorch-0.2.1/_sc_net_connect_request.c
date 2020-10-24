
   char versionstr[SC_NET_BUFFER_SIZE];
   char infomsg[SC_NET_BUFFER_SIZE];
   sc_connection *client;
   sc_packet reply;
   dword version;

   client = &srv->clients[connid];

   /* Make sure this actually looks like a connect request */
   if(incoming->msg_type != SC_NET_CLI_CONNECT) {
      sc_net_set_error("connect_request", "Incoming packet from unknown source, not a connect request");
      return;
   }

   /* From this point, we assume a connection from an xscorch client.
      Any failures from this point should be sent to them. */

   /* Check the version number data (should be in flags) */
   version = ntohl(*(dword *)incoming->data);
   if(version < SC_NET_MIN_VERSION) {
      sc_net_send_message(client, SC_NET_SVR_REJECTED, "Client version is too old");
      sc_net_set_error("connect_request", "Incoming packet had protocol version that was too ancient");
      return;
   }
   if(version > SC_NET_VERSION) {
      sc_net_send_message(client, SC_NET_SVR_REJECTED, "Client version is too new");
      sc_net_set_error("connect_request", "Incoming packet had protocol version that was too recent");
      return;
   }

   /* notify client they are on */
   sc_net_version_info(versionstr, sizeof(versionstr));
   if(!sc_net_packet_init(&reply, SC_NET_SVR_ACCEPTED, strlenn(versionstr) + sizeof(dword))) return;
   *(dword *)reply.data = htonl(SC_NET_VERSION);
   memcpy(reply.data + sizeof(dword), versionstr, strlenn(versionstr));
   sc_net_send_packet_now(client, &reply);

   /* Informative */
   memcpy(versionstr, incoming->data + sizeof(dword), incoming->data_size - sizeof(dword));
   versionstr[incoming->data_size - sizeof(dword)] = '\0';
   sbprintf(infomsg, sizeof(infomsg), "Client accepted, version \"%s\"", versionstr);
   sc_net_set_info("connect_request", infomsg);

   /* Send the game configuration */
   sc_net_svr_send_config(c, srv);

   /* Update server setup */
   sc_net_packet_release(&reply);
   SC_CONN_CLEAR_FLAGS(*client, SC_CONN_NEED_ACCEPT);

}
