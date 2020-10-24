int rpc_ping(const char *host, long seconds, long micros, unsigned int option)
{
	unsigned long vers3 = NFS3_VERSION;
	unsigned long vers2 = NFS2_VERSION;
	int status;

	status = __rpc_ping(host, vers2, IPPROTO_UDP, seconds, micros, option);
	if (status > 0)
		return RPC_PING_V2 | RPC_PING_UDP;

	status = __rpc_ping(host, vers3, IPPROTO_UDP, seconds, micros, option);
	if (status > 0)
		return RPC_PING_V3 | RPC_PING_UDP;

	status = __rpc_ping(host, vers2, IPPROTO_TCP, seconds, micros, option);
	if (status > 0)
		return RPC_PING_V2 | RPC_PING_TCP;

	status = __rpc_ping(host, vers3, IPPROTO_TCP, seconds, micros, option);
	if (status > 0)
		return RPC_PING_V3 | RPC_PING_TCP;

	return status;
}