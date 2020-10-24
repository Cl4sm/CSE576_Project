static int get_vers_and_cost(unsigned logopt, struct host *host,
			     unsigned int version, int port)
{
	struct conn_info pm_info, rpc_info;
	time_t timeout = RPC_TIMEOUT;
	unsigned int supported, vers = (NFS_VERS_MASK | NFS4_VERS_MASK);
	int ret = 0;

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

	vers &= version;

	if (version & TCP_REQUESTED) {
		supported = get_nfs_info(logopt, host,
				   &pm_info, &rpc_info, IPPROTO_TCP, vers, port);
		if (IS_ERR(supported)) {
			if (ERR(supported) == EHOSTUNREACH ||
			    ERR(supported) == ETIMEDOUT)
				return ret;
		} else if (supported) {
			ret = 1;
			host->version |= supported;
		}
	}

	if (version & UDP_REQUESTED) {
		supported = get_nfs_info(logopt, host,
				   &pm_info, &rpc_info, IPPROTO_UDP, vers, port);
		if (IS_ERR(supported)) {
			if (!ret && ERR(supported) == ETIMEDOUT)
				return ret;
		} else if (supported) {
			ret = 1;
			host->version |= (supported << 8);
		}
	}

	return ret;
}