sc_server *sc_net_server_new(sc_config *c, int port) {

   addr socket_info;    /* Information about us */
   sc_server *srv;      /* Server object */
   int i;               /* Iterator */

   /* Construct a new server object and initialise it */   
   srv = (sc_server *)malloc(sizeof(sc_server));
   if(srv == NULL) {
      sc_net_set_error("server_new", "malloc failed");
      return(NULL);
   }
   srv->connections = 0;

   /* Open a new TCP socket */
   srv->linein = socket(PF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);
   if(srv->linein == -1) {
      sc_net_set_error("server_new, socket", strerror(errno));
      free(srv);
      return(NULL);
   }

   /* Setup socket information about us; attempt to bind to a port */
   memset((char *)&socket_info, '\0', sizeof(socket_info));
   socket_info.sin_family = AF_INET;
   socket_info.sin_port = htons(port);
   socket_info.sin_addr.s_addr = INADDR_ANY;
   if(bind(srv->linein, (struct sockaddr *)&socket_info, sizeof(socket_info)) < 0) {
      sc_net_set_error("server_new, bind", strerror(errno));
      free(srv);
      return(NULL);
   }

   /* setup server socket not to block */
   sc_net_set_nonblocking(srv->linein);

   /* Game state initalisation */
   c->numplayers = 0;
   for(i = 0; i < SC_MAX_PLAYERS; ++i) {
      c->players[i]->aitype = SC_AI_RANDOM;
   }

   /* Prepare to listen for incoming packets */
   if(listen(srv->linein, 5) < 0) {
      sc_net_set_error("server_new, listen", strerror(errno));
      free(srv);
      return(NULL);
   }

   /* Display informative message */
   sc_net_set_info("server_new", "Network server established");

   /* Return the new socket object */
   return(srv);

}
