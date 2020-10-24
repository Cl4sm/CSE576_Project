int
addnet(register const char *str)
{
	register char *cp;
	register int width;
	register u_int32_t n, m;
	register struct nets *np;
	char *cp2;
	char tstr[64];

	if (strlen(str) > sizeof(tstr) - 1)
		return(0);

	if (nets_size <= 0) {
		nets_size = 8;
		nets = malloc(nets_size * sizeof(*nets));
	} else if (nets_size <= nets_ind) {
		/* XXX debugging */
		nets_size <<= 1;
		nets = realloc(nets, nets_size * sizeof(*nets));
	}
	if (nets == NULL) {
		(void)fprintf(stderr, "%s: addnet: malloc/realloc: %s\n",
		    prog, strerror(errno));
		exit(1);
	}
	np = nets + nets_ind;

	width = 0;
	strcpy(tstr, str);
	cp = strchr(tstr, '/');
	if (cp != NULL) {
		*cp++ = '\0';
		width = strtol(cp, &cp2, 10);
		/* Trailing garbage */
		if (*cp2 != '\0')
			    return (0);
		if (width > 32)
			    return (0);
	}

	/* XXX hack */
	n = ntohl(inet_addr(tstr));
	while ((n & 0xff000000) == 0) {
		n <<= 8;
		if (n == 0)
			return (0);
	}
	n = htonl(n);

	if (width != 0) {
		m = ~0;
		m <<= 32 - width;
	} else if (IN_CLASSA(n))
		m = IN_CLASSA_NET;
	else if (IN_CLASSB(n))
		m = IN_CLASSB_NET;
	else if (IN_CLASSC(n))
		m = IN_CLASSC_NET;
	else if (IN_CLASSD(n))
		m = IN_CLASSD_NET;
	else
		return (0);
	m = htonl(m);

	np->net = n;
	np->netmask = m;
	++nets_ind;

	return (1);
}