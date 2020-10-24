static int rpc_do_create_client(struct sockaddr *addr, struct conn_info *info, int *fd, CLIENT **client)
{
	CLIENT *clnt = NULL;
	struct sockaddr_in in4_laddr;
	struct sockaddr_in *in4_raddr;
	int type, proto, ret;
	socklen_t slen;

	*client = NULL;

	proto = info->proto;
	if (proto == IPPROTO_UDP)
		type = SOCK_DGRAM;
	else
		type = SOCK_STREAM;

	/*
	 * bind to any unused port.  If we left this up to the rpc
	 * layer, it would bind to a reserved port, which has been shown
	 * to exhaust the reserved port range in some situations.
	 */
	in4_laddr.sin_family = AF_INET;
	in4_laddr.sin_port = htons(0);
	in4_laddr.sin_addr.s_addr = htonl(INADDR_ANY);
	slen = sizeof(struct sockaddr_in);

	if (!info->client) {
		struct sockaddr *laddr;

		*fd = open_sock(addr->sa_family, type, proto);
		if (*fd < 0)
			return -errno;

		laddr = (struct sockaddr *) &in4_laddr;
		if (bind(*fd, laddr, slen) < 0)
			return -errno;
	}

	in4_raddr = (struct sockaddr_in *) addr;
	in4_raddr->sin_port = htons(info->port);

	switch (info->proto) {
	case IPPROTO_UDP:
		clnt = clntudp_bufcreate(in4_raddr,
					 info->program, info->version,
					 info->timeout, fd,
					 info->send_sz, info->recv_sz);
		break;

	case IPPROTO_TCP:
		ret = connect_nb(*fd, addr, slen, &info->timeout);
		if (ret < 0)
			return ret;

		clnt = clnttcp_create(in4_raddr,
				      info->program, info->version, fd,
				      info->send_sz, info->recv_sz);
		break;

	default:
		break;
	}

	*client = clnt;

	return 0;
}