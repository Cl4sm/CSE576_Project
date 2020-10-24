static unsigned int get_nfs_info(unsigned logopt, struct host *host,
			 struct conn_info *pm_info, struct conn_info *rpc_info,
			 int proto, unsigned int version, int port)
{
	unsigned int random_selection = host->options & MOUNT_FLAG_RANDOM_SELECT;
	unsigned int use_weight_only = host->options & MOUNT_FLAG_USE_WEIGHT_ONLY;
	socklen_t len = INET6_ADDRSTRLEN;
	char buf[len + 1];
	struct pmap parms;
	struct timeval start, end;
	struct timezone tz;
	unsigned int supported = 0;
	double taken = 0;
	int status, count = 0;

	if (host->addr)
		debug(logopt, "called with host %s(%s) proto %d version 0x%x",
		      host->name, get_addr_string(host->addr, buf, len),
		      proto, version);
	else
		debug(logopt,
		      "called for host %s proto %d version 0x%x",
		      host->name, proto, version);

	rpc_info->proto = proto;
	if (port < 0)
		rpc_info->port = NFS_PORT;
	else if (port > 0)
		rpc_info->port = port;

	memset(&parms, 0, sizeof(struct pmap));
	parms.pm_prog = NFS_PROGRAM;
	parms.pm_prot = proto;

	if (!(version & NFS4_REQUESTED))
		goto v3_ver;

	if (!port) {
		status = rpc_portmap_getclient(pm_info,
				host->name, host->addr, host->addr_len,
				proto, RPC_CLOSE_DEFAULT);
		if (status == -EHOSTUNREACH) {
			supported = status;
			goto done_ver;
		} else if (status)
			goto done_ver;
		parms.pm_vers = NFS4_VERSION;
		status = rpc_portmap_getport(pm_info, &parms, &rpc_info->port);
		if (status == -EHOSTUNREACH || status == -ETIMEDOUT) {
			supported = status;
			goto done_ver;
		} else if (status < 0) {
			if (version & NFS_VERS_MASK)
				goto v3_ver; /* MOUNT_NFS_DEFAULT_PROTOCOL=4 */
			else
				goto done_ver;
		}
	}

	if (rpc_info->proto == IPPROTO_UDP)
		status = rpc_udp_getclient(rpc_info, NFS_PROGRAM, NFS4_VERSION);
	else
		status = rpc_tcp_getclient(rpc_info, NFS_PROGRAM, NFS4_VERSION);
	if (status == -EHOSTUNREACH) {
		supported = status;
		goto done_ver;
	} else if (!status) {
		gettimeofday(&start, &tz);
		status = rpc_ping_proto(rpc_info);
		gettimeofday(&end, &tz);
		if (status == -ETIMEDOUT) {
			supported = status;
			goto done_ver;
		} else if (status > 0) {
			double reply;
			if (random_selection) {
				/* Random value between 0 and 1 */
				reply = ((float) random())/((float) RAND_MAX+1);
				debug(logopt,
				      "nfs v4 random selection time: %f", reply);
			} else {
				reply = elapsed(start, end);
				debug(logopt, "nfs v4 rpc ping time: %f", reply);
			}
			taken += reply;
			count++;
			supported = NFS4_SUPPORTED;
		}
	}

	if (!(version & NFS_VERS_MASK))
		goto done_ver;

v3_ver:
	if (!(version & NFS3_REQUESTED))
		goto v2_ver;

	if (!port && !pm_info->client) {
		status = rpc_portmap_getclient(pm_info,
				host->name, host->addr, host->addr_len,
				proto, RPC_CLOSE_DEFAULT);
		if (status == -EHOSTUNREACH) {
			supported = status;
			goto done_ver;
		} else if (status)
			goto done_ver;
	}

	if (!port) {
		parms.pm_vers = NFS3_VERSION;
		status = rpc_portmap_getport(pm_info, &parms, &rpc_info->port);
		if (status == -EHOSTUNREACH || status == -ETIMEDOUT) {
			supported = status;
			goto done_ver;
		} else if (status < 0)
			goto v2_ver;
	}

	if (rpc_info->proto == IPPROTO_UDP)
		status = rpc_udp_getclient(rpc_info, NFS_PROGRAM, NFS3_VERSION);
	else
		status = rpc_tcp_getclient(rpc_info, NFS_PROGRAM, NFS3_VERSION);
	if (status == -EHOSTUNREACH) {
		supported = status;
		goto done_ver;
	} else if (!status) {
		gettimeofday(&start, &tz);
		status = rpc_ping_proto(rpc_info);
		gettimeofday(&end, &tz);
		if (status == -ETIMEDOUT) {
			supported = status;
			goto done_ver;
		} else if (status > 0) {
			double reply;
			if (random_selection) {
				/* Random value between 0 and 1 */
				reply = ((float) random())/((float) RAND_MAX+1);
				debug(logopt,
				      "nfs v3 random selection time: %f", reply);
			} else {
				reply = elapsed(start, end);
				debug(logopt, "nfs v3 rpc ping time: %f", reply);
			}
			taken += reply;
			count++;
			supported |= NFS3_SUPPORTED;
		}
	}

v2_ver:
	if (!(version & NFS2_REQUESTED))
		goto done_ver;

	if (!port && !pm_info->client) {
		status = rpc_portmap_getclient(pm_info,
				host->name, host->addr, host->addr_len,
				proto, RPC_CLOSE_DEFAULT);
		if (status == -EHOSTUNREACH) {
			supported = status;
			goto done_ver;
		} else if (status)
			goto done_ver;
	}

	if (!port) {
		parms.pm_vers = NFS2_VERSION;
		status = rpc_portmap_getport(pm_info, &parms, &rpc_info->port);
		if (status == -EHOSTUNREACH || status == -ETIMEDOUT) {
			supported = status;
			goto done_ver;
		} else if (status < 0)
			goto done_ver;
	}

	if (rpc_info->proto == IPPROTO_UDP)
		status = rpc_udp_getclient(rpc_info, NFS_PROGRAM, NFS2_VERSION);
	else
		status = rpc_tcp_getclient(rpc_info, NFS_PROGRAM, NFS2_VERSION);
	if (status == -EHOSTUNREACH) {
		supported = status;
		goto done_ver;
	} else if (!status) {
		gettimeofday(&start, &tz);
		status = rpc_ping_proto(rpc_info);
		gettimeofday(&end, &tz);
		if (status == -ETIMEDOUT)
			supported = status;
		else if (status > 0) {
			double reply;
			if (random_selection) {
				/* Random value between 0 and 1 */
				reply = ((float) random())/((float) RAND_MAX+1);
				debug(logopt,
				      "nfs v2 random selection time: %f", reply);
			} else {
				reply = elapsed(start, end);;
				debug(logopt, "nfs v2 rpc ping time: %f", reply);
			}
			taken += reply;
			count++;
			supported |= NFS2_SUPPORTED;
		}
	}

done_ver:
	if (rpc_info->proto == IPPROTO_UDP) {
		rpc_destroy_udp_client(rpc_info);
		rpc_destroy_udp_client(pm_info);
	} else {
		rpc_destroy_tcp_client(rpc_info);
		rpc_destroy_tcp_client(pm_info);
	}

	if (count) {
		/*
		 * Average response time to 7 significant places as
		 * integral type.
		 */
		if (use_weight_only)
			host->cost = 1;
		else
			host->cost = (unsigned long) ((taken * 1000000) / count);

		/* Allow for user bias */
		if (host->weight)
			host->cost *= (host->weight + 1);

		debug(logopt, "host %s cost %ld weight %d",
		      host->name, host->cost, host->weight);
	}

	return supported;
}