static int add_new_host(struct host **list,
			const char *host, unsigned int weight,
			struct addrinfo *host_addr,
			unsigned int rr, unsigned int options)
{
	struct host *new;
	unsigned int prx;
	int addr_len;

	prx = get_proximity(host_addr->ai_addr);

	/*
	 * If we want the weight to be the determining factor
	 * when selecting a host, or we are using random selection,
	 * then all hosts must have the same proximity. However,
	 * if this is the local machine it should always be used
	 * since it is certainly available.
	 */
	if (prx != PROXIMITY_LOCAL &&
	   (options & (MOUNT_FLAG_USE_WEIGHT_ONLY |
		       MOUNT_FLAG_RANDOM_SELECT)))
		prx = PROXIMITY_SUBNET;

	/*
	 * If we tried to add an IPv6 address and we don't have IPv6
	 * support return success in the hope of getting an IPv4
	 * address later.
	 */
	if (prx == PROXIMITY_UNSUPPORTED)
		return 1;
	if (prx == PROXIMITY_ERROR)
		return 0;

	if (host_addr->ai_addr->sa_family == AF_INET)
		addr_len = INET_ADDRSTRLEN;
	else if (host_addr->ai_addr->sa_family == AF_INET6)
		addr_len = INET6_ADDRSTRLEN;
	else
		return 0;

	new = new_host(host, host_addr->ai_addr, addr_len, prx, weight, options);
	if (!new)
		return 0;

	if (!add_host(list, new)) {
		free_host(new);
		return 0;
	}
	new->rr = rr;

	return 1;
}