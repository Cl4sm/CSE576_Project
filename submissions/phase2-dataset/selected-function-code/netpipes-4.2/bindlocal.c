int
bindlocal(fd, name, addrname, domain, reuseaddr)
     int	fd, domain;
     char	*name, *addrname;
     int	reuseaddr;
{
  struct sockaddr	*laddr;
  int	addrlen;
  int	countdown;
  int	rval, port;
  
  if (reuseaddr && (domain == AF_INET || domain == AF_INET6)) {
#ifdef SO_REUSEADDR
      /* The below fix is based on articles that came from comp.sys.hp.hpux
	 with the problem of having FIN_WAIT_2 statuses on sockets.  But even
	 on Solaris the sockets with TIME_WAIT block the bind() call, so I
	 thought it would be a good idea to try the setsockopt() call.
	 1998/01/18 Thomas Endo <tendo@netcom.com> */
      int	enable = 1;
      if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&enable,
		     sizeof(enable)) < 0)
	  {
	      fprintf(stderr,"%s: error in setsockopt (%s)\n",progname,
		      strerror(errno));
	      exit(EXITCODE_CONNECTION);
	  }
#else
      fprintf(stderr, "%s: Warning. SO_REUSEADDR is not available\n",
	      progname);
#endif
  }

  if (domain==AF_INET || domain==AF_INET6)
    {
      static struct sockaddr_storage	srv;
      static int	initted=0;

      laddr = (struct sockaddr*)&srv;
      addrlen = sizeof(srv);
      memset(&srv, 0, sizeof(srv));

      if (!initted) {
	srv.ss_family = domain;

	if (addrname) {
	    int	count, j = 0;
	    struct sockaddr_storage **addresses;
	    addresses = convert_hostname(addrname, &count);
	    if (addresses == 0) {
		fprintf(stderr, "%s: Unable to convert %s to an internet address\n", progname, addrname);
		errno=0;
		return 0;
	    }
	    while (addresses[j] && addresses[j]->ss_family != AF_INET
			    && addresses[j]->ss_family != AF_INET6)
	      ++j;
	    if (addresses[j])
	      memcpy(&srv, addresses[j], sizeof(srv));
	}
	
	port = name_to_inet_port(name);
      
	if (port==0)
	  {
	    fprintf(stderr, "%s: port %s unknown\n", progname, name);
	    errno = 0;
	    return 0;
	  }

	set_port(&srv, port);
      }
      initted = 1;		/* bindlocal is only called once in
				   each netpipes program */
    }
#ifndef NOUNIXSOCKETS
  else if (domain == AF_UNIX)
    {
      static struct sockaddr_un	srv;
      laddr = (struct sockaddr*)&srv;
      addrlen = sizeof(srv);
      
      srv.sun_family = AF_UNIX;
      strncpy(srv.sun_path, name, sizeof(srv.sun_path));
      srv.sun_path[sizeof(srv.sun_path) -1] = 0; /* NUL terminate that string*/
    }
#endif
  else
    {
      fprintf(stderr, "%s: unknown address family %d in bindlocal()\n",
	      progname, domain);
      exit(EXITCODE_ARGS);
    }
  
  countdown= ((domain!=AF_INET && domain!=AF_INET6)|| reuseaddr)?1:10;
  do {
    rval = bind(fd, laddr, addrlen);
    if (rval != 0)
      {
	if (errno==EADDRINUSE && --countdown>0)
	  {
	    fprintf(stderr,"%s: Address %s in use, sleeping 10.\n",
		    progname, name);
	    sleep (10);
	    fprintf(stderr,"%s: Trying again . . .\n", progname);
	  }
	else
	  return 0;
      }
  } while (rval);

  return 1;
}