int rpc_portmap_getclient(struct conn_info *info,
			  const char *host, struct sockaddr *addr, size_t addr_len,
			  int proto, unsigned int option)
{
	CLIENT *client;
	int ret;

	info->host = host;
	info->addr = addr;
	info->addr_len = addr_len;
	info->program = rpc_getrpcbyname(rpcb_prog);
	info->port = ntohs(rpc_getrpcbport(proto));
	/*
	 * When using libtirpc we might need to change the rpcbind version
	 * to qurey AF_INET addresses. Since we might not have an address
	 * yet set AF_INET rpcbind version in rpc_do_create_client() when
	 * we always have an address.
	 */
	info->version = rpcb_version;
	info->proto = proto;
	info->send_sz = RPCSMALLMSGSIZE;
	info->recv_sz = RPCSMALLMSGSIZE;
	info->timeout.tv_sec = PMAP_TOUT_UDP;
	info->timeout.tv_usec = 0;
	info->close_option = option;
	info->client = NULL;

	if (info->proto == IPPROTO_TCP)
		info->timeout.tv_sec = PMAP_TOUT_TCP;

	ret = create_client(info, &client);
	if (ret < 0)
		return ret;

	info->client = client;

	return 0;
}