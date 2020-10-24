static int get_supported_ver_and_cost(unsigned logopt, struct host *host,
				      unsigned int version, int port)
{
	unsigned int random_selection = host->options & MOUNT_FLAG_RANDOM_SELECT;
	unsigned int use_weight_only = host->options & MOUNT_FLAG_USE_WEIGHT_ONLY;
	socklen_t len = INET6_ADDRSTRLEN;
	char buf[len + 1];
	struct conn_info pm_info, rpc_info;
	int proto;
	unsigned int vers;
	struct timeval start, end;
	struct timezone tz;
	double taken = 0;
	time_t timeout = RPC_TIMEOUT;
	int status = 0;

	if (host->addr)
		debug(logopt, "called with host %s(%s) version 0x%x",
			host->name, get_addr_string(host->addr, buf, len),
			version);
	else
		debug(logopt, "called with host %s version 0x%x",
			host->name, version);

	memset(&pm_info, 0, sizeof(struct conn_info));
	memset(&rpc_info, 0, sizeof(struct conn_info));

	if (host->proximity == PROXIMITY_NET)
		timeout = RPC_TIMEOUT * 2;
	else if (host->proximity == PROXIMITY_OTHER)
		timeout = RPC_TIMEOUT * 8;

	rpc_info.host = host->name;
	rpc_info.addr = host->addr;
	rpc_info.addr_len = host->addr_len;
	rpc_info.program = NFS_PROGRAM;
	rpc_info.timeout.tv_sec = timeout;
	rpc_info.close_option = RPC_CLOSE_DEFAULT;
	rpc_info.client = NULL;

	/*
	 *  The version passed in is the version as defined in
	 *  include/replicated.h.  However, the version we want to send
	 *  off to the rpc calls should match the program version of NFS.
	 *  So, we do the conversion here.
	 */
	if (version & UDP_SELECTED_MASK) {
		proto = IPPROTO_UDP;
		version >>= 8;
	} else
		proto = IPPROTO_TCP;

	switch (version) {
	case NFS2_SUPPORTED:
		vers = NFS2_VERSION;
		break;
	case NFS3_SUPPORTED:
		vers = NFS3_VERSION;
		break;
	case NFS4_SUPPORTED:
		vers = NFS4_VERSION;
		break;
	default:
		crit(logopt, "called with invalid version: 0x%x\n", version);
		return 0;
	}

	rpc_info.proto = proto;

	if (port > 0)
		rpc_info.port = port;
	else if (vers & NFS4_VERSION && port < 0)
		rpc_info.port = NFS_PORT;
	else {
		struct pmap parms;
		int ret = rpc_portmap_getclient(&pm_info,
				host->name, host->addr, host->addr_len,
				proto, RPC_CLOSE_DEFAULT);
		if (ret)
			return 0;

		memset(&parms, 0, sizeof(struct pmap));
		parms.pm_prog = NFS_PROGRAM;
		parms.pm_prot = rpc_info.proto;
		parms.pm_vers = vers;
		ret = rpc_portmap_getport(&pm_info, &parms, &rpc_info.port);
		if (ret < 0)
			goto done;
	}

	if (rpc_info.proto == IPPROTO_UDP)
		status = rpc_udp_getclient(&rpc_info, NFS_PROGRAM, vers);
	else
		status = rpc_tcp_getclient(&rpc_info, NFS_PROGRAM, vers);
	if (status == -EHOSTUNREACH)
		goto done;
	else if (!status) {
		gettimeofday(&start, &tz);
		status = rpc_ping_proto(&rpc_info);
		gettimeofday(&end, &tz);
		if (status > 0) {
			if (random_selection) {
				/* Random value between 0 and 1 */
				taken = ((float) random())/((float) RAND_MAX+1);
				debug(logopt, "random selection time %f", taken);
			} else {
				taken = elapsed(start, end);
				debug(logopt, "rpc ping time %f", taken);
			}
		}
	}
done:
	if (rpc_info.proto == IPPROTO_UDP) {
		rpc_destroy_udp_client(&rpc_info);
		rpc_destroy_udp_client(&pm_info);
	} else {
		rpc_destroy_tcp_client(&rpc_info);
		rpc_destroy_tcp_client(&pm_info);
	}

	if (status) {
		/* Response time to 7 significant places as integral type. */
		if (use_weight_only)
			host->cost = 1;
		else
			host->cost = (unsigned long) (taken * 1000000);

		/* Allow for user bias */
		if (host->weight)
			host->cost *= (host->weight + 1);

		debug(logopt, "cost %ld weight %d", host->cost, host->weight);

		return 1;
	}

	return 0;
}