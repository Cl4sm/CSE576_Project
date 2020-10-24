int rpc_udp_getclient(struct conn_info *info,
		      unsigned int program, unsigned int version)
{
	CLIENT *client;
	int ret;

	if (!info->client) {
		info->proto = IPPROTO_UDP;
		info->timeout.tv_sec = RPC_TOUT_UDP;
		info->timeout.tv_usec = 0;
		info->send_sz = UDPMSGSIZE;
		info->recv_sz = UDPMSGSIZE;
	}

	info->program = program;
	info->version = version;

	ret = create_client(info, &client);
	if (ret < 0)
		return ret;

	info->client = client;

	return 0;
}