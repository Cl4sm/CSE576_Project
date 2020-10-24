int setup_socket(name, backlog, reuseaddr)
char *name;
int	backlog;
int	reuseaddr;
/* This procedure creates a socket and handles retries on the inet domain.
   Sockets seem to "stick" on my system (SunOS [43].x) */
{
  int	sock, zero = 0;
  int	family;

#ifdef DOUNIX
  if (doflags&DOUNIX)
    family = AF_UNIX;
  else
#endif
    family = AF_INET6;

  sock = socket(family, SOCK_STREAM,
#ifdef DOUNIX
		(doflags&DOUNIX)?0:
#endif
		IPPROTO_TCP);
  /* I need a real value for the protocol eventually.  IPPROTO_TCP sounds
     like a good value, but what about AF_UNIX sockets?  It seems to have
     worked so far... */

  if (sock <0) {
      perror("opening stream socket");
      exit(EXITCODE_CONNECTION);
  }

  setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, &zero, sizeof(zero));

  if (!bindlocal(sock, name, localhost,
		 family,
		 reuseaddr)) {
      fprintf(stderr,"%s: error binding stream socket %s (%s)\n",
	      progname,name,strerror(errno));
      exit(EXITCODE_CONNECTION);
    }

  /* We used to ask for NOFILE (max number of open files) for the size
     of the connect queue.  Linux didn't like it (NOFILE=256) so we
     hardcoded a smaller value. */
  if (listen(sock, (backlog>0 ? backlog : 5)) < 0) {
    perror("listen failed");
  }

  return(sock);
}