static unsigned short rpc_getrpcbport(const int proto)
{
#ifdef HAVE_GETSERVBYNAME
	static const char *rpcb_netnametbl[] = {
		"rpcbind", "portmapper", "sunrpc", NULL,
	};
	struct servent *entry;
	struct protoent *p_ent;
	unsigned short port;
	unsigned int i;

	pthread_mutex_lock(&rpcb_mutex);
	p_ent = getprotobynumber(proto);
	if (!p_ent)
		goto done;
	for (i = 0; rpcb_netnametbl[i] != NULL; i++) {
		entry = getservbyname(rpcb_netnametbl[i], p_ent->p_name);
		if (entry) {
			port = entry->s_port;
			pthread_mutex_unlock(&rpcb_mutex);
			return port;
		}
	}
done:
	pthread_mutex_unlock(&rpcb_mutex);
#endif
	return (unsigned short) PMAPPORT;
}