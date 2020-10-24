static rpcprog_t rpc_getrpcbyname(const rpcprog_t program)
{
#ifdef HAVE_GETRPCBYNAME
	static const char *rpcb_pgmtbl[] = {
		"rpcbind", "portmap", "portmapper", "sunrpc", NULL,
	};
	struct rpcent *entry;
	rpcprog_t prog_number;
	unsigned int i;

	pthread_mutex_lock(&rpcb_mutex);
	for (i = 0; rpcb_pgmtbl[i] != NULL; i++) {
		entry = getrpcbyname(rpcb_pgmtbl[i]);
		if (entry) {
			prog_number = entry->r_number;
			pthread_mutex_unlock(&rpcb_mutex);
			return prog_number;
		}
	}
	pthread_mutex_unlock(&rpcb_mutex);
#endif
	return program;
}