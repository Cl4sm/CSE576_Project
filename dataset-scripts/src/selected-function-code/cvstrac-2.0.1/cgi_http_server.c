void cgi_http_server(int iPort){
  int listener;                /* The server socket */
  int connection;              /* A socket for each individual connection */
  fd_set readfds;              /* Set of file descriptors for select() */
  size_t lenaddr;              /* Length of the inaddr structure */
  int child;                   /* PID of the child process */
  int nchildren = 0;           /* Number of child processes */
  struct timeval delay;        /* How long to wait inside select() */
  struct sockaddr_in inaddr;   /* The socket address */
  int opt = 1;                 /* setsockopt flag */

  memset(&inaddr, 0, sizeof(inaddr));
  inaddr.sin_family = AF_INET;
  inaddr.sin_addr.s_addr = INADDR_ANY;
  inaddr.sin_port = htons(iPort);
  listener = socket(AF_INET, SOCK_STREAM, 0);
  if( listener<0 ){
    fprintf(stderr,"Can't create a socket\n");
    exit(1);
  }

  /* if we can't terminate nicely, at least allow the socket to be reused */
  setsockopt(listener,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

  if( bind(listener, (struct sockaddr*)&inaddr, sizeof(inaddr))<0 ){
    fprintf(stderr,"Can't bind to port %d\n", iPort);
    exit(1);
  }
  listen(listener,10);
  while( 1 ){
    if( nchildren>MAX_PARALLEL ){
      /* Slow down if connections are arriving too fast */
      sleep( nchildren-MAX_PARALLEL );
    }
    delay.tv_sec = 60;
    delay.tv_usec = 0;
    FD_ZERO(&readfds);
    FD_SET( listener, &readfds);
    if( select( listener+1, &readfds, 0, 0, &delay) ){
      lenaddr = sizeof(inaddr);
      connection = accept(listener, (struct sockaddr*)&inaddr, &lenaddr);
      if( connection>=0 ){
        child = fork();
        if( child!=0 ){
          if( child>0 ) nchildren++;
          close(connection);
        }else{
          close(0);
          dup(connection);
          close(1);
          dup(connection);
          close(2);
          dup(connection);
          close(connection);
          return;
        }
      }
    }
    /* Bury dead children */
    while( waitpid(0, 0, WNOHANG)>0 ){
      nchildren--;
    }
  }
  /* NOT REACHED */  
  exit(1);
}