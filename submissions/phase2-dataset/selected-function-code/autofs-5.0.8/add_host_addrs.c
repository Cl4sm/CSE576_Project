static int add_host_addrs(struct host **list, const char *host,
			  unsigned int weight, unsigned int options)
{
	struct addrinfo hints, *ni, *this;
	char *n_ptr;
	char *name = n_ptr = strdup(host);
	int len;
	char buf[MAX_ERR_BUF];
	int rr = 0, rr4 = 0, rr6 = 0;
	int ret;

	if (!name) {
		char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
		error(LOGOPT_ANY, "strdup: %s", estr);
		error(LOGOPT_ANY, "failed to add host %s", host);
		return 0;
	}
	len = strlen(name);

	if (name[0] == '[' && name[--len] == ']') {
		name[len] = '\0';
		name++;
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_NUMERICHOST;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	ret = getaddrinfo(name, NULL, &hints, &ni);
	if (ret)
		goto try_name;

	this = ni;
	while (this) {
		ret = add_new_host(list, host, weight, this, 0, options);
		if (!ret)
			break;
		this = this->ai_next;
	}
	freeaddrinfo(ni);
	goto done;

try_name:
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_ADDRCONFIG;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	ret = getaddrinfo(name, NULL, &hints, &ni);
	if (ret) {
		error(LOGOPT_ANY, "hostname lookup failed: %s",
		      gai_strerror(ret));
		free(name);
		return 0;
	}

	this = ni;
	while (this->ai_next) {
		if (this->ai_family == AF_INET) {
			struct sockaddr_in *addr = (struct sockaddr_in *) this->ai_addr;
			if (addr->sin_addr.s_addr != INADDR_LOOPBACK)
				rr4++;
		} else if (this->ai_family == AF_INET6) {
			struct sockaddr_in6 *addr = (struct sockaddr_in6 *) this->ai_addr;
			if (!IN6_IS_ADDR_LOOPBACK(addr->sin6_addr.s6_addr32))
				rr6++;
		}
		this = this->ai_next;
	}
	if (rr4 > 1 || rr6 > 1)
		rr++;
	this = ni;
	while (this) {
		ret = add_new_host(list, host, weight, this, rr, options);
		if (!ret)
			break;
		this = this->ai_next;
	}
	freeaddrinfo(ni);
done:
	free(n_ptr);
	return ret;
}