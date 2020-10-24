static char *getdnsdomainname(unsigned int logopt)
{
	struct addrinfo hints, *ni;
	char name[MAXDNAME + 1];
	char buf[MAX_ERR_BUF];
	char *dnsdomain = NULL;
	char *ptr;
	int ret;

	memset(name, 0, sizeof(name));
	if (gethostname(name, MAXDNAME) == -1) {
		char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
		error(logopt, "gethostname: %s", estr);
		return NULL;
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_CANONNAME;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	ret = getaddrinfo(name, NULL, &hints, &ni);
	if (ret) {
		error(logopt, "hostname lookup failed: %s", gai_strerror(ret));
		return NULL;
	}

	ptr = ni->ai_canonname;
	while (*ptr && *ptr != '.')
		ptr++;

	if (*++ptr)
		dnsdomain = strdup(ptr);

	freeaddrinfo(ni);

	return dnsdomain;
}