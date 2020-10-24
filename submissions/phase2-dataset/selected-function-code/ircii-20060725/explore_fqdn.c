static int
explore_fqdn(pai, hostname, servname, res)
	const struct addrinfo *pai;
	const char *hostname;
	const char *servname;
	struct addrinfo **res;
{
	struct hostent *hp;
	int h_error;
	int af;
	char **aplist = NULL, *apbuf = NULL;
	char *ap;
	struct addrinfo sentinel, *cur;
	int i;
#ifndef USE_GETIPNODEBY
	int naddrs;
#endif
	const struct afd *afd;
	int error = 0;
#if 0
	struct addrinfo pai4;
#ifdef INET6
	struct addrinfo pai6;
#endif
#endif

	*res = NULL;
	sentinel.ai_next = NULL;
	cur = &sentinel;

	/*
	 * If AI_ADDRCONFIG is specified, check if we are expected to
	 * return the address family or not.
	 * assumes PF_UNSPEC = PF_INET + PF_INET6.
	 *
	 * NOTE: PF_UNSPEC case is for future use.
	 */
	if ((pai->ai_flags & AI_ADDRCONFIG) != 0) {
		switch (pai->ai_family) {
#if 0
		case PF_UNSPEC:
			pai4 = pai6 = *pai;
			pai4.ai_family = PF_INET;
#ifndef INET6
			if (!addrconfig(&pai4))
				return 0;
#else
			pai6.ai_family = PF_INET6;
			if (!addrconfig(&pai4)) {
				if (!addrconfig(&pai6))
					return 0;
				pai = &pai6;
			} else {
				if (!addrconfig(&pai6))
					pai = &pai4;
				else
					; /* as is */
			}
#endif
			break;
#endif
		default:
			if (!addrconfig(pai))
				return 0;
			break;
		}
	}

	/*
	 * if the servname does not match socktype/protocol, ignore it.
	 */
	if (get_portmatch(pai, servname) != 0)
		return 0;

	afd = find_afd(pai->ai_family);
	if (afd == NULL)
		return 0;

#ifdef USE_GETIPNODEBY
	hp = getipnodebyname(hostname, pai->ai_family,
	    pai->ai_flags & AI_ADDRCONFIG, &h_error);
#else
#if HAVE_GETHOSTBYNAME2
	hp = gethostbyname2(hostname, pai->ai_family);
#else
	if (pai->ai_family != AF_INET)
		return 0;
	hp = gethostbyname(hostname);
#endif /*HAVE_GETHOSTBYNAME2*/
#if HAVE_H_ERRNO
	h_error = h_errno;
#else
	h_error = EINVAL;
#endif
#endif /*USE_GETIPNODEBY*/

	if (hp == NULL) {
		switch (h_error) {
		case HOST_NOT_FOUND:
		case NO_DATA:
			error = EAI_NODATA;
			break;
		case TRY_AGAIN:
			error = EAI_AGAIN;
			break;
		case NO_RECOVERY:
#ifdef NETDB_INTERNAL
		case NETDB_INTERNAL:
#endif
		default:
			error = EAI_FAIL;
			break;
		}
	} else if ((hp->h_name == NULL) || (hp->h_name[0] == 0)
			|| (hp->h_addr_list[0] == NULL)) {
#ifdef USE_GETIPNODEBY
		freehostent(hp);
#endif
		hp = NULL;
		error = EAI_FAIL;
	}

	if (hp == NULL)
		goto free;

#ifdef USE_GETIPNODEBY
	aplist = hp->h_addr_list;
#else
	/*
	 * hp will be overwritten if we use gethostbyname2().
	 * always deep copy for simplification.
	 */
	for (naddrs = 0; hp->h_addr_list[naddrs] != NULL; naddrs++)
		;
	naddrs++;
	aplist = (char **)malloc(sizeof(aplist[0]) * naddrs);
	apbuf = (char *)malloc((size_t)hp->h_length * naddrs);
	if (aplist == NULL || apbuf == NULL) {
		error = EAI_MEMORY;
		goto free;
	}
	memset(aplist, 0, sizeof(aplist[0]) * naddrs);
	for (i = 0; i < naddrs; i++) {
		if (hp->h_addr_list[i] == NULL) {
			aplist[i] = NULL;
			continue;
		}
		memcpy(&apbuf[i * hp->h_length], hp->h_addr_list[i],
			(size_t)hp->h_length);
		aplist[i] = &apbuf[i * hp->h_length];
	}
#endif

	for (i = 0; aplist[i] != NULL; i++) {
		af = hp->h_addrtype;
		ap = aplist[i];
#ifdef INET6
		if (af == AF_INET6
		 && IN6_IS_ADDR_V4MAPPED((struct in6_addr *)ap)) {
			af = AF_INET;
			ap = ap + sizeof(struct in6_addr)
				- sizeof(struct in_addr);
		}
#endif

		if (af != pai->ai_family)
			continue;

		GET_AI(cur->ai_next, afd, ap);
		GET_PORT(cur->ai_next, servname);
		if ((pai->ai_flags & AI_CANONNAME) != 0) {
			/*
			 * RFC2553 says that ai_canonname will be set only for
			 * the first element.  we do it for all the elements,
			 * just for convenience.
			 */
			GET_CANONNAME(cur->ai_next, hp->h_name);
		}

		while (cur && cur->ai_next)
			cur = cur->ai_next;
	}

	*res = sentinel.ai_next;
	return 0;

free:
#ifdef USE_GETIPNODEBY
	if (hp)
		freehostent(hp);
#endif
	if (aplist)
		free(aplist);
	if (apbuf)
		free(apbuf);
	if (sentinel.ai_next)
		freeaddrinfo(sentinel.ai_next);
	return error;
}
