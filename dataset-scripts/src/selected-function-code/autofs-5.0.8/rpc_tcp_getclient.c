int rpc_tcp_getclient(struct conn_info *info,
		      unsigned int program, unsigned int version)
{
	CLIENT *client;
	int ret;

	if (!info->client) {
		info->proto = IPPROTO_TCP;
		info->timeout.tv_sec = RPC_TOUT_TCP;
		info->timeout.tv_usec = 0;
		info->send_sz = 0;
		info->recv_sz = 0;
	}

	info->program = program;
	info->version = version;

	ret = create_client(info, &client);
	if (ret < 0)
		return ret;

	info->client = client;

	return 0;
}