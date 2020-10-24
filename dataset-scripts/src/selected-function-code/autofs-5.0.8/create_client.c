static int create_client(struct conn_info *info, CLIENT **client)
{
	struct addrinfo *ai, *haddr;
	struct addrinfo hints;
	int fd, ret;

	fd = RPC_ANYSOCK;
	*client = NULL;

	if (info->client) {
		if (!clnt_control(info->client, CLGET_FD, (char *) &fd)) {
			fd = RPC_ANYSOCK;
			clnt_destroy(info->client);
			info->client = NULL;
		} else {
			clnt_control(info->client, CLSET_FD_NCLOSE, NULL);
			clnt_destroy(info->client);
		}
	}

	if (info->addr) {
		ret = rpc_do_create_client(info->addr, info, &fd, client);
		if (ret == 0)
			goto done;
		if (ret == -EHOSTUNREACH)
			goto out_close;

		if (!info->client && fd != RPC_ANYSOCK) {
			close(fd);
			fd = RPC_ANYSOCK;
		}
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_ADDRCONFIG;
	hints.ai_family = AF_UNSPEC;
	if (info->proto == IPPROTO_UDP)
		hints.ai_socktype = SOCK_DGRAM;
	else
		hints.ai_socktype = SOCK_STREAM;

	ret = getaddrinfo(info->host, NULL, &hints, &ai);
	if (ret) {
		error(LOGOPT_ANY,
		      "hostname lookup failed: %s", gai_strerror(ret));
		info->client = NULL;
		goto out_close;
	}

	haddr = ai;
	while (haddr) {
		if (haddr->ai_protocol != info->proto) {
			haddr = haddr->ai_next;
			continue;
		}

		ret = rpc_do_create_client(haddr->ai_addr, info, &fd, client);
		if (ret == 0)
			break;
		if (ret == -EHOSTUNREACH)
			goto out_close;

		if (!info->client && fd != RPC_ANYSOCK) {
			close(fd);
			fd = RPC_ANYSOCK;
		}

		haddr = haddr->ai_next;
	}

	freeaddrinfo(ai);

done:
	if (!*client) {
		info->client = NULL;
		ret = -ENOTCONN;
		goto out_close;
	}

	/* Close socket fd on destroy, as is default for rpcowned fds */
	if  (!clnt_control(*client, CLSET_FD_CLOSE, NULL)) {
		clnt_destroy(*client);
		info->client = NULL;
		ret = -ENOTCONN;
		goto out_close;
	}

	return 0;

out_close:
	if (fd != -1)
		close(fd);
	return ret;
}