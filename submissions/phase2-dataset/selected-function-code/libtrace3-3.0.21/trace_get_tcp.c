DLLEXPORT libtrace_tcp_t *trace_get_tcp(libtrace_packet_t *packet) {
	uint8_t proto;
	uint32_t rem = 0;
	libtrace_tcp_t *tcp;

	tcp=(libtrace_tcp_t*)trace_get_transport(packet,&proto,&rem);

	if (!tcp || proto != TRACE_IPPROTO_TCP)
		return NULL;

	/* We should return NULL if there isn't a full TCP header, because the
	 * caller has no way of telling how much of a TCP header we have
	 * returned - use trace_get_transport() if you want to deal with
	 * partial headers 
	 *
	 * NOTE: We're not going to insist that all the TCP options are present
	 * as well, because lots of traces are snapped after 20 bytes of TCP
	 * header and I don't really want to break libtrace programs that
	 * use this function to process those traces */

	if (rem < sizeof(libtrace_tcp_t))
		return NULL;

	return (libtrace_tcp_t*)tcp;
}