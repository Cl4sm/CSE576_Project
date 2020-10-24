int rpc_portmap_getport(struct conn_info *info,
			struct pmap *parms, unsigned short *port)
{
	struct conn_info pmap_info;
	CLIENT *client;
	enum clnt_stat status;
	int proto = info->proto;
	int ret;

	memset(&pmap_info, 0, sizeof(struct conn_info));

	if (proto == IPPROTO_TCP)
		pmap_info.timeout.tv_sec = PMAP_TOUT_TCP;
	else
		pmap_info.timeout.tv_sec = PMAP_TOUT_UDP;

	if (info->client)
		client = info->client;
	else {
		pmap_info.host = info->host;
		pmap_info.addr = info->addr;
		pmap_info.addr_len = info->addr_len;
		pmap_info.port = ntohs(rpc_getrpcbport(info->proto));
		pmap_info.program = rpc_getrpcbyname(rpcb_prog);
		/*
		 * When using libtirpc we might need to change the rpcbind
		 * version to qurey AF_INET addresses. Since we might not
		 * have an address yet set AF_INET rpcbind version in
		 * rpc_do_create_client() when we always have an address.
		 */
		pmap_info.version = rpcb_version;
		pmap_info.proto = info->proto;
		pmap_info.send_sz = RPCSMALLMSGSIZE;
		pmap_info.recv_sz = RPCSMALLMSGSIZE;

		ret = create_client(&pmap_info, &client);
		if (ret < 0)
			return ret;
	}

	/*
	 * Check to see if server is up otherwise a getport will take
	 * forever to timeout.
	 */
	status = clnt_call(client, PMAPPROC_NULL,
			 (xdrproc_t) xdr_void, 0, (xdrproc_t) xdr_void, 0,
			 pmap_info.timeout);

	if (status == RPC_SUCCESS) {
		status = clnt_call(client, PMAPPROC_GETPORT,
				 (xdrproc_t) xdr_pmap, (caddr_t) parms,
				 (xdrproc_t) xdr_u_short, (caddr_t) port,
				 pmap_info.timeout);
	}

	if (!info->client) {
		/*
		 * Only play with the close options if we think it
		 * completed OK
		 */
		if (proto == IPPROTO_TCP && status == RPC_SUCCESS) {
			struct linger lin = { 1, 0 };
			socklen_t lin_len = sizeof(struct linger);
			int fd;

			if (!clnt_control(client, CLGET_FD, (char *) &fd))
				fd = -1;

			switch (info->close_option) {
			case RPC_CLOSE_NOLINGER:
				if (fd >= 0)
					setsockopt(fd, SOL_SOCKET, SO_LINGER, &lin, lin_len);
				break;
			}
		}
		clnt_destroy(client);
	}

	if (status == RPC_TIMEDOUT)
		return -ETIMEDOUT;
	else if (status != RPC_SUCCESS)
		return -EIO;

	return 0;
}