int
authorize_address(sin)
     struct sockaddr	*sin;
{
#ifdef DOUNIX
  if (doflags&DOUNIX) {
    struct sockaddr_un 	*srv = (struct sockaddr_un*)sin;
    
    if (foreignport != NULL && 0!=strcmp(foreignport, srv->sun_path)) {
      if (doflags&DOVERBOSE) {
	  fprintf(stderr, "%s: Refusing UNIX connection from port %s\n",
		  progname, srv->sun_path);
      }
      return 0;
    }
  } else
#endif
    {
      int is_ipv6 = (sin->sa_family == AF_INET6);
      struct sockaddr_in *srv	= (struct sockaddr_in *) sin;
      struct sockaddr_in6 *srv6	= (struct sockaddr_in6 *) sin;
      int	i;

      if (doflags&DOVERBOSE) {
	  fprintf(stderr, "%s: Incoming connection from ", progname);
	  printhost(stderr, sin);
	  fprintf(stderr, " port %d\n",
			  get_port((struct sockaddr_storage *) sin));
      }

      if (foreignhost) {
	  for (i=0; i<foreignCOUNT; i++) {
	      if (!is_ipv6 && foreignHOST[i]->ss_family==AF_INET &&
			0==memcmp(&srv->sin_addr,
				&((struct sockaddr_in *)foreignHOST[i])->sin_addr,
				sizeof(struct in_addr)))
		  break;
	      if (is_ipv6 && foreignHOST[i]->ss_family==AF_INET6 &&
		    IN6_ARE_ADDR_EQUAL(&srv6->sin6_addr,
			&((struct sockaddr_in6 *)foreignHOST[i])->sin6_addr))
		  break;
	  }
	  if (i>=foreignCOUNT) {
	      if (doflags&DOVERBOSE) {
		  fprintf(stderr, "%s: Refusing connection from host ", progname);
		  printhost(stderr, sin);
		  fprintf(stderr, ".\n");
	      }
	      return 0;
	  }
    }
    
    if (foreignport!=NULL &&
	! (foreignPORT == get_port((struct sockaddr_storage *) sin)) ) {
      if (doflags&DOVERBOSE) {
	fprintf(stderr, "%s: Refusing connection from port %d.\n", progname,
		get_port((struct sockaddr_storage *) sin));
      }
      return 0;
    }
  }
  
  return 1;
}