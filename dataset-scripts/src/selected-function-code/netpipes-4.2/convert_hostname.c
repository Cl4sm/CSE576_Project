struct sockaddr_storage ** /* addr_array */
convert_hostname(name, count_ret)
    char	*name;
    int		*count_ret;
{
  //struct hostent	*hp;
  struct addrinfo hints, *ai, *aiptr;
  struct sockaddr_storage **rval;
  int rc, count = 0;

  memset(&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_ADDRCONFIG | AI_CANONNAME;
  hints.ai_family = AF_UNSPEC;

  if ( (rc = getaddrinfo(name, NULL, &hints, &aiptr)) == 0 ) {
    /* Count the relevant entries. */
    for (ai = aiptr; ai; ai = ai->ai_next)
      if ( ai->ai_family == AF_INET6 || ai->ai_family == AF_INET )
	++count;

    if (count) {
      int i = 0;

      *count_ret = count;
      rval = (struct sockaddr_storage **) malloc(sizeof(*rval) * (count+1));

      /* Recover the found address structures. */
      for (ai = aiptr; ai; ai = ai->ai_next) {
	if ( ai->ai_family != AF_INET6 && ai->ai_family != AF_INET )
	  continue;

	rval[i] = (struct sockaddr_storage *) malloc(sizeof(**rval));
	memcpy(rval[i], ai->ai_addr, ai->ai_addrlen);
	++i;
      }
      rval[count] = NULL;
    }
    freeaddrinfo(aiptr);

    if (count)
      return rval;
  }
#if 0
  hp = gethostbyname(name);
  if (hp != NULL) {
    int	i;
    if (hp->h_length != sizeof(struct in_addr)) {
	fprintf(stderr, "%s: Funky: (hp->h_length = %d) != (sizeof(struct in_addr) = %ld)\n", progname, hp->h_length, (long) sizeof(struct in_addr));
    }
    for (i = 0; hp->h_addr_list[i]; i++)
	{ }
    *count_ret = i;
    rval = (struct sockaddr_storage **)malloc(sizeof(*rval) * (i+1));
    for (i=0; i<*count_ret; i++) {
	rval[i] = (struct sockaddr_storage *)malloc(hp->h_length);
	memcpy((char*)rval[i], hp->h_addr_list[i], hp->h_length);
    }
    rval[*count_ret] = 0;
    return rval;
  } else {
#endif

#if 0
#ifndef HAVE_INET_ATON
      int	count, len;
      unsigned int	a1,a2,a3,a4;
#endif
      rval = (struct sockaddr_storage **)malloc(2*sizeof(*rval));
      rval[0] = (struct sockaddr_storage *)
			malloc(sizeof(struct sockaddr_storage));
#ifdef HAVE_INET_ATON
      if (0==inet_aton(name, rval[0])) {
	  *count_ret = 0;
	  free(rval[0]);
	  free(rval);
	  return 0;
      }
#else
      count = sscanf(name,"%i.%i.%i.%i%n", &a1, &a2, &a3, &a4, &len);
      if (4!=count || 0!=name[len] )
	  return 0;
      rval[0]->s_addr = (((((a1 << 8) | a2) << 8) | a3) << 8) | a4;
#endif
      *count_ret = 1;
      rval[1] = 0;

      return rval;
  }
#else
  return NULL;
#endif
}