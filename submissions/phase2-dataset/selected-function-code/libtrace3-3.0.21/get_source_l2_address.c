static struct sockaddr *get_source_l2_address(
	const libtrace_packet_t *packet, struct sockaddr *addr)
{
	static struct sockaddr_storage dummy;
	void *l2;
	libtrace_linktype_t linktype;
	uint32_t remaining;

	if (!addr)
		addr =(struct sockaddr*)&dummy;

	l2=trace_get_layer2(packet, &linktype, &remaining);
	if (!l2) {
		return NULL;
	}

	switch (linktype) {
		case TRACE_TYPE_ETH:
			return get_source_ethernet_address((libtrace_ether_t*)l2, addr);
		default:
			return NULL;
	}
}