
   char versionstr[SC_NET_BUFFER_SIZE];  /* version info */
   sc_packet packet;    /* Transmitted data packet */
   host *host_info;     /* Remote host information */
   sc_client *cli;      /* Client object */

   /* Get my version data */
   sc_net_version_info(versionstr, sizeof(versionstr));

   /* Construct a new client object and initialise it */   
   cli = (sc_client *)malloc(sizeof(sc_client));
   if(cli == NULL) {
      sc_net_set_error("client_new", "malloc failed");
      return(NULL);
   }
   strcopyb(cli->name, name, sizeof(cli->name));

   /* Open a new TCP socket */
   cli->server.socket = socket(PF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);
   if(cli->server.socket == -1) {
      sc_net_set_error("client_new, socket", strerror(errno));
      free(cli);
      return(NULL);
   }

   /* Get information about the remote host */
   host_info = gethostbyname(hostname);
   if(host_info == NULL) {
      sc_net_set_error("client_new, gethostbyname", "Cannot resolve hostname.");
      free(cli);
      return(NULL);
   }

   /* setup client destination address */
   memset((char *)&cli->server.address, '\0', sizeof(addr));
   memcpy((char *)&cli->server.address.sin_addr, host_info->HOSTENT_H_ADDR, host_info->h_length);
   cli->server.address.sin_family = host_info->h_addrtype;
   cli->server.address.sin_port = htons(port);

   /* Below we initiate a connection request to the server */

   /* Initialise server-client data */
   cli->server.flags = SC_CONN_CLI_IFLAGS;
   cli->server.syncarg = 0;

   /* Attempt a connect request. We want this call to block */
   if(connect(cli->server.socket, (const struct sockaddr *)&cli->server.address, sizeof(addr)) < 0) {
      sc_net_set_error("client_new, connect", strerror(errno));
      free(cli);
      return(NULL);
   }

   /* setup client socket not to block from now on */
   sc_net_set_nonblocking(cli->server.socket);

   /* Register the new connection with the TCP NET packet engine. */
   if(!tn_instantiate(&cli->server.connection, cli->server.socket)) {
      sc_net_set_error("client_new, TCP NET", strerror(errno));
      free(cli);
      return(NULL);
   }

   /* Initiate a connection request to the server */
   sc_net_packet_init(&packet, SC_NET_CLI_CONNECT, strlenn(versionstr) + sizeof(dword));
   *(dword *)packet.data = htonl(SC_NET_VERSION);
   memcpy(packet.data + sizeof(dword), versionstr, strlenn(versionstr));

   /* Attempt to send the packet */
   if(!sc_net_send_packet_now(&cli->server, &packet)) {
      free(cli);
      return(NULL);
   }
   sc_net_packet_release(&packet);
   
   /* Initialise the status data */
   sc_net_status_init(cli);

   /* Return the client connection */
   return(cli);

}
