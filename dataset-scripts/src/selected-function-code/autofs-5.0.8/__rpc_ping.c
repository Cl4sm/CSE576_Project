static int __rpc_ping(const char *host,
		      unsigned long version, int proto,
		      long seconds, long micros, unsigned int option)
{
	int status;
	struct conn_info info;
	struct pmap parms;

	info.proto = proto;
	info.host = host;
	info.addr = NULL;
	info.addr_len = 0;
	info.program = NFS_PROGRAM;
	info.version = version;
	info.send_sz = 0;
	info.recv_sz = 0;
	info.timeout.tv_sec = seconds;
	info.timeout.tv_usec = micros;
	info.close_option = option;
	info.client = NULL;

	status = RPC_PING_FAIL;

	parms.pm_prog = NFS_PROGRAM;
	parms.pm_vers = version;
	parms.pm_prot = info.proto;
	parms.pm_port = 0;

	status = rpc_portmap_getport(&info, &parms, &info.port);
	if (status < 0)
		return status;

	status = rpc_ping_proto(&info);

	return status;
}