static int rpc_get_exports_proto(struct conn_info *info, exports *exp)
{
	CLIENT *client;
	enum clnt_stat status;
	int proto = info->proto;
	unsigned int option = info->close_option;
	int vers_entry;
	int ret;

	if (info->proto == IPPROTO_UDP) {
		info->send_sz = UDPMSGSIZE;
		info->recv_sz = UDPMSGSIZE;
	}
	ret = create_client(info, &client);
	if (ret < 0)
		return 0;

	clnt_control(client, CLSET_TIMEOUT, (char *) &info->timeout);
	clnt_control(client, CLSET_RETRY_TIMEOUT, (char *) &info->timeout);

	client->cl_auth = authunix_create_default();

	vers_entry = 0;
	while (1) {
		status = clnt_call(client, MOUNTPROC_EXPORT,
				 (xdrproc_t) xdr_void, NULL,
				 (xdrproc_t) xdr_exports, (caddr_t) exp,
				 info->timeout);
		if (status == RPC_SUCCESS)
			break;
		if (++vers_entry > 2)
			break;
		CLNT_CONTROL(client, CLSET_VERS,
			    (void *) &mount_vers[vers_entry]);
	}

	/* Only play with the close options if we think it completed OK */
	if (proto == IPPROTO_TCP && status == RPC_SUCCESS) {
		struct linger lin = { 1, 0 };
		socklen_t lin_len = sizeof(struct linger);
		int fd;

		if (!clnt_control(client, CLGET_FD, (char *) &fd))
			fd = -1;

		switch (option) {
		case RPC_CLOSE_NOLINGER:
			if (fd >= 0)
				setsockopt(fd, SOL_SOCKET, SO_LINGER, &lin, lin_len);
			break;
		}
	}
	auth_destroy(client->cl_auth);
	clnt_destroy(client);

	if (status != RPC_SUCCESS)
		return 0;

	return 1;
}